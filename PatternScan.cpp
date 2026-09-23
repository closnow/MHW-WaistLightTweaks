#include "pch.h"
#include "PatternScan.h"

#include <Psapi.h>
#include <vector>
#include <string>

#pragma comment(lib, "Psapi.lib")

static bool DataCompare(const BYTE* data, const BYTE* mask, const char* szMask)
{
    for (; *szMask; ++szMask, ++data, ++mask)
    {
        if (*szMask == 'x' && *data != *mask)
            return false;
    }
    return true;
}

static uintptr_t FindPatternInternal(uintptr_t start, size_t size, const BYTE* mask, const char* szMask)
{
    for (size_t i = 0; i < size; i++)
    {
        if (DataCompare((BYTE*)(start + i), mask, szMask))
            return start + i;
    }

    return 0;
}

uintptr_t FindPattern(const char* moduleName, const char* pattern)
{
    HMODULE hModule = GetModuleHandleA(moduleName);
    if (!hModule)
        return 0;

    MODULEINFO mi{};
    GetModuleInformation(GetCurrentProcess(), hModule, &mi, sizeof(mi));

    std::vector<BYTE> bytes;
    std::string mask;

    const char* current = pattern;

    while (*current)
    {
        if (*current == ' ')
        {
            current++;
            continue;
        }

        if (*current == '?')
        {
            bytes.push_back(0);
            mask.push_back('?');

            if (*(current + 1) == '?')
                current++;

            current++;
            continue;
        }

        bytes.push_back((BYTE)strtoul(current, nullptr, 16));
        mask.push_back('x');

        while (*current && *current != ' ')
            current++;
    }

    mask.push_back('\0');

    return FindPatternInternal(
        (uintptr_t)mi.lpBaseOfDll,
        mi.SizeOfImage,
        bytes.data(),
        mask.c_str());
}