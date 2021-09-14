#pragma once

template<class T>
struct TArray
{
	friend class FString;

public:
	inline TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline const T& operator[](int i) const
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}

private:
	T* Data;
	int32_t Count;
	int32_t Max;
};

class FString : private TArray<wchar_t>
{
public:
	inline FString()
	{

	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);
		std::string str(length, '\0');
		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);
		return str;
	}
};

class FName
{
public:
	int32_t nComparisonIndex;
	int32_t nNumber;
};

class FText {
private:
	char _padding_[0x28];
	PWCHAR Name;
	DWORD Length;
public:
	inline char* c_str()
	{
		char sBuff[255];
		SpoofCall(wcstombs, (char*)sBuff, (const wchar_t*)this->Name, sizeof(sBuff));
		return sBuff;
	}
};

class FTransform
{
public:
	Vector4 vRotation;
	Vector4 vPosition;
	Vector4 vScale;
};

class FRotator
{
public:
	FRotator() : fPitch(0), fYaw(0), fRoll(0)
	{

	}

	FRotator(float _pitch, float _yaw, float _roll) : fPitch(_pitch), fYaw(_yaw), fRoll(_roll)
	{

	}

	float fPitch;
	float fYaw;
	float fRoll;

	static FRotator CalcAngle(Vector3 vCameraPos, Vector3 vAimPos)
	{
		Vector3 vecDelta = vCameraPos - vAimPos;
		float hyp = SpoofCall(sqrtf, (vecDelta.x * vecDelta.x + vecDelta.y * vecDelta.y));

		FRotator ViewAngles;
		ViewAngles.fPitch = -SpoofCall(atanf, (vecDelta.z / hyp)) * (180.0f / 3.141592653589793f);
		ViewAngles.fYaw = SpoofCall(atanf, (vecDelta.y / vecDelta.x)) * (180.0f / 3.141592653589793f);
		ViewAngles.fRoll = 0.0f;

		if (vecDelta.x >= 0.0f)
			ViewAngles.fYaw += 180.0f;

		return ViewAngles;
	}

	FRotator SmoothAngles(bool bDoSmooth, float fSmoothing, FRotator vControlRotation)
	{
		FRotator vSmoothed = FRotator(this->fPitch, this->fYaw, this->fRoll);
		if (bDoSmooth)
		{
			vSmoothed.fPitch = this->fPitch - vControlRotation.fPitch;
			vSmoothed.fYaw = this->fYaw - vControlRotation.fYaw;

			vSmoothed = vSmoothed.ClampAngles();

			vSmoothed.fPitch = (vSmoothed.fPitch / fSmoothing) + vControlRotation.fPitch;
			vSmoothed.fYaw = (vSmoothed.fYaw / fSmoothing) + vControlRotation.fYaw;
		}
		return vSmoothed;
	}

	FRotator ClampAngles()
	{
		FRotator angles = FRotator(fPitch, fYaw, fRoll);

		if (angles.fYaw < -180.0f)
			angles.fYaw += 360.0f;

		if (angles.fYaw > 180.0f)
			angles.fYaw -= 360.0f;

		if (angles.fPitch < -74.0f)
			angles.fPitch = -74.0f;

		if (angles.fPitch > 74.0f)
			angles.fPitch = 74.0f;

		return angles;
	}
};

class FMinimalViewInfo
{
public:
	Vector3 Location;
	FRotator Rotation;
	float FOV;
	float OrthoWidth;
	float OrthoNearClipPlane;
	float OrthoFarClipPlane;
	float AspectRatio;
};


class cSceneComponent
{
public:
	union
	{
		DEFINE_MEMBER_N(Vector3, RootComponent, 0x0130);
		DEFINE_MEMBER_N(Vector3, RelativeLocation, 0x011C);
		DEFINE_MEMBER_N(Vector3, RelativeRotation, 0x0128);
		DEFINE_MEMBER_N(Vector3, ComponentVelocity, 0x0140);
	};
};

class cFortPlayerStateAthena
{
public:
	union
	{
		DEFINE_MEMBER_N(int32_t, TeamIndex, 0x0E68);
	};
};

class cStaticMesh
{
public:
	FTransform Bone[150];
};

class cStaticMeshComponent
{
public:
	union
	{
		DEFINE_MEMBER_N(cStaticMesh*, StaticMesh, 0x488);
		DEFINE_MEMBER_N(FTransform, ComponentToWorld, 0x1C0);
	};
};

class cFortItemDefinition
{
public:
	union
	{
		DEFINE_MEMBER_N(BYTE, Tier, 0x0054);
		DEFINE_MEMBER_N(FText*, DisplayName, 0x0070);
	};
};

class cFortHoagieVehicleConfigs
{
public:
	union
	{
		DEFINE_MEMBER_N(FLOAT, StrafeForce, 0x0478);
		DEFINE_MEMBER_N(FLOAT, MaxStrafeSpeedKmh, 0x0480);
		DEFINE_MEMBER_N(FLOAT, LiftForce, 0x048C);
		DEFINE_MEMBER_N(FLOAT, MaxLiftSpeedKmh, 0x0490);
		DEFINE_MEMBER_N(FLOAT, BoostForce, 0x04E8);
		DEFINE_MEMBER_N(FLOAT, BoostMaxSpeedKmh, 0x04EC);
		DEFINE_MEMBER_N(FLOAT, BoostDuration, 0x04F0);
		DEFINE_MEMBER_N(FLOAT, BoostCooldown, 0x04F4);
	};
};

class cFortMeatballVehicleConfigs
{
public:
	union
	{
		DEFINE_MEMBER_N(FLOAT, BoostMinPushForce, 0x0488);
		DEFINE_MEMBER_N(FLOAT, BoostTopSpeedForceMultiplier, 0x048C);
		DEFINE_MEMBER_N(FLOAT, BoostTopSpeedMultiplier, 0x0490);
		DEFINE_MEMBER_N(FLOAT, LandTopSpeedMultiplier, 0x0498);
		DEFINE_MEMBER_N(FLOAT, LandPushForceMultiplier, 0x049C);
		DEFINE_MEMBER_N(FLOAT, BoostSteeringMultiplier, 0x04DC);
		DEFINE_MEMBER_N(FLOAT, LandSteeringMultiplier, 0x04E4);
		DEFINE_MEMBER_N(FLOAT, LandMinSpeedSteeringAngle, 0x04E8);
		DEFINE_MEMBER_N(FLOAT, LandMaxSpeedSteeringAngle, 0x04EC);
	};
};

class cActor
{
public:
	union
	{// TrackedNativeGlowFlags 0x01D8
		DEFINE_MEMBER_N(FLOAT, CustomTimeDilation, 0x0098);
		DEFINE_MEMBER_N(cSceneComponent*, SceneComponent, 0x0130);
		DEFINE_MEMBER_N(cFortPlayerStateAthena*, PlayerState, 0x0240);
		
		DEFINE_MEMBER_N(cStaticMeshComponent*, PlayerMesh, 0x0280);
		DEFINE_MEMBER_N(BYTE, bIsDying, 0x0538);
		DEFINE_MEMBER_N(BYTE, bIsDBNO, 0x0552);
		DEFINE_MEMBER_N(PVOID, test, 0x05A0);
		DEFINE_MEMBER_N(uintptr_t, CurrentWeapon, 0x05A0);
		DEFINE_MEMBER_N(uintptr_t, WeaponData, 0x0360);
		DEFINE_MEMBER_N(uintptr_t, GliderHex, 0x02F2);
		//DEFINE_MEMBER_N(uintptr_t, SpreadReduction, 0x00F8);
		//DEFINE_MEMBER_N(uintptr_t, ReloadTime, 0x00FC);
		//FortProjectileAthena
		DEFINE_MEMBER_N(Vector3, FireLoc, 0x0880);

		//BuildingContainer
		DEFINE_MEMBER_N(BYTE, bAlreadySearched, 0x0C69);

		//FortPickup/PrimaryPickupItemEntry + FortItemEntry/ItemDefinition
		DEFINE_MEMBER_N(cFortItemDefinition*, ItemDefinition, 0x0018);

		//FortHoagieVehicle
		DEFINE_MEMBER_N(cFortHoagieVehicleConfigs*, HoagieVehicleConfigs, 0x0F68);

		//FortMeatballVehicle
		DEFINE_MEMBER_N(cFortMeatballVehicleConfigs*, MeatballVehicleConfigs, 0x1180);
	};

	inline const bool IsSearched() const
	{
		return ((this->bAlreadySearched >> 7) & 1);
	}

	inline const bool IsDBNO() const
	{
		return ((this->bIsDBNO >> 1) & 1);
	}

	inline const bool IsDying() const
	{
		return ((this->bIsDying >> 2) & 1);
	}
};


class cPlayerController
{
public:
	union
	{
		DEFINE_MEMBER_N(cActor*, AcknowledgedPawn, 0x02A0);
		DEFINE_MEMBER_N(cActor*, SpectatorPawn, 0x0548);
	};
};

class cLocalPlayer
{
public:
	union
	{
		DEFINE_MEMBER_N(cPlayerController*, PlayerController, 0x0030);
	};
};

class cGameInstance
{
public:
	union
	{
		DEFINE_MEMBER_N(cLocalPlayer**, LocalPlayers, 0x0038);
	};
};

class cLevel
{
public:
	union
	{
		DEFINE_MEMBER_N(cActor**, Actors, 0x0098);
		DEFINE_MEMBER_N(int32_t, ActorsCount, 0x00A0);
	};
};

class cUWorld
{
public:
	union
	{
		DEFINE_MEMBER_N(cLevel**, Levels, 0x0148);
		DEFINE_MEMBER_N(int32_t, LevelCount, 0x150);
		DEFINE_MEMBER_N(cGameInstance*, GameInstance, 0x0190);
	};
};


class cCurrentClosest
{
public:
	float fClosestAimbot = FLT_MAX;
	cActor* pClosestAimbot = nullptr;

	float fClosestPlayer = FLT_MAX;
	cActor* pClosestPlayer = nullptr;

	float fClosestBoat = FLT_MAX;
	cActor* pClosestBoat = nullptr;

	float fClosestHoagie = FLT_MAX;
	cActor* pClosestHoagie = nullptr;
};


