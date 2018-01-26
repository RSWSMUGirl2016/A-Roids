#pragma once
#if !defined( __JOB__ )
#define __JOB__

#include "Engine/EngineCommon.hpp"

#include "Engine/DataStructures/ThreadSafeQueue.hpp"
#include "Engine/Threading/Signal.hpp"
#include "Engine/Core/Atomic.hpp"

#include <string>

enum eJobType
{
    JOB_GENERIC = 0,
    JOB_MAIN,
    JOB_IO,
    JOB_RENDER,
    JOB_LOGGING,

    JOB_TYPE_COUNT,
};

enum eJobState
{
    CREATED,
    DISPATCHED,
    ENQUEUED,
    RUNNING,
    FINISHED
};

class Job;
void JobDispatchAndRelease(Job *job);

void JobDispath(Job *job);
void JobRelease(Job *job);


typedef void(*job_work_cb)(void*);

//************************************************************************
#include "Engine/RHI/Texture2D.hpp"
#include "Engine/Core/Image.hpp"
struct load_texture_job_t
{
    std::string m_filename;
    Image *m_image;
    Texture2D *m_texture;
};
//************************************************************************
typedef void(*on_texture_loaded_cb)(Texture2D* tex, void* userArg);
typedef void(*on_texture_loaded_cb_two)(Image* img, std::string* name);
typedef void(*on_texture_loaded_cb_three)(Texture2D* tex, Image* img);
typedef void(*on_texture_loaded_cb_four)(on_texture_loaded_cb cb, Texture2D* tex, void* userArg);

//--------------------------------------------------------------------
//--------------------------------------------------------------------
class Job
{
public:
    eJobType type;
    eJobState state = CREATED;
    job_work_cb work_cb;
    on_texture_loaded_cb texture_cb;
    on_texture_loaded_cb_two textureTwo_cb;
    on_texture_loaded_cb_three textureThree_cb;
    on_texture_loaded_cb_four textureFour_cb;

    void *user_data;

    std::vector<Job*> dependents;
    uint num_dependencies;

public:
    void on_finish();
    void on_dependancy_finished();

    void dependent_on(Job *parent);
};

//--------------------------------------------------------------------
//--------------------------------------------------------------------
class JobConsumer
{
public: 
    std::vector<eJobType> m_types;

    JobConsumer();
    ~JobConsumer();

    void AddCategory(eJobType type);
    bool ConsumeJob(eJobType type);
    uint JobConsumeAll(eJobType type);
};

/************************************************************************/
void JobSystemStartup(uint job_category_count, int generic_thread_count = -1);
void JobSystemShutdown();

Job* JobCreate(eJobType type, job_work_cb work_cb, void *user_data);
void JobDispatchAndRelease(Job *job);


//------------------------------------------------------------------------
// THIS SHOULD BE MOVED TO A JOB CONSUMER OBJECT!
uint JobConsumeAll(eJobType type);

//************************************************************************
struct create_file_job_t
{
    std::string m_filename;
    uint m_byteSize;
};

//************************************************************************
void JobWait(Job *job);

//************************************************************************
void JobWaitAndRelease(Job *job);

//************************************************************************
void JobRun(job_work_cb ptr, create_file_job_t* jobPtr);

//************************************************************************
void JobCreateLargeFile(void *userPtr);

//************************************************************************
void CreateLargeFileAsync(char const *filename, uint byteSize);

//************************************************************************
void JobLoggerThread(void* userArg);

//************************************************************************
Job* JobCreateLoadImage(eJobType type, on_texture_loaded_cb_two work_cb, Image* img, std::string* name);

//************************************************************************
Job* JobCreateTexture(eJobType type, on_texture_loaded_cb_three work_cb, Texture2D* tex, Image* img);

//************************************************************************
Job* JobCreate(eJobType type, on_texture_loaded_cb_four work_cb, on_texture_loaded_cb cb, Texture2D* tex, void* userArg);

//************************************************************************
void LoadImageFromFileJob(Image* outImage, std::string* str);

//************************************************************************
void LoadTextureFromImageJob(Texture2D *tex, Image *img);

//************************************************************************
void CleanupJob(void* ptr);

//************************************************************************
void CallLoadTextureCallbackJob(on_texture_loaded_cb cb, Texture2D *tex, void* userArg);
#endif 
