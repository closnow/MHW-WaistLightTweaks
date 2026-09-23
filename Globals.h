#pragma once

#include <Windows.h>
#include <cstdint>

extern "C"
{
    extern uintptr_t g_ScoutFlyHook;
    extern uintptr_t g_ScoutFlyReturn;

    extern uintptr_t g_WaistHook;
    extern uintptr_t g_WaistReturn;

    extern uint32_t light_flag;

    extern float cfg_guide_brightness;

    extern float cfg_waist_cage_brightness;
    extern float cfg_waist_cage_range;

    extern float cfg_waist_light_brightness;
    extern float cfg_waist_light_range;

    extern float cfg_base_cage_brightness;
    extern float cfg_base_cage_range;

    extern float cfg_base_waist_brightness;
    extern float cfg_base_waist_range;

    extern int cfg_toggle_key;

    void LoadConfig();
    void EnsureDefaultIni();
    DWORD WINAPI IniReloadThread(LPVOID);
}

extern char g_IniPath[MAX_PATH];
void InitIniPath(HMODULE hModule);

uintptr_t FindPattern(const char* moduleName, const char* pattern);