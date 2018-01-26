#include "Engine/Console.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"
#include "Game/App.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Tools/fbx.h"
#include "Engine/Memory/Memory.hpp"
#include "Engine/Threading/Thread.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Profiler/Profiler.hpp"
#include "Engine/Job/Job.hpp"
#include "Engine/Net/tcpsocket.hpp"
#include "Engine/Net/RemoteCommandService.hpp"
#include "Game/Game.hpp"

#include <sstream>
#include <vector>

//*******************************************************************
bool ConsoleClear(std::string args)
{
    g_theConsole->Clear(args);
    return true;
}

//*******************************************************************
bool ConsoleHelp(std::string args)
{
    g_theConsole->Help(args);
    return true;
}

//*******************************************************************
bool ConsoleQuit(std::string args)
{
    UNUSED(args);
    g_theApp->OnExitRequested();
    return true;
}

//*******************************************************************
bool ConsoleFbxFileList(std::string args)
{
    FbxListFile(args.c_str());
    return true;
}

//*******************************************************************
bool ConsoleBasicMemoryProfiling(std::string args)
{
    g_theConsole->BasicMemoryProfiling(args);
    return true;
}

//*******************************************************************
bool ConsoleMemoryProfilerLogLiveAllocations(std::string args)
{
    UNUSED(args);
    ConsoleMemProfLogLiveAlloc();
    return true;
}

//*******************************************************************
bool ConsoleThreadTest(std::string args)
{
    std::string filepath = "";
    uint byte_size = 0;

    std::istringstream iStringStream(args);
    std::string stringToken;
    std::vector<std::string> parts;
    while (std::getline(iStringStream, stringToken, ' '))
    {
        parts.push_back(stringToken);
    }

    if ((int)parts.size() == 0 || (int)parts.size() > 2)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT # OF PARAMS: %s", "THREADTEST");
        return true;
    }

    std::string::size_type sz;

    filepath = parts[0];
    byte_size = std::stoi(parts[1], &sz);

    if (filepath == "" || byte_size == 0)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "THREADTEST");
        return true;
    }

    ThreadTest(filepath.c_str(), byte_size);
    return true;
}

//*******************************************************************
bool ConsoleLogTest(std::string args)
{
    int numThreads = 0;
    int numLines = 0;

    std::istringstream iStringStream(args);
    std::string stringToken;
    std::vector<std::string> parts;
    while (std::getline(iStringStream, stringToken, ' '))
    {
        parts.push_back(stringToken);
    }

    if ((int)parts.size() == 0 || (int)parts.size() > 2)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT # OF PARAMS: %s", "LOGTEST");
        return true;
    }

    std::string::size_type sz;

    numThreads = std::stoi(parts[0], &sz);
    numLines = std::stoi(parts[1], &sz);

    if (numThreads == 0 || numLines == 0)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "LOGTEST");
        return true;
    }

    LogTest(numThreads, numLines);
    return true;
}

//*******************************************************************
bool ConsoleLogFlushTest(std::string args)
{
    std::string text = "";
    text = args;

    if (text == "")
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "FLUSHTEST");
        return true;
    }

    LogFlushTest(text.c_str());
    return true;
}

//*******************************************************************
bool ConsoleLogTagged(std::string args)
{
    std::string tag = "";
    std::string format = "";
    std::string varargs = "";

    std::istringstream iStringStream(args);
    std::string stringToken;
    std::vector<std::string> parts;
    int partsCounter = 0;
    while (std::getline(iStringStream, stringToken, ' '))
    {
        if (partsCounter > 1)
        {
            varargs.append(stringToken);
            if (stringToken.at(stringToken.length() - 1) != '\0')
            {
                varargs.append(", ");
            }
        }
        parts.push_back(stringToken);
        partsCounter++;
    }

    if ((int)parts.size() == 0 || (int)parts.size() > 2)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT # OF PARAMS: %s", "LOGDEFAULT");
        return true;
    }

    tag = parts[0];
    format = parts[1];

    if (tag == "" || format == "" || format == "")
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "LOGDEFAULT");
        return true;
    }

    LogTaggedPrintf(tag.c_str(), format.c_str(), varargs.c_str());
    return true;
}

//*******************************************************************
bool ConsoleProfilerPause(std::string args)
{
    UNUSED(args);
    g_profiler->ProfilerPause();
    return true;
}

//*******************************************************************
bool ConsoleProfilerResume(std::string args)
{
    UNUSED(args);
    g_profiler->ProfilerResume();
    return true;
}

//*******************************************************************
bool ConsoleProfilerSwitchFrameView(std::string args)
{
    UNUSED(args);
    //g_theApp->m_treeView = !g_theApp->m_treeView;
    return true;
}

//*******************************************************************
bool ConsoleJobGenerateFile(std::string args)
{
    std::string filepath = "";
    uint byte_size = 0;

    std::istringstream iStringStream(args);
    std::string stringToken;
    std::vector<std::string> parts;
    while (std::getline(iStringStream, stringToken, ' '))
    {
        parts.push_back(stringToken);
    }

    if ((int)parts.size() == 0 || (int)parts.size() > 2)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT # OF PARAMS: %s", "JOBGENERATEFILE");
        return true;
    }

    std::string::size_type sz;

    filepath = parts[0];
    byte_size = std::stoi(parts[1], &sz);

    if (filepath == "" || byte_size == 0)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "JOBGENERATEFILE");
        return true;
    }

    CreateLargeFileAsync(filepath.c_str(), byte_size);
    return true;
}

//*******************************************************************
bool ConsoleJobLoggerThread(std::string args)
{
    std::string filepath = "";

    std::istringstream iStringStream(args);
    std::string stringToken;
    std::vector<std::string> parts;
    while (std::getline(iStringStream, stringToken, ' '))
    {
        parts.push_back(stringToken);
    }

    if ((int)parts.size() == 0 || (int)parts.size() > 1)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT # OF PARAMS: %s", "JOBLOGGERFILE");
        return true;
    }

    //std::string::size_type sz;
    filepath = parts[0];

    if (filepath == "")
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "JOBLOGGERFILE");
        return true;
    }

    JobLoggerThread((void*)(filepath.c_str()));
    return true;
}

//*******************************************************************
bool ConsoleSetTextureToCube(std::string args)
{
    std::string filepath = "";

    std::istringstream iStringStream(args);
    std::string stringToken;
    std::vector<std::string> parts;
    while (std::getline(iStringStream, stringToken, ' '))
    {
        parts.push_back(stringToken);
    }

    if ((int)parts.size() == 0 || (int)parts.size() > 1)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT # OF PARAMS: %s", "SETCUBETEXTURE");
        return true;
    }

    //std::string::size_type sz;
    filepath = parts[0];

    if (filepath == "")
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "SETCUBETEXTURE");
        return true;
    }

    Texture2D* newTexture = g_theSimpleRenderer->LoadTextureAsync(filepath.c_str());
    //g_theApp->m_cubeTexture = newTexture;
    if (newTexture == nullptr)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "COULD NOT FIND TEXTURE: %s", "SETCUBETEXTURE");
        return true;
    }
    else
    {
        profiler_node_t* profileNode = g_profiler->ProfilerGetPreviousFrame("LoadTextureAsync");
        uint64_t totalTime = profileNode->m_endTime - profileNode->m_startTime;

        g_theConsole->ConsolePrintf(Rgba(255, 255, 0, 255), "%LoadTextureAsynic took %s time.", TimeOpCountToString(totalTime));

    }
    return true;
}

//*******************************************************************
bool ConsoleSetIterations(std::string args)
{
    int numIterations = 0;

    std::istringstream iStringStream(args);
    std::string stringToken;
    std::vector<std::string> parts;
    while (std::getline(iStringStream, stringToken, ' '))
    {
        parts.push_back(stringToken);
    }

    if ((int)parts.size() == 0 || (int)parts.size() > 1)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT # OF PARAMS: %s", "SETITERATIONS");
        return true;
    }

    std::string::size_type sz;

    numIterations = std::stoi(parts[0], &sz);

    if (numIterations == 0)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "SETITERATIONS");
        return true;
    }

    g_theSimpleRenderer->m_job->SetIterations((float)numIterations);
    g_theSimpleRenderer->m_context->SetCSConstantBuffer(MANDELBROT_BUFFER_INDEX, g_theSimpleRenderer->m_job->m_mandelbrot_cb);
    return true;
}

//*******************************************************************
bool ConsoleJoinSocket(std::string args)
{
    std::string ipAddress = "";

    std::istringstream iStringStream(args);
    std::string stringToken;
    std::vector<std::string> parts;
    while (std::getline(iStringStream, stringToken, ' '))
    {
        parts.push_back(stringToken);
    }

    if ((int)parts.size() == 0 || (int)parts.size() > 1)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT # OF PARAMS: %s", "SETCUBETEXTURE");
        return true;
    }

    //std::string::size_type sz;
    ipAddress = parts[0];

    if (ipAddress == "")
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "SETCUBETEXTURE");
        return true;
    }

    /*g_theGame->m_socket->Join(ipAddress.c_str());
    g_theGame->m_socket->SetBlocking(false);
    g_theGame->m_socket->Send("Rebecca", 7);*/

    return true;
}

//*******************************************************************
bool ConsoleBlastSockets(std::string args)
{
    UNUSED(args);
    //g_theGame->JoinSockets();
    return true;
}

//*******************************************************************
bool ConsoleGetListenerMessages(std::string args)
{
    UNUSED(args);
    //g_theGame->GetMessageFromListenerSocket();
    return true;
}

//*******************************************************************
bool ConsoleRemoteCommand(std::string args)
{
    RemoteCommandService *rcs = RemoteCommandService::GetInstance();

    char const *commandAndArgs = args.c_str();

    if (args == "")
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "RC");
        return true;
    }

    rcs->SendCommandToOthers(commandAndArgs);
    return true;
}

//*******************************************************************
bool ConsoleRemoteCommandJoin(std::string args)
{
    net_address_t addr = StringToNetAddress(args.c_str());
    RemoteCommandService::GetInstance()->Join(addr);
    return true;
}

//*******************************************************************
bool ConsoleGameSetFieldSize(std::string args)
{
    UNUSED(args);
    /*if (g_theGame->m_gameSession != nullptr)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAVE TO SET THE FIELD SIZE BEFORE HOSTING", "");
        return true;
    }

    int width = 0;
    int height = 0;

    std::istringstream iStringStream(args);
    std::string stringToken;
    std::vector<std::string> parts;
    while (std::getline(iStringStream, stringToken, ' '))
    {
        parts.push_back(stringToken);
    }

    if ((int)parts.size() == 0 || (int)parts.size() != 2)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT # OF PARAMS: %s", "FIELDSIZE");
        return true;
    }

    std::string::size_type sz;

    width = std::stoi(parts[0], &sz);
    height = std::stoi(parts[1], &sz);

    if (width == 0 || height == 0)
    {
        g_theConsole->ConsolePrintf(Rgba(255, 0, 0, 255), "HAS INCORRECT PARAMS: %s", "FIELDSIZE");
        return true;
    }

    g_theGame->m_fieldSize.m_mins = Vector2();
    g_theGame->m_fieldSize.m_maxs = Vector2((float)width, (float)height);*/

    return true;
}

//*******************************************************************
Console::Console()
    : m_drawCursor(false)
    , m_cursorX(5)
    , m_cursorY(0)
{
    std::string clear = "CLEAR";
    RegisterCommand(clear, ConsoleClear);
  
    std::string help = "HELP";
    RegisterCommand(help, ConsoleHelp);
  
    std::string quit = "QUIT";
    RegisterCommand(quit, ConsoleQuit);
  
    std::string fbxListFile = "FBXLISTFILE";
    RegisterCommand(fbxListFile, ConsoleFbxFileList);
   
    std::string basicMemory = "BASIC";
    RegisterCommand(basicMemory, ConsoleBasicMemoryProfiling);
   
    std::string verboseMemory = "VERBOSE";
    RegisterCommand(verboseMemory, ConsoleMemoryProfilerLogLiveAllocations);
   
    std::string threadTest = "THREADTEST";
    RegisterCommand(threadTest, ConsoleThreadTest);
   
    std::string logTest = "LOGTEST";
    RegisterCommand(logTest, ConsoleLogTest);
   
    std::string logFlushTest = "FLUSHTEST";
    RegisterCommand(logFlushTest, ConsoleLogFlushTest);
   
    std::string logTaggedPrintf = "LOGTAGGED";
    RegisterCommand(logTaggedPrintf, ConsoleLogTagged);
    
    std::string logDefault = "LOGDEFAULT";
    RegisterCommand(logDefault, ConsoleLogTagged);
    
    std::string logWarning = "LOGWARNING";
    RegisterCommand(logWarning, ConsoleLogTagged);
    
    std::string logError = "LOGERROR";
    RegisterCommand(logError, ConsoleLogTagged);

    std::string profilerPause = "PROFILERPAUSE";
    RegisterCommand(profilerPause, ConsoleProfilerPause);

    std::string profilerResume = "PROFILERRESUME";
    RegisterCommand(profilerResume, ConsoleProfilerResume);

    std::string profilerSwitchFrameView = "SWITCHFRAMEVIEW";
    RegisterCommand(profilerSwitchFrameView, ConsoleProfilerSwitchFrameView);

    std::string jobGenerateFile = "JOBGENERATEFILE";
    RegisterCommand(jobGenerateFile, ConsoleJobGenerateFile);

    std::string jobLoggerFile = "JOBLOGGERFILE";
    RegisterCommand(jobLoggerFile, ConsoleJobLoggerThread);

    std::string setCubeTexture = "SETCUBETEXTURE";
    RegisterCommand(setCubeTexture, ConsoleSetTextureToCube);

    std::string setIterations = "SETITERATIONS";
    RegisterCommand(setIterations, ConsoleSetIterations);

    std::string joinSocket = "JOINSOCKET";
    RegisterCommand(joinSocket, ConsoleJoinSocket);

    std::string blastSockets = "BLASTSOCKETS";
    RegisterCommand(blastSockets, ConsoleBlastSockets);

    std::string listenToSockets = "LISTENING";
    RegisterCommand(listenToSockets, ConsoleGetListenerMessages);

    std::string remoteCommand = "RC";
    RegisterCommand(remoteCommand, ConsoleRemoteCommand);

    std::string remoteCommandJoin = "RCJ";
    RegisterCommand(remoteCommandJoin, ConsoleRemoteCommandJoin);

    std::string gameSetFieldSize = "FIELDSIZE";
    RegisterCommand(gameSetFieldSize, ConsoleGameSetFieldSize);
    
    /*std::string logDisableTag = "DISABLETAG";
    std::string logEnableTag = "ENABLETAG";
    std::string logDisableAll = "DISABLEALLTAGS";
    std::string logEnableAll = "ENABLEALLTAGS";*/

}

//*******************************************************************
Console::~Console()
{
}

//*******************************************************************
void Console::RegisterCommand(std::string command, ConsoleCommand consoleCommand)
{
    m_consoleCommands[command] = consoleCommand;
}

//*******************************************************************
void Console::UpdateConsole(unsigned char keyCode)
{
    if (g_theApp->m_isKeyDown)
    {
        g_theApp->m_isKeyDown = false;

        if (keyCode == KEY_OEM2)
        {
            keyCode = '/';
        }

        if (keyCode == KEY_PERIOD)
        {
            keyCode = '.';
        }

        if (keyCode == KEY_SHIFT)
        {
            keyCode = KEY_SHIFT;
            return;
        }

        if (keyCode == KEY_QUOTE)
        {
            keyCode = '\'';
            return;
        }

        if (m_prevKeyCode == KEY_SHIFT && keyCode == '5')
        {
            keyCode = '%';
        }

        if (m_prevKeyCode == KEY_SHIFT && keyCode == KEY_QUOTE)
        {
            keyCode = '"';
        }

        if (m_prevKeyCode == KEY_SHIFT && keyCode == KEY_OEM1)
        {
            keyCode = ':';
        }

        if (m_prevKeyCode == KEY_SHIFT && keyCode == KEY_DASH)
        {
            keyCode = '_';
        }

        if (keyCode == KEY_OEM3)
        {
            g_theApp->m_isKeyDown = false;
            g_theApp->m_isDeveloperConsoleOpen = !g_theApp->m_isDeveloperConsoleOpen;
            g_theApp->m_didDeveloperWindowJustOpen = !g_theApp->m_didDeveloperWindowJustOpen;
            return;
        }

        if (keyCode == KEY_F1)
        {
            g_theApp->m_isKeyDown = false;
            //g_theApp->m_isProfilerOpen = !g_theApp->m_isProfilerOpen;
            //g_theApp->m_didProfilerJustOpen = !g_theApp->m_didProfilerJustOpen;
            return;
        }

        if (keyCode == KEY_ESCAPE)
        {
            /*if (g_theApp->m_isProfilerOpen)
            {
                g_theApp->OnExitRequested();
            }*/

            if ((int)m_consoleMessages.size() > 0)
            {
                if (m_cursorX == 5)
                {
                    g_theApp->OnExitRequested();
                }
                else
                {
                    m_consoleMessages.pop_back();
                    m_cursorX = 5;
                }
            }
            else
            {
                g_theApp->OnExitRequested();
            }
            return;
        }

        if (keyCode == KEY_BACK)
        {
            if (m_cursorX == 5)
            {
                if ((int)m_consoleMessages.size() == 1)
                {
                    return;
                }
                m_consoleMessages.pop_back();
                m_cursorY += g_theSimpleRenderer->m_font->m_lineHeight;

                int newCursorX = 5;
                std::string consoleString = m_consoleMessages.back().message;
                for (int stringIndex = 0; stringIndex < (int)consoleString.length(); stringIndex++)
                {
                    int currChar = (int)consoleString.at(stringIndex);
                    int xOffset = g_theSimpleRenderer->m_font->m_glyphInfos.find(currChar)->second.m_xOffset;
                    int width = g_theSimpleRenderer->m_font->m_glyphInfos.find(currChar)->second.m_width;
                    newCursorX = newCursorX + (xOffset + width);
                }
                m_cursorX = newCursorX;
                return;
            }
            std::string& consoleString = m_consoleMessages.back().message;
            char lastChar = consoleString.at(consoleString.length() - 1);
            consoleString.pop_back();

            int xOffset = g_theSimpleRenderer->m_font->m_glyphInfos.find((int)lastChar)->second.m_xOffset;
            int width = g_theSimpleRenderer->m_font->m_glyphInfos.find((int)lastChar)->second.m_width;
            m_cursorX = m_cursorX - (xOffset + width);

            return;
        }

        if (g_theApp->m_didDeveloperWindowJustOpen)
        {
            g_theApp->m_didDeveloperWindowJustOpen = false;

            if (keyCode == KEY_ENTER)
            {
                g_theApp->m_wasEnterPressed = true;

                std::string consoleString(1, '\n');

                std::string commandString = consoleString;
                RunCommand(commandString);

                //g_theApp->m_cursorY -= g_theSimpleRenderer->m_font->m_lineHeight;


                console_message cm;
                cm.message = consoleString;
                cm.color = Rgba::WHITE;
                m_consoleMessages.push_back(cm);

                m_cursorX = 5;

                return;

            }

            std::string consoleString(1, (char)g_theApp->m_keyCode);
            console_message cm;
            cm.message = consoleString;
            cm.color = Rgba::WHITE;
            m_consoleMessages.push_back(cm);

            int xOffset = g_theSimpleRenderer->m_font->m_glyphInfos.find(g_theApp->m_keyCode)->second.m_xOffset;
            int width = g_theSimpleRenderer->m_font->m_glyphInfos.find(g_theApp->m_keyCode)->second.m_width;
            m_cursorX = m_cursorX + xOffset + width;
        }
        else if (keyCode == KEY_ENTER)
        {
            g_theApp->m_wasEnterPressed = true;

            std::string commandString = m_consoleMessages.back().message;
            RunCommand(commandString);

            m_cursorY -= g_theSimpleRenderer->m_font->m_lineHeight;

            std::string consoleString(1, '\n');
            console_message cm;
            cm.message = consoleString;
            cm.color = Rgba::WHITE;
            m_consoleMessages.push_back(cm);

            m_cursorX = 5;

        }
        else
        {
            if (g_theApp->m_wasEnterPressed)
            {
                g_theApp->m_wasEnterPressed = false;
                m_consoleMessages.pop_back();

                std::string consoleString(1, keyCode);

                console_message cm;
                cm.message = consoleString;
                cm.color = Rgba::WHITE;

                m_consoleMessages.push_back(cm);
            }
            else
            {
                std::string& consoleString = m_consoleMessages.back().message;
                std::string keyCodeString(1, keyCode);
                consoleString += keyCodeString;
            }
            int xOffset = g_theSimpleRenderer->m_font->m_glyphInfos.find(g_theApp->m_keyCode)->second.m_xOffset;
            int width = g_theSimpleRenderer->m_font->m_glyphInfos.find(g_theApp->m_keyCode)->second.m_width;
            m_cursorX = m_cursorX + xOffset + width;
        }
    }

    m_prevKeyCode = keyCode;
}

//*******************************************************************
void Console::Clear(std::string args)
{
    UNUSED(args);
    m_consoleMessages.clear();
    m_cursorY = (int)ORHTO_HEIGHT;
}

//*******************************************************************
void Console::Help(std::string args)
{
    UNUSED(args);

    typedef std::map<std::string, ConsoleCommand>::iterator configIterator;
    for (configIterator iterator = g_theConsole->m_consoleCommands.begin(); iterator != g_theConsole->m_consoleCommands.end(); iterator++)
    {
        std::string command = iterator->first;
        std::string message = command;

        console_message cm;
        cm.message = message;
        cm.color = Rgba(255, 255, 0, 255);

        m_consoleMessages.push_back(cm);
        m_cursorY -= g_theSimpleRenderer->m_font->m_lineHeight;
    }
}

//*******************************************************************
void Console::BasicMemoryProfiling(std::string args)
{
    UNUSED(args);

    unsigned int bytesAlloc = MemoryProfilerGetTotalByteCount();
    std::string bytesAllocationed = Stringf("Bytes allocated: %i", bytesAlloc);
    console_message cm;
    cm.message = bytesAllocationed;
    cm.color = Rgba(0, 255, 255, 255);
    cm.message = bytesAllocationed;
    m_consoleMessages.push_back(cm);
    m_cursorY -= g_theSimpleRenderer->m_font->m_lineHeight;

    unsigned int totAlloc = MemoryProfilerGetAllocationCount();
    std::string totalAllocations = Stringf("Total allocation: %i", totAlloc);
    cm.message = totalAllocations;
    m_consoleMessages.push_back(cm);
    m_cursorY -= g_theSimpleRenderer->m_font->m_lineHeight;

    unsigned int highwater = MemoryProfilerHighwaterByteCount();
    std::string highwaterAllocation = Stringf("Highwater: %i", highwater);
    cm.message = highwaterAllocation;
    m_consoleMessages.push_back(cm);
    m_cursorY -= g_theSimpleRenderer->m_font->m_lineHeight;

    unsigned int allocsLast = MemoryProfilerGetAllocationsLastFrame();
    std::string allocationsLastFrame = Stringf("Allocations last frame: %i", allocsLast);
    cm.message = allocationsLastFrame;
    m_consoleMessages.push_back(cm);
    m_cursorY -= g_theSimpleRenderer->m_font->m_lineHeight;


    unsigned int freesLast = MemoryProfilerGetFreesLastFrame();
    std::string freesLastFrame = Stringf("Frees last frame: %i", freesLast);
    cm.message = freesLastFrame;
    m_consoleMessages.push_back(cm);
    m_cursorY -= g_theSimpleRenderer->m_font->m_lineHeight;
}

//*******************************************************************
void Console::RunCommand(std::string command)
{
    std::istringstream iStringStream(command);
    std::string stringToken;
    std::vector<std::string> parts;
    while (std::getline(iStringStream, stringToken, ' '))
    {
        parts.push_back(stringToken);
    }

    bool wasCommandFound = FindCommand(parts[0]);
    if (wasCommandFound)
    {
        ConsoleCommand consoleCommand = m_consoleCommands[parts[0]];
        if ((int)parts.size() < 2)
        {
            consoleCommand("test string");
        }
        else
        {
            std::string args = "";
            int argsCount = 1;
            while (argsCount < ((int)parts.size()))
            {
                if(argsCount == 1)
                    args = args + parts[argsCount];
                else
                    args = args + " " + parts[argsCount];
                argsCount++;
            }
            consoleCommand(args);
        }
    }
    else
    {
        ConsolePrintf(Rgba(255, 0, 0, 255), "Failed to find commands: %s,", parts[0]);
    }
}

//*******************************************************************
bool Console::FindCommand(std::string command)
{
    if (m_consoleCommands.find(command) != m_consoleCommands.end())
    {
        return true;
    }
    return false;
}

//*******************************************************************
void Console::ConsolePrintf(Rgba color, std::string description, std::string command)
{
    std::istringstream iStringStream(description);
    std::string stringToken;
    std::vector<std::string> parts;
    while (std::getline(iStringStream, stringToken, '%'))
    {
        parts.push_back(stringToken);
    }

    std::string message = parts[0] + command;
    if ((int)parts.size() > 1)
        message = message + parts[1];

    console_message cm;
    cm.message = message;
    cm.color = color;

    m_consoleMessages.push_back(cm);
    m_cursorY -= g_theSimpleRenderer->m_font->m_lineHeight;
}

//*******************************************************************
void Console::AddConsoleMessage(std::string consoleString)
{
    std::string commandString = consoleString;

    console_message cm;
    cm.message = consoleString;
    cm.color = Rgba::WHITE;
    m_consoleMessages.push_back(cm);
}

//*******************************************************************
void Console::AddLogMessage(char const *tag, char const *format, va_list varargs)
{
    const int MESSAGE_MAX_LENGTH = 2048;
    char messageLiteral[MESSAGE_MAX_LENGTH];
    vsnprintf_s(messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, format, varargs);
    va_end(varargs);
    messageLiteral[MESSAGE_MAX_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

    std::string commandString = messageLiteral;

    console_message cm;
    cm.message = messageLiteral;

    typedef std::map<const char*, Rgba> TagColorContainer;
    typedef TagColorContainer::iterator TagColorIterator;
    TagColorIterator tagAndColor = m_tagsAndColors.find(tag);
    if (tagAndColor != m_tagsAndColors.end())
    {
        cm.color = m_tagsAndColors[tag];
    }
    else
    {
        cm.color = Rgba::WHITE;
    }

    m_consoleMessages.push_back(cm);
}

//*******************************************************************
void Console::DevConsoleSetTagColor(const char* tag, Rgba color)
{
    m_tagsAndColors[tag] = color;
}

//*******************************************************************
void Console::RenderConsole() const
{
    int currentY = (int)ORHTO_HEIGHT;
    for (int consoleStringIndex = 0; consoleStringIndex < (int)m_consoleMessages.size(); consoleStringIndex++)
    {
        currentY -= g_theSimpleRenderer->m_font->m_lineHeight;
        g_theSimpleRenderer->DrawText(g_theSimpleRenderer->m_font, 5, currentY, m_consoleMessages[consoleStringIndex].message.c_str(), m_consoleMessages[consoleStringIndex].color);

    }
    if (m_drawCursor)
    {
        g_theSimpleRenderer->DrawText(g_theSimpleRenderer->m_font, m_cursorX, m_cursorY, "|", Rgba::WHITE);
    }
}


