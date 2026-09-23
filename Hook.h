#pragma once

#include <cstdint>

void WriteJump(
    uintptr_t address,
    uintptr_t destination
);

void WriteAbsoluteJump(
    uintptr_t address,
    uintptr_t destination
);

uintptr_t AllocateNear(
    uintptr_t target
);
