#include <windows.h>
#include "UserListModule.h"
#include "ImGui/imgui.h"

void UserListModule::Init()
{
    LoadUsers();
}

void UserListModule::LoadUsers()
{
    //users.clear();
    //Load from memory
}

void UserListModule::AddUser(User usr)
{
    //User newUser;
    //newUser.name = "New User";
    //users.push_back(newUser);
    users.push_back(usr);
}

std::string UserListModule::Render()
{
    std::string clickedUser;

    for (size_t i = 0; i < users.size(); i++)
    {
        ImGui::PushID(users[i].id);

        if (ImGui::Selectable(users[i].name.c_str(), selectedIndex == (int)i))
        {
            selectedIndex = i;
            clickedUser = std::to_string(users[i].id);
        }

        ImGui::PopID();
    }

    return clickedUser;
}