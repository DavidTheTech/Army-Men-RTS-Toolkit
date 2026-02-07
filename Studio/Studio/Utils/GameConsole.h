#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>

class GameConsole
{
public:
    enum class ArgType
    {
        Int,
        Float,
        Bool,
        String
    };

    enum class CallConv
    {
        Cdecl,
        Stdcall,
        Thiscall
    };

    using CommandFn = std::function<void(const std::vector<std::string>&)>;

    GameConsole();
    ~GameConsole();

    void Start();
    void Stop();

    void RegisterFunction(const std::string& name, CommandFn fn);

private:
    void AllocateConsole();
    void ConsoleThread();
    void ExecuteLine(const std::string& line);

    void CmdSetVar(const std::vector<std::string>& args);
    void CmdCall(const std::vector<std::string>& args);
    void CmdCallRegistered(const std::vector<std::string>& args);

    static ArgType ParseType(const std::string& t);
    static CallConv ParseConv(const std::string& c);

    std::unordered_map<std::string, CommandFn> m_commands;
    std::thread m_thread;
    std::mutex m_mutex;
    bool m_running;
};