#define _CRT_SECURE_NO_WARNINGS 1
#include "Heading.h"

unsigned long DllThread()
{
	while (!Settings::BaseOffsets::uWorld || !Settings::BaseOffsets::uObjects || !Settings::BaseOffsets::GetPlayerName || !Settings::BaseOffsets::SetControlRotation || !Settings::BaseOffsets::LaunchCharacter)
	{
		if (!Settings::BaseOffsets::uObjects)
		{
			uintptr_t UObjects = MemoryHelper::Pattern::PatternScan(_xor("48 8B 05 ? ? ? ? 4C 8D 3C CD").c_str());
			if (UObjects != 0)
			{
				UObjects = RELATIVE_ADDR(UObjects, 7);
				if (UObjects != 0)
				{
					// Get Count
					DWORD nTMPObjectsCount = *(DWORD*)(UObjects + 0x14);
					Settings::BaseOffsets::nObjectsCount = nTMPObjectsCount > 0x9000 ? 0x9000 : nTMPObjectsCount;

					// Get Array
					uintptr_t deref_1 =  *(uintptr_t*)UObjects;
					if (!SpoofCall(IsBadReadPtr, (const void*)deref_1, (UINT_PTR)8))
					{
						uintptr_t deref_2 = *(uintptr_t*)deref_1;
						if (!SpoofCall(IsBadReadPtr, (const void*)deref_2, (UINT_PTR)8))
							Settings::BaseOffsets::uObjects = deref_2;
					}
				}
			}
		}

		if (!Settings::BaseOffsets::fnGetBounds)
		{
			Settings::BaseOffsets::fnGetBounds = MemoryHelper::Pattern::PatternScan(_xor("E8 ? ? ? ? 48 8B 4D 77 B3 01").c_str());
			if (Settings::BaseOffsets::fnGetBounds)
				Settings::BaseOffsets::fnGetBounds = RELATIVE_ADDR(Settings::BaseOffsets::fnGetBounds, 7);
			Settings::BaseOffsets::fnGetBounds + *(DWORD*)(+0x1) + 0x5;
		}

		if (!Settings::BaseOffsets::uWorld)
		{
			Settings::BaseOffsets::uWorld = MemoryHelper::Pattern::PatternScan(_xor("48 8b 0d ? ? ? ? 48 85 c9 74 30 e8 ? ? ? ? 48 8b f8").c_str());
			if (Settings::BaseOffsets::uWorld)
				Settings::BaseOffsets::uWorld = RELATIVE_ADDR(Settings::BaseOffsets::uWorld, 7);
		}

		if (!Settings::BaseOffsets::GetPlayerName)
			Settings::BaseOffsets::GetPlayerName = EngineHelper::UObjects::FindObject(_xor("GetPlayerName").c_str());

		if (!Settings::BaseOffsets::InvisibleCharacter)
			Settings::BaseOffsets::InvisibleCharacter = EngineHelper::UObjects::FindObject(_xor("SetActorHiddenInGame").c_str());

		if (!Settings::BaseOffsets::SetControlRotation)
			Settings::BaseOffsets::SetControlRotation = EngineHelper::UObjects::FindObject(_xor("SetControlRotation").c_str());

		if (!Settings::BaseOffsets::LaunchCharacter)
			Settings::BaseOffsets::LaunchCharacter = EngineHelper::UObjects::FindObject(_xor("LaunchCharacter").c_str());

		if (!Settings::BaseOffsets::Test)
			Settings::BaseOffsets::Test = EngineHelper::UObjects::FindObject(_xor("GetControlRotation").c_str());

		if (!Settings::BaseOffsets::teleport)
			Settings::BaseOffsets::teleport = EngineHelper::UObjects::FindObject(_xor("K2_TeleportTo").c_str());

		if (!Settings::BaseOffsets::Test2)
			Settings::BaseOffsets::Test2 = EngineHelper::UObjects::FindObject(_xor("CanBeDamaged").c_str());

		if (!Settings::BaseOffsets::Collision)
			Settings::BaseOffsets::Collision = EngineHelper::UObjects::FindObject(_xor("K2_SetActorLocation").c_str());

	}

	Renderer::HookDiscordScene();

	return 0;
}

bool DllMain(HMODULE module_instance, DWORD call_reason, void*)
{
	if (call_reason == DLL_PROCESS_ATTACH) {
		DllThread();
	}

	return TRUE;
}


/*
extern "C" __declspec(dllexport)
LRESULT wnd_hk(int32_t pCode, WPARAM wParam, LPARAM lParam)
{
	static auto bIsInjected = false;
	const auto pmsg = reinterpret_cast<MSG*>(lParam);

	if (!bIsInjected && pmsg->message == 0x5b0)
	{
		UnhookWindowsHookEx(reinterpret_cast<HHOOK>(lParam));
		if (const auto handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&DllThread, nullptr, 0, nullptr))
		{
			if (handle != nullptr)
			{
				CloseHandle(handle);
				bIsInjected = true;
			}
		}
	}

	return CallNextHookEx(nullptr, pCode, wParam, lParam);
}

*/