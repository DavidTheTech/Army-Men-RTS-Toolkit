## Requirements

This project depends on the following libraries:

1. **ImGui**  
	Repository: https://github.com/ocornut/imgui  
	- Get the following files from the ImGui GitHub repository:  
		- imconfig.h  
		- imgui.cpp  
		- imgui.h  
		- imgui_draw.cpp  
		- imgui_impl_dx9.cpp  
		- imgui_impl_dx9.h  
		- imgui_impl_win32.cpp  
		- imgui_impl_win32.h  
		- imgui_internal.h  
		- imgui_tables.cpp  
		- imgui_widgets.cpp  
		- imstb_rectpack.h  
		- imstb_textedit.h  
		- imstb_truetype.h  
	- Place all ImGui files into:  
		`Studio\Studio\Server\ImGui`

2. **MinHook**  
	Repository: https://github.com/TsudaKageyu/minhook  
	- Copy `MinHook.h` into `Includes\MinHook\MinHook.h`.  
	- Compile MinHook and place `libMinHook.x86.lib` into `Libs\libMinHook.x86.lib`.

3. **LuaJIT**  
	Repository: https://github.com/LuaJIT/LuaJIT  
	- Drag and drop the LuaJIT repository into `Includes\LuaJIT\`.  
	- Compile LuaJIT and place `lua51.lib` into `Libs\lua51.lib`.

4. **nlohmann/json**  
	Repository: https://github.com/nlohmann/json  
	- Copy `json.hpp` into `Includes\json.hpp`.

Make sure all library files and headers are placed in the correct folders before compiling the project.

## Discord
[![Discord Developers](https://invite.casperiv.dev?inviteCode=Y3jy4NJJZf))](https://discord.gg/Y3jy4NJJZf)