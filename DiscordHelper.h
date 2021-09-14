#pragma once
namespace DiscordHelper
{
    uintptr_t GetDiscordModuleBase();
    uintptr_t GetSceneAddress();

    short GetAsyncKeyState(const int vKey);

    bool InsertHook(uintptr_t pOriginal, uintptr_t pHookedFunction, uintptr_t pOriginalCall);
    bool RemoveHook(uintptr_t pOriginal);

    bool CreateHook(uintptr_t pOriginal, uintptr_t pHookedFunction, uintptr_t pOriginalCall);
    bool EnableHook(uintptr_t pTarget, bool bIsEnabled);
    bool EnableHookQue();

    std::vector<uintptr_t> pCreatedHooksArray;
}

uintptr_t DiscordHelper::GetDiscordModuleBase()
{
    static uintptr_t discordModuleBase = 0;

    if (!discordModuleBase)
        discordModuleBase = (uintptr_t)GetModuleHandleA(_xor("DiscordHook64.dll").c_str());

    return discordModuleBase;
}

uintptr_t DiscordHelper::GetSceneAddress()
{
    static uintptr_t presentSceneAdress = 0;

    if (!presentSceneAdress)
        presentSceneAdress = MemoryHelper::Pattern::PatternScan(GetDiscordModuleBase(), _xor("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B D9 41 8B F8").c_str());

    return presentSceneAdress;
}

short DiscordHelper::GetAsyncKeyState(const int vKey)
{
    static uintptr_t addrGetAsyncKeyState = NULL;

    if (!addrGetAsyncKeyState)
        addrGetAsyncKeyState = MemoryHelper::Pattern::PatternScan(GetDiscordModuleBase(), _xor("40 53 48 83 EC 20 8B D9 FF 15 ? ? ? ?").c_str());

    if (!addrGetAsyncKeyState)
        return false;

    using GetAsyncKeyState_t = short(__fastcall*)(int);
    auto fnGetAyncKeyState = (GetAsyncKeyState_t)addrGetAsyncKeyState;

    return SpoofCall(fnGetAyncKeyState, vKey);
}

// Wrapper
bool DiscordHelper::InsertHook(uintptr_t pOriginal, uintptr_t pHookedFunction, uintptr_t pOriginalCall)
{
    bool bAlreadyCreated = false;
    for (auto _Hook : pCreatedHooksArray)
    {
        if (_Hook == pOriginal)
        {
            bAlreadyCreated = true;
            break;
        }
    }

    if (!bAlreadyCreated)
        bAlreadyCreated = CreateHook(pOriginal, pHookedFunction, pOriginalCall);

    if (bAlreadyCreated)
        if (EnableHook(pOriginal, true))
            if (EnableHookQue())
                return true;

    return false;
}

bool DiscordHelper::RemoveHook(uintptr_t pOriginal)
{
    bool bAlreadyCreated = false;
    for (auto _Hook : pCreatedHooksArray)
    {
        if (_Hook == pOriginal)
        {
            bAlreadyCreated = true;
            break;
        }
    }

    if (bAlreadyCreated)
        if (EnableHook(pOriginal, false))
            if (EnableHookQue())
                return true;

    return false;
}

// Internal Calls
bool DiscordHelper::CreateHook(uintptr_t pOriginal, uintptr_t pHookedFunction, uintptr_t pOriginalCall)
{
    static uintptr_t addrCreateHook = NULL;

    if (!addrCreateHook)
        addrCreateHook = MemoryHelper::Pattern::PatternScan(GetDiscordModuleBase(), _xor("40 53 55 56 57 41 54 41 56 41 57 48 83 EC 60").c_str());

    if (!addrCreateHook)
        return false;

    using CreateHook_t = uint64_t(__fastcall*)(LPVOID, LPVOID, LPVOID*);
    auto fnCreateHook = (CreateHook_t)addrCreateHook;

    return SpoofCall(fnCreateHook, (void*)pOriginal, (void*)pHookedFunction, (void**)pOriginalCall) == 0 ? true : false;
}

bool DiscordHelper::EnableHook(uintptr_t pTarget, bool bIsEnabled)
{
    static uintptr_t addrEnableHook = NULL;

    if (!addrEnableHook)
        addrEnableHook = MemoryHelper::Pattern::PatternScan(GetDiscordModuleBase(), _xor("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 20 33 F6 8B FA").c_str());

    if (!addrEnableHook)
        return false;

    using EnableHook_t = uint64_t(__fastcall*)(LPVOID, bool);
    auto fnEnableHook = (EnableHook_t)addrEnableHook;

    return SpoofCall(fnEnableHook, (void*)pTarget, bIsEnabled) == 0 ? true : false;
}

bool DiscordHelper::EnableHookQue()
{
    static uintptr_t addrEnableHookQueu = NULL;

    if (!addrEnableHookQueu)
        addrEnableHookQueu = MemoryHelper::Pattern::PatternScan(GetDiscordModuleBase(), _xor("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 57").c_str());

    if (!addrEnableHookQueu)
        return false;

    using EnableHookQueu_t = uint64_t(__stdcall*)(VOID);
    auto fnEnableHookQueu = (EnableHookQueu_t)addrEnableHookQueu;

    return SpoofCall(fnEnableHookQueu) == 0 ? true : false;
}