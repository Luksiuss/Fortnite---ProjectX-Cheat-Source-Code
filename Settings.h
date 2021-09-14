#pragma once
namespace Settings
{
	bool bIsDev = true;
	bool bNeedUnHook = false;

	float fAirStuckValue = 0.0000001f;
	float fMaxVehicleDistance = 15.0f;

	namespace Local
	{
		bool bHasShotgun = false;
		bool bHasSniper = false;
		bool bIsValid = false;

		int32_t LocalTeam = 0;
		cActor* LocalPawn = nullptr;
		FMinimalViewInfo Camera;
	}

	namespace BaseOffsets
	{
		PVOID* UWorld = 0;
		uintptr_t fnGetBounds = 0;
		uintptr_t uWorld = 0;
		uintptr_t uObjects = 0;
		DWORD nObjectsCount = 0;

		uintptr_t GetPlayerName;
		uintptr_t SetControlRotation;
		uintptr_t LaunchCharacter;
		uintptr_t InvisibleCharacter;
		uintptr_t Test;
		uintptr_t teleport;
		uintptr_t Test2;
		uintptr_t Collision;
	}

	namespace Sceen
	{
		float ScreenX = 0;
		float ScreenY = 0;
	}

	namespace Menu
	{		
		// Aimbot
		bool AIM_Enabled = true;
		bool AIM_OnTeam = false;
		bool AIM_CheckVisible = true;
		bool AIM_CheckKO = true;


		bool AIM_UseSmooth = false;
		float AIM_Smooth = 0.0f;
		
		bool AIM_UseFoV = true;
		float AIM_FoV = 240.0f;
		int32_t AIM_FoVType = 1;

		bool AIM_AimAuto = false;
		bool AIM_AimLock = true;
		bool AIM_Silent = true;
		bool AIM_ShotgunAlwaysHead = true;
		bool AIM_SniperalwaysHead = true;
 
		int32_t AIM_Bone = 2;
		int32_t AIM_Key = 1;

		// ESP
		bool ESP_Enabled = true;
		bool ThreeDESP = false;
		bool ESP_OnTeam = false;
		bool ESP_CheckVisible = true;
		bool ESP_CheckKO = true;
		bool ESP_Username = false;
		bool ESP_Bones = true;
		bool ESP_UseBox = true;
		bool ESP_FillBox = true;
		bool ESP_Distance = false;

		bool ESP_FillText = true;
		float ESP_FontSize = 12.0f;
		float ESP_Thickness = .5f;


		// Markers
		bool MARK_Llama = false && 10000.0f;
		bool MARK_Shark = false;
		bool Shark = false;
		bool MARK_CampFire = false;
		bool MARK_FlopperSpawn = false;
		bool MARK_XPCoin = false;
		bool MARK_Chest = false;
		bool MARK_AmmoBox = false;
		bool MARK_PickUpAmmo = false;
		bool MARK_PickUpItem = false;

		bool MARK_Boat = false;
		bool MARK_Hoagie = false;

		float MARK_MaxDistance = 95.0f;
		int32_t MARK_MinTier = 3;

		// Exploits
		bool Other_UseCameraFoV = false;
		float Other_CameraFoV = 110.0f;

		bool Other_BulletTP = false;
		bool Other_AirStuck = false;
		bool NoSpread = false;
		bool InstantReload = false;
		bool FirstPerson = false;
		bool Colission = false;
		bool bESP2D = false;
		bool Other_HoagieBooster = false;
		bool Other_HoagieSpeed = false;
		float Other_HoagieSpeedMultiplier = 1.0f;

		bool Other_BoatTeleport = false;
		bool Other_PlayerTP = false;
		bool Other_RapidFire = false;
		bool Radar = false;
		bool Invisible = false;
		bool Other_GliderHex = false;
		bool Other_SpreadReduction = false;
		bool Other_BoatSpeed = false;
		float Other_BoatSpeedMultiplier = 1.0f;

		// Colors
		Vector4 ESPColor = Vector4(0.874, 0.149, 0.192, 1);
		Vector4 ESPVisibleColor = Vector4(0.215, 0.874, 0.149, 1);
		Vector4 ESPBotColor = Vector4(0.149, 0.396, 0.874, 1);
		Vector4 ESPHenchManColor = Vector4(0.238, 0.130, 0.238, 1);
		Vector4 ESPKnockedDownColor = Vector4(0.952, 0.949, 0.125, 1);
		Vector4 MiscColor = Vector4(0.125, 0.925, 0.952, 1);
		Vector4 MarkerColor = Vector4(0.952, 0.580, 0.125, 1);
		Vector4 TextFillColor = Vector4(0, 0, 0, 0.7);
		Vector4 SharkColor = Vector4(0.65, 0.105, 0.225, 1);
		Vector4 LlamaColor = Vector4(0.225, 0.225, 0.225, 1);

	}
}