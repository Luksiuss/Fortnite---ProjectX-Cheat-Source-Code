#pragma once

// Read Data
#define RELATIVE_ADDR(addr, size)       ((uintptr_t)((uintptr_t)(addr) + *(PINT)((uintptr_t)(addr) + ((size) - sizeof(INT))) + (size)))
#define ReadPointer(base, offset)       ( *(uintptr_t*) ((PBYTE)base + offset) )
#define ReadDWORD(base, offset)         ( *(DWORD*)     ((PBYTE)base + offset) )
#define ReadByte(base, offset)          ( *(BYTE*)    (base + offset) )
#define ReadFloat(base, offset)         ( *(FLOAT*)   (base + offset) )
#define ReadInt(base, offset)           ( *(int32_t*) (base + offset) )

// Auto Padding
#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct { unsigned char MAKE_PAD(offset); type name;}