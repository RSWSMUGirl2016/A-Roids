#pragma once
#include <vector>
#include <map>
#include <string>
#include "Engine/Core/Rgba.hpp"
#include <functional>

struct console_message
{
    Rgba color;
    std::string message;
};

typedef bool(*ConsoleCommand)(std::string args);

class Console
{
public:
    Console();
    ~Console();

    void RunCommand(std::string command);
    bool FindCommand(std::string command);
    void RegisterCommand(std::string command, ConsoleCommand consoleCommand);

    void UpdateConsole(unsigned char keyCode);

    void Clear(std::string args);
    void Help(std::string args);
    void BasicMemoryProfiling(std::string args);

    void ConsolePrintf(Rgba color, std::string description, std::string command);
    void AddConsoleMessage(std::string consoleString);
    void AddLogMessage(char const *tag, char const *format, va_list varargs);
    void DevConsoleSetTagColor(const char* tag, Rgba color);
    void RenderConsole() const;
    std::vector<std::string> m_commands;
    std::map<std::string, ConsoleCommand> m_consoleCommands;

    std::vector<console_message> m_consoleMessages;
    bool m_drawCursor;
    int m_cursorX;
    int m_cursorY;
    unsigned char m_prevKeyCode;
    std::map<const char*, Rgba> m_tagsAndColors;
};

