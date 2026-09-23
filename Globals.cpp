#include "pch.h"
#include <cstdlib>
#include <cstring>
#include <Windows.h>
#include "Globals.h"

extern "C"
{
    uintptr_t g_ScoutFlyHook = 0;
    uintptr_t g_ScoutFlyReturn = 0;

    uintptr_t g_WaistHook = 0;
    uintptr_t g_WaistReturn = 0;

    uint32_t light_flag = 0;

    float cfg_guide_brightness = 60.00236893f;

    float cfg_waist_cage_brightness = 1000.0f;
    float cfg_waist_cage_range = 70.0f;

    float cfg_waist_light_brightness = 1500.0f;
    float cfg_waist_light_range = 3000.0f;

    float cfg_base_cage_brightness = 1.501250267f;
    float cfg_base_cage_range = 70.0f;

    float cfg_base_waist_brightness = 54.61333084f;
    float cfg_base_waist_range = 800.0f;

    int cfg_toggle_key = 0x78;
}

char g_IniPath[MAX_PATH] = { 0 };

void InitIniPath(HMODULE hModule)
{
    char szModulePath[MAX_PATH] = { 0 };
    GetModuleFileNameA(hModule, szModulePath, MAX_PATH);

    char* lastSlash = strrchr(szModulePath, '\\');
    if (lastSlash)
    {
        *(lastSlash + 1) = '\0';
    }

    strcpy_s(g_IniPath, MAX_PATH, szModulePath);
    strcat_s(g_IniPath, MAX_PATH, "WaistLightTweaks.ini");
}

// INIキャッシュを強制クリア（フラッシュ）
static void FlushIniCache()
{
    WritePrivateProfileStringA(NULL, NULL, NULL, g_IniPath);
}

// デフォルト値でiniを書き出す（無ければ作る／壊れてても上書きしたい場合はforce=trueで呼ぶ）
extern "C" void EnsureDefaultIni()
{
    if (GetFileAttributesA(g_IniPath) != INVALID_FILE_ATTRIBUTES)
        return; // すでに存在するなら何もしない

    WritePrivateProfileStringA("Hotkey", "toggle_key", "0x78", g_IniPath);

    WritePrivateProfileStringA("GuideScoutfly", "brightness", "60.00236893", g_IniPath);

    WritePrivateProfileStringA("WaistCage", "brightness", "1000.0", g_IniPath);
    WritePrivateProfileStringA("WaistCage", "range", "70.0", g_IniPath);

    WritePrivateProfileStringA("WaistLight", "brightness", "1500.0", g_IniPath);
    WritePrivateProfileStringA("WaistLight", "range", "3000.0", g_IniPath);

    WritePrivateProfileStringA("BaseCage", "brightness", "1.501250267", g_IniPath);
    WritePrivateProfileStringA("BaseCage", "range", "70.0", g_IniPath);

    WritePrivateProfileStringA("BaseWaist", "brightness", "54.61333084", g_IniPath);
    WritePrivateProfileStringA("BaseWaist", "range", "800.0", g_IniPath);

    FlushIniCache(); // 書き込み直後にキャッシュフラッシュしておく
}

float GetPrivateProfileFloatA(const char* section, const char* key, float def, const char* path)
{
    char buf[64];
    GetPrivateProfileStringA(section, key, "", buf, sizeof(buf), path);
    if (buf[0] == '\0') return def;
    return static_cast<float>(atof(buf));
}

float ReadFloat(const char* section, const char* key, float def)
{
    return GetPrivateProfileFloatA(section, key, def, g_IniPath);
}

int ReadInt(const char* section, const char* key, int def)
{
    char buf[64];
    GetPrivateProfileStringA(section, key, "", buf, sizeof(buf), g_IniPath);
    if (buf[0] == '\0') return def;

    // "0x78" などの16進数形式と 10進数の両方に対応してパース
    return static_cast<int>(strtol(buf, nullptr, 0));
}

extern "C" void LoadConfig()
{
    // 読み込み前にWindowsのINIキャッシュをクリアしてディスクから再読み込みさせる
    FlushIniCache();

    cfg_toggle_key = ReadInt("Hotkey", "toggle_key", 0x78);

    cfg_guide_brightness = ReadFloat("GuideScoutfly", "brightness", 60.00236893f);

    cfg_waist_cage_brightness = ReadFloat("WaistCage", "brightness", 1000.0f);
    cfg_waist_cage_range = ReadFloat("WaistCage", "range", 70.0f);

    cfg_waist_light_brightness = ReadFloat("WaistLight", "brightness", 1500.0f);
    cfg_waist_light_range = ReadFloat("WaistLight", "range", 3000.0f);

    cfg_base_cage_brightness = ReadFloat("BaseCage", "brightness", 1.501250267f);
    cfg_base_cage_range = ReadFloat("BaseCage", "range", 70.0f);

    cfg_base_waist_brightness = ReadFloat("BaseWaist", "brightness", 54.61333084f);
    cfg_base_waist_range = ReadFloat("BaseWaist", "range", 800.0f);
}

extern "C" DWORD WINAPI IniReloadThread(LPVOID)
{
    WIN32_FILE_ATTRIBUTE_DATA data = {};
    FILETIME lastWrite = {};
    bool fileExists = false;

    if (GetFileAttributesExA(g_IniPath, GetFileExInfoStandard, &data))
    {
        lastWrite = data.ftLastWriteTime;
        fileExists = true;
    }

    while (true)
    {
        WIN32_FILE_ATTRIBUTE_DATA now = {};
        bool ok = GetFileAttributesExA(g_IniPath, GetFileExInfoStandard, &now) != 0;

        if (!ok)
        {
            // iniが存在しない（削除された）→デフォルトを再生成
            if (fileExists)
            {
                // 直前まで存在していた＝今回削除されたタイミング
                Sleep(100); // 削除処理の完了待ち（念のため）
                EnsureDefaultIni();

                if (GetFileAttributesExA(g_IniPath, GetFileExInfoStandard, &now))
                {
                    lastWrite = now.ftLastWriteTime;
                    LoadConfig();
                }
                fileExists = true;
            }
            // fileExists が元々falseなら何もしない（起動直後で未生成の一瞬など）
        }
        else
        {
            if (!fileExists || CompareFileTime(&now.ftLastWriteTime, &lastWrite) != 0)
            {
                lastWrite = now.ftLastWriteTime;
                Sleep(100); // 書き込み完了待ち
                LoadConfig();
            }
            fileExists = true;
        }

        Sleep(1000);
    }
    return 0;
}