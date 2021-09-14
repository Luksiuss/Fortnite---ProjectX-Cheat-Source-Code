#pragma once

namespace Hook_Kick
{
    namespace Details
    {
        bool bIsHooked = false;
        uintptr_t pAddress = 0x00;
    }

    PVOID(*HkKick)(PVOID, PVOID) = nullptr;

    PVOID MyKick(PVOID pController, PVOID pReason);

    void Hook();
    void UnHook();
}

PVOID Hook_Kick::MyKick(PVOID pController, PVOID pReason)
{
    return 0;
}

void Hook_Kick::Hook()
{
    if (!Details::pAddress)
    {
        Details::pAddress = MemoryHelper::Pattern::PatternScan(_xor("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 81 EC ? ? ? ? 48 8B 81 ? ? ? ? 4C 8B F2 48 8B F9 48 85 C0 74 07 C6 80 ? ? ? ? ? 48 8B 02 48 89 44 24 ? 48 8B 42 08 48 89 44 24 ? 48 85 C0 74 04 F0 FF 40 08").c_str());
        if (!Details::pAddress)
        {
            Settings::bNeedUnHook = true;
            SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)_xor("Address Error: AntiKick").c_str(), (LPCSTR)0, (UINT)0);
            return;
        }
    }

    if (Details::bIsHooked)
        return;

    static bool bIsCreated = false;
    if (!bIsCreated)
        bIsCreated = DiscordHelper::InsertHook(Details::pAddress, (uintptr_t)MyKick, (uintptr_t)&HkKick);

    if (bIsCreated)
        Details::bIsHooked = true;
}

void Hook_Kick::UnHook()
{
    if (!Details::bIsHooked)
        return;

    if (DiscordHelper::RemoveHook(Details::pAddress))
        Details::bIsHooked = false;
}