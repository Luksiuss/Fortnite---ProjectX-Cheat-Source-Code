#pragma once

namespace CheatAimbot
{
	namespace Details
	{
		cActor* pAimbotTarget = 0;
	}

	bool GetAimPos(Vector3* vAimPos);

	bool IsAimbotting();
	
	int32_t GetBoneID();
}


bool CheatAimbot::GetAimPos(Vector3* vAimPos)
{
	if (!Details::pAimbotTarget) return false;
	if (!Details::pAimbotTarget->SceneComponent) return false;
	if (!Details::pAimbotTarget->PlayerState) return false;
	if (!Details::pAimbotTarget->PlayerMesh) return false;
	if (!Details::pAimbotTarget->PlayerMesh->StaticMesh) return false;

	if (!EngineHelper::Internal::GetBoneWorld(Details::pAimbotTarget, GetBoneID(), vAimPos))
		return false;

	return true;
}


bool CheatAimbot::IsAimbotting()
{
	if (Settings::Menu::AIM_AimAuto)
		return true;

	switch (Settings::Menu::AIM_Key)
	{
	default:
	case 0:
		return DiscordHelper::GetAsyncKeyState(VK_LBUTTON);
		break;
	case 1:
		return DiscordHelper::GetAsyncKeyState(VK_RBUTTON);
		break;
	case 2:
		return DiscordHelper::GetAsyncKeyState(VK_LSHIFT);
		break;
	case 3:
		return DiscordHelper::GetAsyncKeyState(VK_LCONTROL);
		break;
	case 4:
		return DiscordHelper::GetAsyncKeyState(VK_LMENU);
		break;
	}
}

int32_t CheatAimbot::GetBoneID()
{
	if (Settings::Menu::AIM_ShotgunAlwaysHead && Settings::Local::bHasShotgun)
		return 66;

	if (Settings::Menu::AIM_SniperalwaysHead && Settings::Local::bHasSniper)
		return 66;

	switch (Settings::Menu::AIM_Bone)
	{
	default:
	case 0:
		return 66;
		break;
	case 1:
		return 65;
		break;
	case 2:
		return 5;
		break;
	}
}
void ClientSetRotation(PVOID PlayerController, FRotator angles)
{
	uintptr_t vTable = *(uintptr_t*)PlayerController;

	if (!vTable) return;

	uintptr_t func = *(uintptr_t*)(vTable + 0x628);

	if (!func) return;

	auto ClientSetRotation = reinterpret_cast<void(__fastcall*)(uintptr_t PlayerController, FRotator NewAngle, char bResetCamera)>(func);

	SpoofCall(ClientSetRotation, (uintptr_t)PlayerController, angles, (char)0);
}


void SetIngnoreLookInput(PVOID PlayerController)
{
	if (PlayerController == nullptr) return;

	uintptr_t vTable = *(uintptr_t*)PlayerController;
	if (!vTable) return;

	uintptr_t func = *(uintptr_t*)(vTable + 0x728);
	if (!func) return;

	auto SetIgnoreLookInput = reinterpret_cast<void(__fastcall*)(uintptr_t PlayerController, char bResult)>(func);

	SpoofCall(SetIgnoreLookInput, (uintptr_t)PlayerController, (char)1);
}

void ResetLookInput(PVOID PlayerController)
{
	if (PlayerController == nullptr) return;

	uintptr_t vTable = *(uintptr_t*)PlayerController;
	if (!vTable) return;

	uintptr_t func = *(uintptr_t*)(vTable + 0x730);
	if (!func) return;

	auto ResetLookInput = reinterpret_cast<void(__fastcall*)(uintptr_t PlayerController)>(func);

	SpoofCall(ResetLookInput, (uintptr_t)PlayerController);
}
