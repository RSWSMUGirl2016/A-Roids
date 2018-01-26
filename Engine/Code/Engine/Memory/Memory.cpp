/************************************************************************/
/*                                                                      */
/* INCLUDE                                                              */
/*                                                                      */
/************************************************************************/
#include "Engine/Memory/Memory.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Memory/BuildConfig.hpp"

#include <malloc.h>

/************************************************************************/
/*                                                                      */
/* DEFINES AND CONSTANTS                                                */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* MACROS                                                               */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* TYPES                                                                */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* STRUCTS                                                              */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* CLASSES                                                              */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* LOCAL VARIABLES                                                      */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* GLOBAL VARIABLES                                                     */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* LOCAL FUNCTIONS                                                      */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* EXTERNAL FUNCTIONS                                                   */
/*                                                                      */
/************************************************************************/


static uint gAllocCount = 0;
static uint gFrameAllocs = 0;
static uint gFrameFrees = 0;
static uint gPrevFrameAllocs = 0;
static uint gPrevFrameFrees = 0;
static size_t gAllocatedByteCount = 0;
static size_t gHighwaterByteCount = 0;

LinkedList gAllocactionList = LinkedList();

struct allocation_t
{
    size_t byte_size;
};


#if defined TRACK_MEMORY

void* operator new(size_t const size)
{
    ++gAllocCount;
    ++gFrameAllocs;
    gAllocatedByteCount += size;

    if (gAllocatedByteCount >= gHighwaterByteCount)
        gHighwaterByteCount = gAllocatedByteCount;


    size_t alloc_size = size + sizeof(allocation_t);
    allocation_t *ptr = (allocation_t*)malloc(alloc_size);
    ptr->byte_size = size;

#if TRACK_MEMORY == TRACK_MEMORY_VERBOSE
    gAllocactionList.CreateNode(CreateCallstack(0), (uint)size);
#endif

    return ptr + 1;
}

void operator delete(void *ptr)
{
    --gAllocCount;
    ++gFrameFrees;

    allocation_t *size_ptr = (allocation_t*)ptr;

    if (size_ptr == nullptr)
    {
        //free(size_ptr);
        return;
    }
    if (size_ptr->byte_size > gAllocatedByteCount)
    {
        return;
    }
        
    size_ptr--;
    gAllocatedByteCount -= size_ptr->byte_size;
    free(size_ptr);

#if TRACK_MEMORY == TRACK_MEMORY_VERBOSE
        gAllocactionList.DeleteLast();
#endif
}

#endif

class Foo
{
public:
    Foo()
    {
        printf("Foo::ctor\n");
    }

    ~Foo()
    {
        printf("Foo::dtor\n");
    }
};

void ProfileMemoryFrameTick()
{
    gPrevFrameAllocs = gFrameAllocs;
    gPrevFrameFrees = gFrameFrees;

    gFrameAllocs = 0;
    gFrameFrees = 0;
}

#include <map>
#include <string>
#include <random>

void MemTest()
{
    int *array = new int[1000];
    uint3 *f3 = new uint3();
    f3->x = 1;
    f3->y = 2;
    f3->z = 3;

    byte_t buffer[128];
    Foo *test = new (buffer) Foo();
    test->~Foo();

    delete[] array;
    delete f3;
}
uint GetAllocCount()
{
    return gAllocCount;
}

uint MemoryProfilerGetTotalByteCount()
{
    return gAllocatedByteCount;
}

uint MemoryProfilerGetAllocationCount()
{
    return gAllocCount;
}

uint MemoryProfilerHighwaterByteCount()
{
    return gHighwaterByteCount;
}

uint MemoryProfilerGetAllocationsLastFrame()
{
    return gPrevFrameAllocs;
}

uint MemoryProfilerGetFreesLastFrame()
{
    return gPrevFrameFrees;
}

void MemoryProfilerLogLiveAllocations()
{
    Node *currentNode = gAllocactionList.m_head;
    while (currentNode != nullptr)
    {
        //DebuggerPrintf("%i leaked allocations.  Total: %i\n", gAllocactionList.m_head->m_allocation, gAllocactionList.m_head->m_allocation * 8);        

        /*callstack_line_t *line_buffer = (callstack_line_t*)malloc(sizeof(callstack_line_t));
        uint const max_lines = 3;
        CallstackGetLines(line_buffer, max_lines, gAllocactionList.m_head->m_callstack);

        
        DebuggerPrintf("%s", std::string(line_buffer->filename));
        DebuggerPrintf("(");
        DebuggerPrintf("%i", (int)line_buffer->line);
        DebuggerPrintf(") : ");
        DebuggerPrintf("%s\n", std::string(line_buffer->function_name));*/

        gAllocactionList.m_head = gAllocactionList.m_head->m_next;
        currentNode = gAllocactionList.m_head;
    }
}

void ConsoleMemProfLogLiveAlloc()
{
    Node *currentNode = gAllocactionList.m_head;
    while (currentNode != nullptr)
    {
        std::string message = Stringf("%i leaked allocations.  Total: %i", gAllocactionList.m_head->m_allocation, gAllocactionList.m_head->m_allocation * 8);

        console_message cm;
        cm.message = message;
        cm.color = Rgba(0, 255, 255, 255);

        g_theConsole->m_consoleMessages.push_back(cm);
        g_theConsole->m_cursorY -= g_theSimpleRenderer->m_font->m_lineHeight;

        gAllocactionList.m_head = gAllocactionList.m_head->m_next;
        currentNode = gAllocactionList.m_head;
    }
}

/*
void PrintLiveAllocations()
{
for each (allocation) {
printf( "Allocation at 0x%08x took %u B\n",
allocation->ptr,
allocation->size );
}
}
*/

/************************************************************************/
/*                                                                      */
/* COMMANDS                                                             */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* UNIT TESTS                                                           */
/*                                                                      */
/************************************************************************/

