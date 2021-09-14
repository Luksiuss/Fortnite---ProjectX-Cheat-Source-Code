#pragma once
namespace EngineHelper
{


    namespace Toggle
    {
        Vector4 TierToColor(BYTE bTier);
    }

    namespace Check
    {
        bool CheckClosest(cActor* pActor, float& fClosest);
        bool CheckClosestPlayer(cActor* pActor, int32_t nBone, float& fClosest);
        bool CheckIsInFov(Vector2 vScreenPos, float fClosest);
    }

    namespace Instance
    {
        cUWorld* UWorld;
        cPlayerController* LocalController;
    }

    namespace Internal
    {
        bool LineOfSightTo(cActor* pActor);
        bool WorldToScreen(Vector3 vWorldPos, Vector3* vScreenPos);

        // Bone
        bool GetBoneWorld(cActor* pActor, int nBone, Vector3* vBonePos);
        bool GetBoneScreen(cActor* pActor, int nBone, Vector3* vBonePos);
        bool GetRootPosition(cActor* pActor, Vector3* vRootPos);
    }

    namespace UObjects
    {
        uintptr_t FindObject(char* sFunctionName);
        uintptr_t FindOffset(char* sClassName, char* sVarName);

        void  FixName(char* Name);
        void FreeObjName(uintptr_t pAddress);

        char* GetObjectName(uintptr_t pObject, bool bSkipFix = false);
        char* GetNameByIndex(int32_t nIndex, bool bSkipFix = false);
  
    }
}

class UClass {
public:
    BYTE _padding_0[0x40];
    UClass* SuperClass;
};


class UObject {
public:
    PVOID VTableObject;
    DWORD ObjectFlags;
    DWORD InternalIndex;
    UClass* Class;
    BYTE _padding_0[0x8];
    UObject* Outer;

    inline BOOLEAN IsA(PVOID parentClass) {
        for (auto super = this->Class; super; super = super->SuperClass) {
            if (super == parentClass) {
                return TRUE;
            }
        }

        return FALSE;
    }
};





// Toggle
Vector4 EngineHelper::Toggle::TierToColor(BYTE bTier)
{
    switch (bTier)
    {
    default:
    case 0:
    case 1:
        return Vector4(0.470, 0.462, 0.450, 1); // Gray
        break;
    case 2:
        return Vector4(0.215, 0.874, 0.149, 1); // Green
        break;
    case 3:
        return Vector4(0.149, 0.396, 0.874, 1); // Blue
        break;
    case 4:
        return Vector4(0.662, 0.333, 0.905, 1); // Purple
        break;
    case 5:
    case 6:
    case 7:
    case 8:
        return Vector4(0.9, 0.843, 0, 1); // Gold
        break;
    }
}


// Check
bool EngineHelper::Check::CheckClosest(cActor* pActor, float& fClosest)
{
    if (!pActor) return false;
    if (!pActor->SceneComponent) return false;

    if (pActor->PlayerState)
        if (Settings::Local::LocalTeam == pActor->PlayerState->TeamIndex)
            return false;

    float fDistance = Settings::Local::Camera.Location.Distance(pActor->SceneComponent->RelativeLocation);
    if (fDistance < fClosest)
    {
        fClosest = fDistance;
        return true;
    }

    return false;
}

bool EngineHelper::Check::CheckClosestPlayer(cActor* pActor, int32_t nBone, float& fClosest)
{
    if (!pActor) return false;
    if (!pActor->SceneComponent) return false;
    if (!pActor->PlayerState) return false;
    if (!pActor->PlayerMesh) return false;
    if (!pActor->PlayerMesh->StaticMesh) return false;

    if (!Settings::Menu::AIM_OnTeam && Settings::Local::LocalTeam == pActor->PlayerState->TeamIndex)
        return false;

    if (Settings::Menu::AIM_CheckKO && pActor->IsDBNO())
        return false;

    if (Settings::Menu::AIM_CheckVisible && !EngineHelper::Internal::LineOfSightTo(pActor))
        return false;

    if (Settings::Menu::AIM_UseFoV)
    {
        Vector3 vScreenPos;
        if (!EngineHelper::Internal::GetBoneScreen(pActor, nBone, &vScreenPos))
            return false;

        float fDistance = Vector2(Settings::Sceen::ScreenX / 2, Settings::Sceen::ScreenY / 2).Distance(vScreenPos.ToVec2());
        if (CheckIsInFov(vScreenPos.ToVec2(), fClosest) && fDistance < fClosest)
        {
            fClosest = fDistance;
            return true;
        }
    }
    else 
    {
        float fDistance = Settings::Local::Camera.Location.Distance(pActor->SceneComponent->RelativeLocation);
        if (fDistance < fClosest)
        {
            fClosest = fDistance;
            return true;
        }
    }

    return false;
}

bool EngineHelper::Check::CheckIsInFov(Vector2 vScreenPos, float fClosest)
{
    if (Settings::Menu::AIM_FoVType == 1)
    {
        // left
        if (vScreenPos.x < ((Settings::Sceen::ScreenX / 2.0f) - Settings::Menu::AIM_FoV))
            return false;
        // right
        if (vScreenPos.x > ((Settings::Sceen::ScreenX / 2.0f) + Settings::Menu::AIM_FoV))
            return false;
        // up
        if (vScreenPos.y < ((Settings::Sceen::ScreenY / 2.0f) - Settings::Menu::AIM_FoV))
            return false;
        // down
        if (vScreenPos.y > ((Settings::Sceen::ScreenY / 2.0f) + Settings::Menu::AIM_FoV))
            return false;
    }
    else
    {
        if (vScreenPos.Distance(Vector2(Settings::Sceen::ScreenX / 2.0f, Settings::Sceen::ScreenY / 2.0f)) > fClosest)
            return false;
    }

    return true;
}


// Internal
bool EngineHelper::Internal::LineOfSightTo(cActor* pActor)
{
    if (!Instance::LocalController) return false;

    static uintptr_t pLineOfSightAddress = 0;
    if (!pLineOfSightAddress)
    {
        pLineOfSightAddress = MemoryHelper::Pattern::PatternScan(_xor("40 55 53 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 E0 49").c_str());
        if (!pLineOfSightAddress)
        {
            Settings::bNeedUnHook = true;
            SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)_xor("Address Error: LineOfSightTo").c_str(), (LPCSTR)0, (UINT)0);
            return false;
        }
    }

    auto LineOfSight = reinterpret_cast<bool(__fastcall*)(uintptr_t pPlayerController, uintptr_t pActor, Vector3 * vViewPoint)>(pLineOfSightAddress);

    Vector3 vViewport = Vector3(0, 0, 0);
    return SpoofCall(LineOfSight, (uintptr_t)Instance::LocalController, (uintptr_t)pActor, &vViewport);
}

bool EngineHelper::Internal::WorldToScreen(Vector3 vWorldPos, Vector3* vScreenPos)
{
    if (!Instance::LocalController) return false;

    static uintptr_t pW2SAddress = 0;
    if (!pW2SAddress)
    {
        pW2SAddress = MemoryHelper::Pattern::PatternScan(_xor("48 83 EC 28 E8 ? ? ? ? 48 83 C4 28 C3 CC 0C E9").c_str());
        if (!pW2SAddress)
        {
            Settings::bNeedUnHook = true;
            SpoofCall(MessageBoxA,(HWND)0, (LPCSTR)_xor("Address Error: ProjectWorldToScreen").c_str(), (LPCSTR)0, (UINT)0);
            return false;
        }
    }

    auto WorldToScreen = reinterpret_cast<bool(__fastcall*)(uintptr_t pPlayerController, Vector3 vWorldPos, Vector3 * vScreenPosOut, char)>(pW2SAddress);
    
    return SpoofCall(WorldToScreen, (uintptr_t)Instance::LocalController, vWorldPos, vScreenPos, (char)0);
}

bool EngineHelper::Internal::GetBoneWorld(cActor* pActor, int nBone, Vector3* vBonePos)
{
    if (!pActor) return false;
    if (!pActor->PlayerMesh) return false;

    static uintptr_t pGetBoneMatrix = 0;
    if (!pGetBoneMatrix)
    {
        pGetBoneMatrix = MemoryHelper::Pattern::PatternScan(_xor("48 8B C4 55 53 56 57 41 54 41 56 41 57 48 8D 68 A1 48 81 EC ? ? ? ? 0F 29 78 B8 33 F6 44 0F 29 40 ? 4C 8B E2").c_str());
        if (!pGetBoneMatrix)
        {
            SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)_xor("Address Error: GetBoneMatrix").c_str(), (LPCSTR)0, (UINT)0);
            SpoofCall(ExitProcess, (UINT)0);
            return false;
        }
    }

    auto GetBoneMatrix = reinterpret_cast<Matrix4 * (__fastcall*)(uintptr_t, Matrix4*, int)>(pGetBoneMatrix);

    Matrix4 BoneMatrix;
    if (!SpoofCall(GetBoneMatrix, (uintptr_t)pActor->PlayerMesh, &BoneMatrix, nBone)) return false;

    vBonePos->x = BoneMatrix._41;
    vBonePos->y = BoneMatrix._42;
    vBonePos->z = BoneMatrix._43;

    return true;
}

bool EngineHelper::Internal::GetBoneScreen(cActor* pActor, int nBone, Vector3* vBonePos)
{
    if (!pActor) return false;

    Vector3 vWorldPos;
    if (!GetBoneWorld(pActor, nBone, &vWorldPos))
        return false;

    if (!WorldToScreen(vWorldPos, vBonePos))
        return false;

    return true;
}

bool EngineHelper::Internal::GetRootPosition(cActor* pActor, Vector3* vRootPos)
{
    if (!pActor) return false;
    if (!pActor->SceneComponent) return false;
    vRootPos->x = pActor->SceneComponent->RelativeLocation.x;
    vRootPos->y = pActor->SceneComponent->RelativeLocation.y;
    vRootPos->z = pActor->SceneComponent->RelativeLocation.z;
    return true;
}


// Object
uintptr_t EngineHelper::UObjects::FindObject(char* sFunctionName)
{
    for (DWORD i = 0x0; i < Settings::BaseOffsets::nObjectsCount; i++)
    {
        auto CurrentObject = *(uintptr_t*)(Settings::BaseOffsets::uObjects + (i * 0x18));
        if (!CurrentObject)
            return 0;

        if (!(*(uintptr_t*)(CurrentObject + 0x50)) || *(DWORD*)(CurrentObject + 0x54) == 0xFFFFFFFF)
            continue;

        char* CurObjectName = GetObjectName((uintptr_t)CurrentObject);
        if (!strcmp(CurObjectName, sFunctionName))
            return CurrentObject;
    }
    return 0;
}

uintptr_t EngineHelper::UObjects::FindOffset(char* sClassName, char* sVarName)
{
    for (DWORD i = 0x0; i < Settings::BaseOffsets::nObjectsCount; i++)
    {
        auto CurrentObject = *(uintptr_t*)(Settings::BaseOffsets::uObjects + (i * 0x18));
        if (!CurrentObject)
            return NULL;

        if (!(*(uintptr_t*)(CurrentObject + 0x50)) || *(DWORD*)(CurrentObject + 0x54) == 0xFFFFFFFF)
            continue;

        char* CurObjectName = GetObjectName(CurrentObject);
        if (!strcmp(CurObjectName, sClassName)) //Same class
        {
            for (auto Property = *(uint64_t*)(CurrentObject + 0x50); !SpoofCall(IsBadReadPtr, (const void*)Property, (UINT_PTR)8) && Property; Property = *(uint64_t*)(Property + 0x20))
            {
                auto Type = *(uint64_t*)(Property + 0x8);
                if (!SpoofCall(IsBadReadPtr, (const void*)Type, (UINT_PTR)8) && Type)
                {
                    auto Property_FName = *(FName*)(Property + 0x28);
                    auto Offset = *(DWORD*)(Property + 0x4C);
                    if (Offset != 0)
                    {
                        auto Property_idx = Property_FName.nComparisonIndex;
                        if (Property_idx)
                        {
                            char* PropertyName = GetNameByIndex(Property_idx);
                            if (!strcmp(PropertyName, sVarName))
                            {
                                return Offset;
                            }
                        }
                    }
                }
            }
            return NULL;
        }
    }
}

void EngineHelper::UObjects::FixName(char* Name)
{
    for (int i = 0; Name[i] != '\0'; i++)
    {
        if (Name[i] == '_')
        {
            if (Name[i + 1] == '0' ||
                Name[i + 1] == '1' ||
                Name[i + 1] == '2' ||
                Name[i + 1] == '3' ||
                Name[i + 1] == '4' ||
                Name[i + 1] == '5' ||
                Name[i + 1] == '6' ||
                Name[i + 1] == '7' ||
                Name[i + 1] == '8' ||
                Name[i + 1] == '9')
                Name[i] = '\0';
            break;
        }
    }
    return;
}

void EngineHelper::UObjects::FreeObjName(uintptr_t pAddress)
{
    if (!pAddress)
        return;

    static uintptr_t pFreeObjNameAddress = 0;
    if (!pFreeObjNameAddress)
    {
        pFreeObjNameAddress = MemoryHelper::Pattern::PatternScan(_xor("48 85 C9 74 2E 53").c_str()) + 0x6;
        if (!pFreeObjNameAddress)
        {
            Settings::bNeedUnHook = true;
            SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)_xor("Address Error: FreeObjName").c_str(), (LPCSTR)0, (UINT)0);
            return;
        }
    }

    auto func = reinterpret_cast<__int64(__fastcall*)(__int64)>(pFreeObjNameAddress);

    SpoofCall(func, (__int64)pAddress);
}

char* EngineHelper::UObjects::GetObjectName(uintptr_t pObject, bool bSkipFix)
{
    if (!pObject)
        return _xor("").c_str();

    static uintptr_t pGetObjectNameAddress = 0;
    if (!pGetObjectNameAddress)
    {
        pGetObjectNameAddress = MemoryHelper::Pattern::PatternScan(_xor("40 53 48 83 EC 20 48 8B D9 48 85 D2 75 45 33 C0 48 89 01 48 89 41 08 8D 50 05 E8 ? ? ? ? 8B 53 08 8D 42 05 89 43 08 3B 43 0C 7E 08 48 8B CB E8 ? ? ? ? 48 8B 0B 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B C3 48 83 C4 20 5B C3 48 8B 42 18").c_str());
        if (!pGetObjectNameAddress)
        {
            Settings::bNeedUnHook = true;
            SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)_xor("Address Error: GetObjName").c_str(), (LPCSTR)0, (UINT)0);
            return _xor("").c_str();
        }
    }

    auto fGetObjName = reinterpret_cast<FString * (__fastcall*)(FString*, uintptr_t)>(pGetObjectNameAddress);

    FString Buffer;
    SpoofCall(fGetObjName, (FString*)&Buffer, pObject);

    if (!Buffer.c_str())
        return _xor("").c_str();

    char pResult[1024];
    wcstombs(pResult, Buffer.c_str(), sizeof(pResult));

    FreeObjName((uintptr_t)Buffer.c_str());

    if(!bSkipFix)
        FixName(pResult);

    return pResult;
}

char* EngineHelper::UObjects::GetNameByIndex(int32_t nIndex, bool bSkipFix)
{
    static uintptr_t pGetNameByIndexAddress = 0;
    if (!pGetNameByIndexAddress)
    {
        pGetNameByIndexAddress = MemoryHelper::Pattern::PatternScan(_xor("48 89 5C 24 ? 55 56 57 48 8B EC 48 83 EC 30 8B").c_str());
        if (!pGetNameByIndexAddress)
        {
            Settings::bNeedUnHook = true;
            SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)_xor("Address Error: GetNameByIndex").c_str(), (LPCSTR)0, (UINT)0);
            return _xor("").c_str();
        }
    }

    auto fGetNameByIdx = reinterpret_cast<FString * (__fastcall*)(int*, FString*)>(pGetNameByIndexAddress);

    FString Buffer;
    SpoofCall(fGetNameByIdx, &nIndex, &Buffer);

    if (!Buffer.c_str())
        return _xor("").c_str();

    char pResult[1024];
    wcstombs(pResult, Buffer.c_str(), sizeof(pResult));

    FreeObjName((uintptr_t)Buffer.c_str());

    if (!bSkipFix)
        FixName(pResult);

    return pResult;

}