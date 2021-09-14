#pragma once
namespace Hook_CalculateShot
{
	namespace Details
	{
		bool bIsHooked = false;
		uintptr_t pAddress = 0x00;
	}

	float* (*HkCalculateShot)(PVOID, PVOID, PVOID) = nullptr;

	float* MyCalculateShot(PVOID pArg0, PVOID pArg1, PVOID pArg2);

	void Hook();
	void UnHook();
}

float* Hook_CalculateShot::MyCalculateShot(PVOID pArg0, PVOID pArg1, PVOID pArg2)
{
	float* uResult = HkCalculateShot(pArg0, pArg1, pArg2);

	while (uResult && Settings::Menu::AIM_Enabled && Settings::Menu::AIM_Silent)
	{
		if (!Settings::Local::LocalPawn) break;
		if (!Settings::Local::LocalPawn->SceneComponent) break;
		Vector3 vRootPos = Settings::Local::LocalPawn->SceneComponent->RelativeLocation;

		Vector3 vAimPos;
		if (!CheatAimbot::GetAimPos(&vAimPos))
			break;

		auto dx = vAimPos.x - vRootPos.x;
		auto dy = vAimPos.y - vRootPos.y;
		auto dz = vAimPos.z - vRootPos.z;

		if (dx * dx + dy * dy + dz * dz < 125000.0f) 
		{
			uResult[4] = vAimPos.x;
			uResult[5] = vAimPos.y;
			uResult[6] = vAimPos.z;
		}
		else
		{
			vAimPos.z -= 16.0f;
			vRootPos.z += 45.0f;

			auto y = SpoofCall(atan2f, vAimPos.y - vRootPos.y, vAimPos.x - vRootPos.x);
			vRootPos.x += cosf(y + 1.5708f) * 32.0f;
			vRootPos.y += sinf(y + 1.5708f) * 32.0f;

			auto length = SpoofCall(sqrtf, SpoofCall(powf, vAimPos.x - vRootPos.x, 2.0f) + SpoofCall(powf, vAimPos.y - vRootPos.y, 2.0f));
			auto x = -SpoofCall(atan2f,vAimPos.z - vRootPos.z, length);
			y = SpoofCall(atan2f,vAimPos.y - vRootPos.y, vAimPos.x - vRootPos.x);

			x /= 2.0f;
			y /= 2.0f;

			uResult[0] = -(sinf(x) * sinf(y));
			uResult[1] = sinf(x) * cosf(y);
			uResult[2] = cosf(x) * sinf(y);
			uResult[3] = cosf(x) * cosf(y);
		}

		break;
	}

	return uResult;
}

void Hook_CalculateShot::Hook()
{
	if (!Details::pAddress)
	{
		Details::pAddress = MemoryHelper::Pattern::PatternScan(_xor("48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 4C 89 4C 24 20 55 41 54 41 55 41 56 41 57 48 8D 6C 24 D0").c_str());
		if (!Details::pAddress)
		{
			Settings::bNeedUnHook = true;
			SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)_xor("Address Error: CalculateShot").c_str(), (LPCSTR)0, (UINT)0);
			return;
		}
	}

	if (Details::bIsHooked)
		return;

	static bool bIsCreated = false;
	if (!bIsCreated)
		bIsCreated = DiscordHelper::InsertHook(Details::pAddress, (uintptr_t)MyCalculateShot, (uintptr_t)&HkCalculateShot);

	if (bIsCreated)
		Details::bIsHooked = true;
}

void Hook_CalculateShot::UnHook()
{
	if (!Details::bIsHooked)
		return;

	if (DiscordHelper::RemoveHook(Details::pAddress))
		Details::bIsHooked = false;
}