@echo off
setlocal

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

cl.exe /LD /O2 /EHsc /std:c++17 /Fe:Gw2RotationCoach.dll RotationCoachAddon.cpp KeybindDetector.cpp RotationEngine.cpp RotationCatalog.cpp MumbleLinkReader.cpp imgui\imgui.cpp imgui\imgui_draw.cpp imgui\imgui_tables.cpp imgui\imgui_widgets.cpp user32.lib iphlpapi.lib ws2_32.lib

if "%~1"=="" goto finish
if exist "%~1" (
    copy /Y Gw2RotationCoach.dll "%~1\Gw2RotationCoach.dll"
)

:finish
endlocal
