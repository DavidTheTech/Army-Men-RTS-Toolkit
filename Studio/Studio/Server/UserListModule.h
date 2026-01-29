#pragma once
#include <string>
#include <vector>

struct User {
    int id;
    std::string name;
};

class UserListModule
{
public:
    void Init();
    std::string Render();
    void AddUser(User usr);
    void LoadUsers();
    
    std::vector<User>& GetAllUsers()
    {
        return users;
    }

private:

    std::vector<User> users;
    int selectedIndex = -1;
};