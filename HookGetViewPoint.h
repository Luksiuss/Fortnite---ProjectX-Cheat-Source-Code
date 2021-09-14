#pragma once
namespace Hook_GetViewPoint
{
	namespace Details
	{
		bool bIsHooked = false;
		uintptr_t pAddress = 0x00;
	}

	INT(*HkGetViewPoint)(uintptr_t*, FMinimalViewInfo*, BYTE) = nullptr;

	INT MyGetViewPointHook(uintptr_t* pPlayer, FMinimalViewInfo* pViewInfo, BYTE bstereoPass);

	void Hook();
	void UnHook();
}

INT Hook_GetViewPoint::MyGetViewPointHook(uintptr_t* pPlayer, FMinimalViewInfo* pViewInfo, BYTE bstereoPass)
{
	INT uResult = HkGetViewPoint(pPlayer, pViewInfo, bstereoPass);

	Settings::Local::Camera = *(FMinimalViewInfo*)pViewInfo;


	if (Settings::Menu::Other_UseCameraFoV)
	{
		const float upperFOV = 50.534008f;
		const float lowerFOV = 40.0f;

		auto fov = pViewInfo->FOV;
		auto desired = (((180.0f - upperFOV) / (180.0f - 80.0f)) * (Settings::Menu::Other_CameraFoV - 80.0f)) + upperFOV;

		if (fov > upperFOV)
			fov = desired;
		else if (fov > lowerFOV)
			fov = (((fov - lowerFOV) / (upperFOV - lowerFOV)) * (desired - lowerFOV)) + lowerFOV;

		pViewInfo->FOV = fov;
	}

	return uResult;
}

void Hook_GetViewPoint::Hook()
{
	if (!Details::pAddress)
	{
		Details::pAddress = MemoryHelper::Pattern::PatternScan(_xor("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B D9 41 8B F0 48 8B 49 30 48 8B FA E8 ? ? ? ? BA ? ? ? ? 48 8B C8").c_str());
		if (!Details::pAddress)
		{
			Settings::bNeedUnHook = true;
			SpoofCall(MessageBoxA, (HWND)0, (LPCSTR)_xor("Address Error: GetViewPoint").c_str(), (LPCSTR)0, (UINT)0);
			return;
		}
	}

	if (Details::bIsHooked)
		return;

	static bool bIsCreated = false;
	if (!bIsCreated)
		bIsCreated = DiscordHelper::InsertHook(Details::pAddress, (uintptr_t)MyGetViewPointHook, (uintptr_t)&HkGetViewPoint);

	if (bIsCreated)
		Details::bIsHooked = true;
}

void Hook_GetViewPoint::UnHook()
{
	if (!Details::bIsHooked)
		return;

	if (DiscordHelper::RemoveHook(Details::pAddress))
		Details::bIsHooked = false;
}