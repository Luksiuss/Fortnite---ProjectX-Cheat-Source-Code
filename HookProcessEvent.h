#pragma once
namespace Hook_ProcessEvent
{
	namespace Details
	{
		bool bIsHooked = false;
		uintptr_t pAddress = 0x00;
	}

	PVOID(*HkProcessEvent)(PVOID, PVOID, PVOID, PVOID) = nullptr;

	PVOID MyProcessEvent(PVOID pObject, PVOID pFunction, PVOID pArgs, PVOID pResult);

	void Hook();
	void UnHook();
}

PVOID Hook_ProcessEvent::MyProcessEvent(PVOID pObject, PVOID pFunction, PVOID pArgs, PVOID pResult)
{
	while (pObject && pFunction && Settings::Local::bIsValid && EngineHelper::Instance::LocalController)
	{	

		char* sObject = EngineHelper::UObjects::GetObjectName((uintptr_t)pObject);
		char* sFunction = EngineHelper::UObjects::GetObjectName((uintptr_t)pFunction);



		Vector3 vAimPos;
		if (!CheatAimbot::GetAimPos(&vAimPos))
			break;

		if (vAimPos.IsZero() || Settings::Local::Camera.Location.IsZero())
			break;

		// Aimbot
		if (Settings::Menu::AIM_Enabled && !Settings::Menu::AIM_Silent && CheatAimbot::IsAimbotting() && strstr(sFunction, _xor("Tick").c_str()))
		{
			FRotator vAngles = FRotator::CalcAngle(Settings::Local::Camera.Location, vAimPos)
				.SmoothAngles(Settings::Menu::AIM_UseSmooth, Settings::Menu::AIM_Smooth, Settings::Local::Camera.Rotation)
				.ClampAngles();


			ClientSetRotation(EngineHelper::Instance::LocalController, vAngles);
			SetIngnoreLookInput(EngineHelper::Instance::LocalController);
		}
		/*
		if (Settings::Menu::Colission)
		{
			if (SpoofCall(GetAsyncKeyState, VK_CONTROL))
			{
				(HkProcessEvent(0, (PVOID)Settings::BaseOffsets::Collision, (PVOID)false, 0));

			}
		}
		*/

		// Bullet Teleport
		if (Settings::Menu::Other_BulletTP && Settings::Local::bHasSniper && strstr(sFunction, _xor("OnRep_FireStart").c_str()))
		{
			cActor* pActor = (cActor*)pObject;
			if (pActor && pActor->SceneComponent)
			{
				pActor->FireLoc = vAimPos;
				pActor->SceneComponent->RelativeLocation = vAimPos;
				pActor->SceneComponent->ComponentVelocity = Vector3(0, 0, 0);
			}
		}

		break;
	}

	return HkProcessEvent(pObject, pFunction, pArgs, pResult);
}

void Hook_ProcessEvent::Hook()
{
	if (!Details::pAddress)
	{
		Details::pAddress = MemoryHelper::Pattern::PatternScan(_xor("40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A").c_str());
		if (!Details::pAddress)
		{
			Settings::bNeedUnHook = true;
			SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)_xor("Address Error: ProcessEvent").c_str(), (LPCSTR)0, (UINT)0);
			return;
		}
	}

	if (Details::bIsHooked)
		return;

	static bool bIsCreated = false;
	if (!bIsCreated)
		bIsCreated = DiscordHelper::InsertHook(Details::pAddress, (uintptr_t)MyProcessEvent, (uintptr_t)&HkProcessEvent);

	if (bIsCreated)
		Details::bIsHooked = true;
}

void Hook_ProcessEvent::UnHook()
{
	if (!Details::bIsHooked)
		return;

	if (DiscordHelper::RemoveHook(Details::pAddress))
		Details::bIsHooked = false;
}

