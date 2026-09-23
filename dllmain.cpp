#include "pch.h"
#include <Windows.h>
#include <cstdio>

#include "PatternScan.h"
#include "Globals.h"
#include "Hook.h"

extern "C"
{
    void ScoutFlyHook();
    void WaistHook();
}

DWORD WINAPI MainThread(LPVOID)
{
    EnsureDefaultIni();   // ← 元の WritePrivateProfileStringA 羅列をここに集約

    // INI 読み込み
    LoadConfig();

    // 導蟲ケージ
    uintptr_t addrCage = FindPattern(
        "MonsterHunterWorld.exe",
        "F3 0F 10 99 44 01 00 00 F3 0F 10 15"
    );

    // 腰ライト
    uintptr_t addrWaist = FindPattern(
        "MonsterHunterWorld.exe",
        "8B 89 40 01 00 00 F3 0F 10 9B 44 01 00 00 0F B6 C1"
    );

    // 導蟲ケージフック
    if (addrCage)
    {
        g_ScoutFlyHook = addrCage;
        g_ScoutFlyReturn = addrCage + 8;

        uintptr_t tramp = AllocateNear(g_ScoutFlyHook);
        if (tramp)
        {
            WriteAbsoluteJump(tramp, (uintptr_t)ScoutFlyHook);
            WriteJump(g_ScoutFlyHook, tramp);
        }
    }

    // 腰ライトフック
    if (addrWaist)
    {
        g_WaistHook = addrWaist;
        g_WaistReturn = addrWaist + 0xE;

        uintptr_t tramp = AllocateNear(g_WaistHook);
        if (tramp)
        {
            WriteAbsoluteJump(tramp, (uintptr_t)WaistHook);
            WriteJump(g_WaistHook, tramp);
        }
    }

    // 設定されたキーで ON/OFF トグル
    bool keyWasDown = false;
    while (true)
    {
        SHORT state = GetAsyncKeyState(cfg_toggle_key);
        bool isDown = (state & 0x8000) != 0;
        if (isDown && !keyWasDown)
        {
            light_flag ^= 1;
        }
        keyWasDown = isDown;
        Sleep(10);
    }

    return 0;
}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved
)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);

        // DLL自身のパスから ini の絶対パスを生成
        InitIniPath(hModule);

        CreateThread(nullptr, 0, IniReloadThread, nullptr, 0, nullptr);

        CreateThread(
            nullptr,
            0,
            MainThread,
            nullptr,
            0,
            nullptr
        );
    }

    return TRUE;
}