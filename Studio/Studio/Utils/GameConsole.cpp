#include "GameConsole.h"
#include <iostream>
#include <sstream>

GameConsole::GameConsole() : m_running(false)
{
    AllocateConsole();

    RegisterFunction("setvar", [this](auto& a) { CmdSetVar(a); });
    RegisterFunction("call", [this](auto& a) { CmdCall(a); });
    RegisterFunction("callreg", [this](auto& a) { CmdCallRegistered(a); });
}

GameConsole::~GameConsole()
{
    Stop();
}

void GameConsole::AllocateConsole()
{
    AllocConsole();

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONIN$", "r", stdin);

    SetConsoleTitleA("Game Console");
    std::cout << "[GameConsole] Ready\n";
}

void GameConsole::Start()
{
    if (m_running)
    {
        return;
    }

    m_running = true;
    m_thread = std::thread(&GameConsole::ConsoleThread, this);
    m_thread.detach();
}

void GameConsole::Stop()
{
    m_running = false;
}

void GameConsole::RegisterFunction(const std::string& name, CommandFn fn)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_commands[name] = fn;
}

void GameConsole::ConsoleThread()
{
    while (m_running)
    {
        std::string line;
        std::getline(std::cin, line);

        if (!line.empty())
        {
            ExecuteLine(line);
        }
    }
}

void GameConsole::ExecuteLine(const std::string& line)
{
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string t;

    while (iss >> t)
    {
        tokens.push_back(t);
    }

    if (tokens.empty())
    {
        return;
    }

    std::string cmd = tokens[0];
    tokens.erase(tokens.begin());

    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_commands.find(cmd);
    if (it != m_commands.end())
    {
        try
        {
            it->second(tokens);
        }
        //except (EXCEPTION_EXECUTE_HANDLER)
        catch (const std::exception& e)
        {
            std::cout << "[GameConsole] Exception in command " << e.what() << "\n" << std::endl;
        }
    }
    else
    {
        std::cout << "[GameConsole] Unknown command\n";
    }
}

void GameConsole::CmdSetVar(const std::vector<std::string>& args)
{
    if (args.size() < 3)
    {
        std::cout << "setvar <addr> <type> <value>\n";
        return;
    }

    uintptr_t addr = std::stoul(args[0], nullptr, 16);
    ArgType type = ParseType(args[1]);

    switch (type)
    {
    case ArgType::Int:
        *(int*)addr = std::stoi(args[2]);
        break;

    case ArgType::Float:
        *(float*)addr = std::stof(args[2]);
        break;

    case ArgType::Bool:
        *(bool*)addr = (args[2] == "1" || args[2] == "true");
        break;

    case ArgType::String:
        strcpy_s((char*)addr, 256, args[2].c_str());
        break;
    }

    std::cout << "OK\n";
}

void GameConsole::CmdCall(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        std::cout << "call <addr> <cdecl|stdcall>\n";
        return;
    }

    uintptr_t addr = std::stoul(args[0], nullptr, 16);
    CallConv conv = ParseConv(args[1]);

    if (conv == CallConv::Cdecl)
    {
        using Fn = int(__cdecl*)();
        int ret = ((Fn)addr)();
        std::cout << "Returned: " << ret << "\n";
    }
    else if (conv == CallConv::Stdcall)
    {
        using Fn = int(__stdcall*)();
        int ret = ((Fn)addr)();
        std::cout << "Returned: " << ret << "\n";
    }
    else
    {
        std::cout << "thiscall requires args (extend me)\n";
    }
}

void GameConsole::CmdCallRegistered(const std::vector<std::string>& args)
{
    if (args.empty())
    {
        return;
    }

    auto it = m_commands.find(args[0]);
    if (it == m_commands.end())
    {
        std::cout << "No such function\n";
        return;
    }

    std::vector<std::string> fnArgs(args.begin() + 1, args.end());
    it->second(fnArgs);
}

GameConsole::ArgType GameConsole::ParseType(const std::string& t)
{
    if (t == "int") return ArgType::Int;
    if (t == "float") return ArgType::Float;
    if (t == "bool") return ArgType::Bool;
    return ArgType::String;
}

GameConsole::CallConv GameConsole::ParseConv(const std::string& c)
{
    if (c == "stdcall") return CallConv::Stdcall;
    if (c == "thiscall") return CallConv::Thiscall;
    return CallConv::Cdecl;
}
