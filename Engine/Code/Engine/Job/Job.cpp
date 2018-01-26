#include "Engine/Job/Job.hpp"

#include "Engine/DataStructures/ThreadSafeQueue.hpp"
#include "Engine/Threading/Signal.hpp"
#include "Engine/Core/Atomic.hpp"
#include "Engine/Threading/Thread.hpp"

#include "Engine/Profiler/Profiler.hpp"
#include "Engine/Core/StringUtils.hpp"

#include "Engine/Renderer/SimpleRenderer.hpp"
#include "Game/GameCommon.hpp"

class JobSystem
{
public:
    ThreadSafeQueue<Job*> *queues;
    Signal **signals;
    uint queue_count;
    JobConsumer* consumer;

    bool is_running;
};

static JobSystem *gJobSystem = nullptr;

//************************************************************************

//------------------------------------------------------------------------
static void GenericJobThread(Signal *signal)
{
    JobConsumer* genericConsumer = gJobSystem->consumer;

    while (gJobSystem->is_running) {
        signal->wait();
        genericConsumer->JobConsumeAll(JOB_GENERIC);
    }

    genericConsumer->JobConsumeAll(JOB_GENERIC);
}

//------------------------------------------------------------------------
void Job::on_finish()
{
    for (uint i = 0; i < dependents.size(); ++i) {
        dependents[i]->on_dependancy_finished();
    }
}

//------------------------------------------------------------------------
void Job::on_dependancy_finished()
{
    JobDispatchAndRelease(this);
}

//------------------------------------------------------------------------
void Job::dependent_on(Job *parent)
{
    AtomicIncrement(&num_dependencies);
    parent->dependents.push_back(this);
}

uint category_count = 0;
//------------------------------------------------------------------------
void JobSystemStartup(uint job_category_count, int generic_thread_count /*= -1*/)
{
    category_count = job_category_count;
    int core_count = (int)std::thread::hardware_concurrency();
    if (generic_thread_count <= 0) {
        core_count += generic_thread_count;
    }
    core_count--; // one is always being created - so subtract from total wanted;

                  // We need queues! 
    gJobSystem = new JobSystem();
    gJobSystem->queues = new ThreadSafeQueue<Job*>[job_category_count];
    gJobSystem->signals = new Signal*[job_category_count];
    gJobSystem->queue_count = job_category_count;
    gJobSystem->is_running = true;

    gJobSystem->consumer = new JobConsumer();
    gJobSystem->consumer->AddCategory(JOB_GENERIC);

    for (uint i = 0; i < job_category_count; ++i) {
        gJobSystem->signals[i] = nullptr;
    }

    // create the signal
    gJobSystem->signals[JOB_GENERIC] = new Signal();

    ThreadCreateWithArguments(GenericJobThread, gJobSystem->signals[JOB_GENERIC]);
    for (int i = 0; i < core_count; ++i) {
        ThreadCreateWithArguments(GenericJobThread, gJobSystem->signals[JOB_GENERIC]);
    }
}

//------------------------------------------------------------------------
void JobSystemShutdown()
{
    for (int index = 0; index < (int)category_count; index++)
    {
        delete gJobSystem->signals[index];
    }
    delete[] gJobSystem->signals;

    Job *job;
    uint processed_jobs = 0;

    for (int index = 0; index < JOB_TYPE_COUNT; index++)
    {
        ThreadSafeQueue<Job*> &queue = gJobSystem->queues[index];
        while (queue.pop(&job)) {
            job->work_cb(job->user_data);
            ++processed_jobs;

            job->on_finish();
            delete job;
        }
    }

    delete gJobSystem;
    gJobSystem = nullptr;
}


//------------------------------------------------------------------------
Job* JobCreate(eJobType type, job_work_cb work_cb, void *user_data)
{
    Job *job = new Job();
    job->type = type;
    job->work_cb = work_cb;
    job->user_data = user_data;
    job->num_dependencies = 1;

    return job;
}

//************************************************************************
Job* JobCreate(eJobType type, on_texture_loaded_cb_four work_cb, on_texture_loaded_cb cb, Texture2D* tex, void* userArg)
{
    UNUSED(cb);
    load_texture_job_t* user_data = new load_texture_job_t();
    user_data->m_texture = tex;

    Job *job = new Job();
    job->type = type;
    job->textureFour_cb = work_cb;
    job->user_data = userArg;
    job->num_dependencies = 1;

    return job;
}

//************************************************************************
Job* JobCreateLoadImage(eJobType type, on_texture_loaded_cb_two work_cb, Image* img, std::string* name)
{
    load_texture_job_t* user_data = new load_texture_job_t();
    user_data->m_image = img;
    user_data->m_filename = *name;
    user_data->m_texture = nullptr;

    Job *job = new Job();
    job->type = type;
    job->textureTwo_cb = work_cb;
    job->user_data = (void*)user_data;
    job->num_dependencies = 1;

    return job;
}

//************************************************************************
Job* JobCreateTexture(eJobType type, on_texture_loaded_cb_three work_cb, Texture2D* tex, Image* img)
{
    load_texture_job_t* user_data = new load_texture_job_t();
    user_data->m_image = img;
    user_data->m_filename = nullptr;
    user_data->m_texture = tex;

    Job *job = new Job();
    job->type = type;
    job->textureThree_cb = work_cb;
    job->user_data = (void*)user_data;
    job->num_dependencies = 1;

    return job;
}

//------------------------------------------------------------------------
void JobDispatchAndRelease(Job *job)
{
    // if I'm not ready to run, don't. 
    uint dcount = AtomicDecrement(&job->num_dependencies);
    if (dcount != 0) {
        return;
    }

    gJobSystem->queues[job->type].push(job);
    job->state = DISPATCHED;

    Signal *signal = gJobSystem->signals[job->type];
    if (nullptr != signal) {
        signal->signal_all();
    }
    job->state = ENQUEUED;
}

//************************************************************************
void JobDispath(Job *job)
{
    uint dcount = AtomicDecrement(&job->num_dependencies);
    if (dcount != 0) {
        return;
    }
    gJobSystem->queues[job->type].push(job);
    job->state = DISPATCHED;
}

//************************************************************************
void JobRelease(Job *job)
{
    Signal *signal = gJobSystem->signals[job->type];
    if (nullptr != signal) {
        signal->signal_all();
    }
    job->state = ENQUEUED;
}

//------------------------------------------------------------------------
// THIS SHOULD BE MOVED TO A JOB CONSUMER OBJECT!
uint JobConsumeAll(eJobType type)
{
    Job *job;
    uint processed_jobs = 0;

    ThreadSafeQueue<Job*> &queue = gJobSystem->queues[type];
    while (queue.pop(&job)) {
        job->work_cb(job->user_data);
        ++processed_jobs;

        job->on_finish();
        job->state = FINISHED;
        delete job;
    }

    return processed_jobs;
}

//************************************************************************
void JobWait(Job *job)
{
    while (job->state != FINISHED)
    {
        // Loops until Flush has finished pushing to file
        ThreadYield();
    }
}

//************************************************************************
void JobWaitAndRelease(Job *job)
{
    JobWait(job);
    JobRelease(job);
}

//************************************************************************
void JobRun(job_work_cb ptr, create_file_job_t* jobPtr)
{
    //PROFILE_LOG_SCOPE("JobRun");
    //Supply the CreateLargeFile function
    Job *largeFileJob = JobCreate(JOB_GENERIC, ptr, jobPtr);
    JobDispatchAndRelease(largeFileJob);
}

//************************************************************************
void JobCreateLargeFile(void *userPtr)
{
    create_file_job_t* data = (create_file_job_t*)userPtr;
    ThreadTest(data->m_filename.c_str(), data->m_byteSize);
    delete data;
}

//************************************************************************
void CreateLargeFileAsync(char const *filename, uint byteSize)
{
    create_file_job_t* jobData = new create_file_job_t();
    jobData->m_filename = filename;
    jobData->m_byteSize = byteSize;

    // Job Run creates, then dispatches & releases the job - it truely is the fire and forget of the job system.
    JobRun(JobCreateLargeFile, jobData);
}

//************************************************************************
JobConsumer::JobConsumer()
{}

//************************************************************************
JobConsumer::~JobConsumer()
{}

//************************************************************************
void JobConsumer::AddCategory(eJobType type)
{
    m_types.push_back(type);
}

//************************************************************************
bool JobConsumer::ConsumeJob(eJobType type)
{
    Job* job;
    ThreadSafeQueue<Job*> &queue = gJobSystem->queues[type];

    if (queue.pop(&job))
    {
        job->work_cb(job->user_data);
        job->on_finish();
        job->state = FINISHED;
        delete job;
        return true;
    }
    return false;

}

//************************************************************************
uint JobConsumer::JobConsumeAll(eJobType type)
{
    Job *job;
    uint processed_jobs = 0;

    ThreadSafeQueue<Job*> &queue = gJobSystem->queues[type];
    while (queue.pop(&job)) {
        job->work_cb(job->user_data);
        ++processed_jobs;

        job->on_finish();
        job->state = FINISHED;
        delete job;
    }

    return processed_jobs;
}

//************************************************************************
void JobSystemSetCategoryAndSignal(eJobType type, Signal* signal)
{
    gJobSystem->consumer->AddCategory(type);
    gJobSystem->signals[type] = signal;
}

//************************************************************************
#include <ctime>
void JobLoggerThread(void* userArg)
{
    JobConsumer logConsumer;
    logConsumer.AddCategory(JOB_LOGGING);
    JobSystemSetCategoryAndSignal(JOB_LOGGING, GetLogSignal());

    FILE *fh = nullptr;

    time_t currentTime = time(0);
    tm* currentDate = localtime(&currentTime);
    char filename[256] = { 0 };
    strcat(filename, Stringf("%d%d%d_%d%d%d", currentDate->tm_mday, currentDate->tm_mon + 1, currentDate->tm_year + 1900, currentDate->tm_hour, currentDate->tm_min, currentDate->tm_sec).c_str());

    char const* path = (char const*)userArg;
    if (path == nullptr)
        path = "log.log";

    char const* fileDirectories = "../Run_Win32/Data/Log/";

    char oldPath[256];
    strcpy(oldPath, fileDirectories);
    strcat(oldPath, path);
    strcat(oldPath, ".log");

    char newPath[256];
    strcpy(newPath, fileDirectories);
    strcat(newPath, path);
    strcat(newPath, "_");
    strcat(newPath, filename);
    strcat(newPath, ".log");

    WCHAR fromPath[1024];
    MultiByteToWideChar(GetACP(), 0, path, -1, fromPath, sizeof(fromPath) / sizeof(fromPath[0]));
    WCHAR toPath[1024];
    MultiByteToWideChar(GetACP(), 0, newPath, -1, toPath, sizeof(toPath) / sizeof(toPath[0]));
    CopyFile(fromPath, toPath, TRUE);


    errno_t err = fopen_s(&fh, newPath, "a+");
    if ((err != 0) || (fh == nullptr)) {
        return;
    }

    GetLogEvent()->subscribe(fh, LogWriteToFile);

    while (IsLoggerThreadRunning())
    {
        GetLogSignal()->wait();
        FlushMessages(fh);
        logConsumer.JobConsumeAll(JOB_LOGGING);
    }

    FlushMessages(fh);
    fclose(fh);
}

//************************************************************************
void LoadImageFromFileJob(Image* outImage, std::string* str)
{
    outImage = new Image();
    outImage->LoadFromFile(str->c_str());
}

//************************************************************************
void LoadTextureFromImageJob(Texture2D *tex, Image *img)
{
    if (img == nullptr)
    {
        std::string imageName = "Data/Images/Fuschia.png";
        tex = new Texture2D(g_theSimpleRenderer->m_device, imageName.c_str());
    }

    tex = new Texture2D(g_theSimpleRenderer->m_device, *img);
}

//************************************************************************
void CleanupJob(void* ptr)
{
    load_texture_job_t* jobData = (load_texture_job_t*)ptr;

    if (jobData->m_image != nullptr)
    {
        delete jobData->m_image;
        jobData->m_image = nullptr;
    }

    if (jobData->m_texture != nullptr)
    {
        delete jobData->m_texture;
        jobData->m_texture = nullptr;
    }

    delete ptr;
}

//************************************************************************
void CallLoadTextureCallbackJob(on_texture_loaded_cb cb, Texture2D *tex, void* userArg)
{
    cb(tex, userArg);
}

