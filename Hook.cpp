#include "pch.h"

#include <Windows.h>
#include <cstring>
#include <cstdint>


void WriteJump(
    uintptr_t address,
    uintptr_t destination
)
{
    DWORD oldProtect;

    VirtualProtect(
        (LPVOID)address,
        8,
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );

    uint8_t patch[8]{};

    // JMP rel32
    patch[0] = 0xE9;

    int32_t relative =
        (int32_t)(destination - address - 5);

    memcpy(
        &patch[1],
        &relative,
        sizeof(relative)
    );

    patch[5] = 0x90;
    patch[6] = 0x90;
    patch[7] = 0x90;


    memcpy(
        (void*)address,
        patch,
        sizeof(patch)
    );


    VirtualProtect(
        (LPVOID)address,
        8,
        oldProtect,
        &oldProtect
    );
}

void WriteAbsoluteJump(
    uintptr_t address,
    uintptr_t destination
)
{
    DWORD oldProtect;

    VirtualProtect(
        (LPVOID)address,
        12,
        PAGE_EXECUTE_READWRITE,
        &oldProtect
    );


    uint8_t code[12]{};

    code[0] = 0x48;
    code[1] = 0xB8;

    *(uintptr_t*)&code[2] = destination;

    code[10] = 0xFF;
    code[11] = 0xE0;


    memcpy(
        (void*)address,
        code,
        sizeof(code)
    );


    VirtualProtect(
        (LPVOID)address,
        12,
        oldProtect,
        &oldProtect
    );
}

uintptr_t AllocateNear(uintptr_t target)
{
    const uintptr_t range = 0x70000000;

    uintptr_t start =
        target - range;

    for (uintptr_t addr = start;
        addr < target + range;
        addr += 0x10000)
    {
        void* result = VirtualAlloc(
            (void*)addr,
            0x1000,
            MEM_COMMIT | MEM_RESERVE,
            PAGE_EXECUTE_READWRITE
        );

        if (result)
        {
            return (uintptr_t)result;
        }
    }

    return 0;
}