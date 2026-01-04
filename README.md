## Requirements

This project depends on the following libraries:

1. **MinHook**  
	Repository: [https://github.com/TsudaKageyu/minhook](https://github.com/TsudaKageyu/minhook)  
	- Copy `MinHook.h` into `Includes/MinHook/MinHook.h`.  
	- Compile MinHook and place the library `libMinHook.x86.lib` into `Libs\libMinHook.x86.lib`.

2. **LuaJIT**  
	Repository: [https://github.com/LuaJIT/LuaJIT](https://github.com/LuaJIT/LuaJIT)  
	- Drag and drop the LuaJIT repository into `Includes\LuaJIT\`.
	- Compile LuaJIT and place `lua51.lib` into `Libs\lua51.lib`.

3. **nlohmann/json**  
	Repository: [https://github.com/nlohmann/json](https://github.com/nlohmann/json)  
	- Copy `json.hpp` into `Includes\json.hpp`.

Make sure all library files and headers are placed in the correct folders before compiling the project.