import shutil, subprocess, sys
from pathlib import Path

BASE = Path.cwd()
LIBS = BASE / "Libs"
INCLUDES = BASE / "Includes"
IMGUI = BASE / "Studio" / "Studio" / "Server" / "ImGui"
MINHOOK_INC = INCLUDES / "MinHook"
LUAJIT_INC = INCLUDES / "LuaJIT"
TOBUILD = BASE / "TO_BUILD"
MINHOOK_BUILD = TOBUILD / "MinHook_VC17"
LUAJIT_BUILD = TOBUILD / "LuaJIT_Build"
TEMP = BASE / "temp_deps"

if subprocess.run(["git", "--version"], capture_output=True).returncode != 0:
    print("ERROR: git is not installed or not in PATH.")
    sys.exit(1)

for d in [LIBS, INCLUDES, IMGUI, MINHOOK_INC, LUAJIT_INC, TOBUILD, MINHOOK_BUILD, LUAJIT_BUILD, TEMP]:
    d.mkdir(parents=True, exist_ok=True)

def clone(repo, dst):
    if not dst.exists():
        subprocess.run(["git", "clone", "--depth", "1", repo, str(dst)], check=True)

clone("https://github.com/ocornut/imgui.git", TEMP / "imgui")
clone("https://github.com/TsudaKageyu/minhook.git", TEMP / "minhook")
clone("https://github.com/LuaJIT/LuaJIT.git", TEMP / "luajit")
clone("https://github.com/nlohmann/json.git", TEMP / "json")

IMGUI_FILES = [
    "imconfig.h", "imgui.cpp", "imgui.h", "imgui_draw.cpp",
    "imgui_impl_dx9.cpp", "imgui_impl_dx9.h", "imgui_impl_win32.cpp",
    "imgui_impl_win32.h", "imgui_internal.h", "imgui_tables.cpp",
    "imgui_widgets.cpp", "imstb_rectpack.h", "imstb_textedit.h", "imstb_truetype.h"
]

imgui_root = TEMP / "imgui"
imgui_backends = imgui_root / "backends"

for f in IMGUI_FILES:
    src = imgui_root / f
    if not src.exists():
        src = imgui_backends / f
    if src.exists():
        shutil.copy2(src, IMGUI / f)
    else:
        print(f"Warning: {f} not found")

shutil.copy2(TEMP / "minhook" / "include" / "MinHook.h", MINHOOK_INC / "MinHook.h")

for item in (TEMP / "minhook").iterdir():
    if item.name != ".git":
        dst = MINHOOK_BUILD / item.name
        (shutil.copytree if item.is_dir() else shutil.copy2)(item, dst)

for item in (TEMP / "luajit").iterdir():
    if item.name != ".git":
        dst = LUAJIT_INC / item.name
        (shutil.copytree if item.is_dir() else shutil.copy2)(item, dst)

for item in (TEMP / "luajit").iterdir():
    if item.name != ".git":
        dst = LUAJIT_BUILD / item.name
        (shutil.copytree if item.is_dir() else shutil.copy2)(item, dst)

shutil.copy2(TEMP / "json" / "single_include" / "nlohmann" / "json.hpp", INCLUDES / "json.hpp")

GREEN = '\033[92m'
BOLD = '\033[1m'
RESET = '\033[0m'
print("\n" + "="*60)
print(GREEN + BOLD + "  DEPENDENCIES DOWNLOADED AND SETUP  " + RESET)
print("="*60)
print(GREEN + "MANUAL BUILD STEPS REQUIRED:" + RESET)
print(f"  * MinHook: Open solution -> {MINHOOK_BUILD / 'VC17' / 'MinHook.sln'}\n")
print(f"    Build for x86, then copy libMinHook.lib to {LIBS / 'libMinHook.x86.lib'}\n")
print(f"  * LuaJIT: Open VS command prompt, cd {LUAJIT_BUILD}, run 'msvcbuild.bat'\n")
print(f"    Copy lua51.lib to {LIBS / 'lua51.lib'}\n")
print(GREEN + "Then build your main Studio solution." + RESET)
print("="*60 + "\n")