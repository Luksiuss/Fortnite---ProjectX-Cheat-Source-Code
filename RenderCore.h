#pragma once
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#define PI (3.141592653589793f)
namespace Renderer
{
    namespace typeDefs
    {
        typedef HRESULT(__stdcall* TOPresentReturn)(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags);
        typedef HRESULT(__stdcall* TOResizeReturn) (IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
    }

    namespace Details
    {
        bool bShowMenu = true;
        bool bSceneHooked = false;

        ID3D11Device* pD11Device = nullptr;
        ID3D11DeviceContext* pD11DeviceContext = nullptr;
        ID3D11RenderTargetView* pD11RenderTargetView = nullptr;

        typeDefs::TOPresentReturn OPresentReturn = nullptr;
        typeDefs::TOResizeReturn  OResizeReturn = nullptr;
        WNDPROC oWndReturn = nullptr;     
    }

    bool HookDiscordScene();
    void UnHookDiscordScene();

    void DrawMenu();
    void DrawCanvas();
    void rapidfire();
    HRESULT DiscordHookCallback(IDXGISwapChain* dxSwapChain, const UINT syncInterval, UINT flags);
    HRESULT ResizeCallback(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
    LRESULT WINAPI ImGuiHookCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}

// Hook Handle
bool Renderer::HookDiscordScene()
{
    uintptr_t BaseDiscord = (uintptr_t)SpoofCall(GetModuleHandleA, (LPCSTR)_xor("DiscordHook64.dll").c_str());
    if (!BaseDiscord)
        return false;

    // Present
    const uintptr_t OffsetPresent = 0x2DB338; // 48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B D9 41 8B F8
    Details::OPresentReturn = (typeDefs::TOPresentReturn) (*(uintptr_t*)(BaseDiscord + OffsetPresent));
    *(uintptr_t*)(BaseDiscord + OffsetPresent) = (uintptr_t)DiscordHookCallback;

    // Resize
    const uintptr_t OffsetResize = 0x2DB310; // 48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 30 41 8B D9 41 8B F8
    Details::OResizeReturn = (typeDefs::TOResizeReturn) (*(uintptr_t*)(BaseDiscord + OffsetResize));
    *(uintptr_t*)(BaseDiscord + OffsetResize) = (uintptr_t)ResizeCallback;

    return true;

}

void Renderer::UnHookDiscordScene()
{
    if (!Details::bSceneHooked)
        return;

// just reverse function above.alr
}
void Active()
{
    ImGuiStyle* Style = &ImGui::GetStyle();
    Style->Colors[ImGuiCol_Button] = ImColor(25, 30, 34);
    Style->Colors[ImGuiCol_ButtonActive] = ImColor(25, 30, 34);
    Style->Colors[ImGuiCol_ButtonHovered] = ImColor(25, 30, 34);
}
void Hovered()
{
    ImGuiStyle* Style = &ImGui::GetStyle();
    Style->Colors[ImGuiCol_Button] = ImColor(19, 22, 27);
    Style->Colors[ImGuiCol_ButtonActive] = ImColor(19, 22, 27);
    Style->Colors[ImGuiCol_ButtonHovered] = ImColor(19, 22, 27);
}

// Draw Handle
void Renderer::DrawMenu()
{
    ImGuiStyle* Style = &ImGui::GetStyle();
    Style->WindowRounding = 0;
    Style->WindowBorderSize = 0;
    Style->ChildRounding = 0;
    Style->FrameBorderSize = 0;
    Style->Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0, 0);
    Style->Colors[ImGuiCol_ChildBg] = ImColor(19, 22, 27);
    Style->Colors[ImGuiCol_Button] = ImColor(25, 30, 34);
    Style->Colors[ImGuiCol_ButtonHovered] = ImColor(25, 30, 34);
    Style->Colors[ImGuiCol_ButtonActive] = ImColor(25, 30, 34);
    static auto Name = "ProjectX";
    static auto Flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
    static int Active_Tab = 1;
    static int Active_SubTabRage = 1;
    static int Active_SubTabVisuals = 1;
    static int Active_SubTabMisc = 1;
    static int Active_SubTabSkins = 1;
    static int Active_SubTabConfigs = 1;
    if (ImGui::Begin(Name, NULL, Flags))
    {
        ImGui::BeginChild("##Main", ImVec2{ 640, 430 }, false);
        {
            ImGui::BeginChild("##UP", ImVec2{ 640, 45 }, false);
            {
                ImGui::SetCursorPos(ImVec2{ 240, 0 });
                if (Active_Tab == 1) Active(); else Hovered();
                if (ImGui::Button("Aimbot", ImVec2{ 75, 45 }))
                    Active_Tab = 1;
                ImGui::SetCursorPos(ImVec2{ 320, 0 });
                if (Active_Tab == 2) Active(); else Hovered();
                if (ImGui::Button("Visuals", ImVec2{ 75, 45 }))
                    Active_Tab = 2;
                ImGui::SetCursorPos(ImVec2{ 400, 0 });
                if (Active_Tab == 3) Active(); else Hovered();
                if (ImGui::Button("Misc", ImVec2{ 75, 45 }))
                    Active_Tab = 3;
            }
            ImGui::EndChild();
            ImGui::SetCursorPos(ImVec2{ 0, 45 });
            Style->Colors[ImGuiCol_ChildBg] = ImColor(25, 30, 34);
            Style->Colors[ImGuiCol_Button] = ImColor(25, 30, 34);
            Style->Colors[ImGuiCol_ButtonHovered] = ImColor(25, 30, 34);
            Style->Colors[ImGuiCol_ButtonActive] = ImColor(25, 30, 34);
            ImGui::BeginChild("##Childs", ImVec2{ 640, 365 }, false);
            {
                ImGui::SetCursorPos(ImVec2{ 15, 5 });
                Style->ChildRounding = 0;
                ImGui::BeginChild("##Left", ImVec2{ 155, 320 }, false);
                {
                    if (Active_Tab == 1)
                    {
                        ImGui::SetCursorPosY(10);
                        if (ImGui::Button("Main                              ", ImVec2{ 140, 20 })) Active_SubTabRage = 1;
                        if (ImGui::Button("Config                             ", ImVec2{ 140, 20 })) Active_SubTabRage = 2;
                    }
                    else if (Active_Tab == 2)
                    {
                        ImGui::SetCursorPosY(10);
                        if (ImGui::Button("Player                               ", ImVec2{ 140, 20 })) Active_SubTabVisuals = 1;
                        if (ImGui::Button("World                              ", ImVec2{ 140, 20 })) Active_SubTabVisuals = 2;
                    }
                    else if (Active_Tab == 3)
                    {
                        ImGui::SetCursorPosY(10);
                        if (ImGui::Button("Exploits                               ", ImVec2{ 140, 20 })) Active_SubTabVisuals = 1;
                    }
                }
                ImGui::EndChild();

                ImGui::SetCursorPos(ImVec2{ 175, 5 });
                Style->Colors[ImGuiCol_ChildBg] = ImColor(29, 34, 38);
                Style->ChildRounding = 5;
                ImGui::BeginChild("##SubMain", ImVec2{ 455, 350 }, false);
                {
                    ImGui::SetCursorPos(ImVec2{ 10, 10 });
                    if (Active_Tab == 1 && Active_SubTabRage == 1)
                    {
                        ImGui::Checkbox(_xor("Enable Aimbot").c_str(), &Settings::Menu::AIM_Enabled);
                        ImGui::Checkbox(_xor("Enable On Team").c_str(), &Settings::Menu::AIM_OnTeam);
                        ImGui::Separator();
                        ImGui::Checkbox(_xor("Check Visibility").c_str(), &Settings::Menu::AIM_CheckVisible);
                        ImGui::Checkbox(_xor("Check Knocked Down").c_str(), &Settings::Menu::AIM_CheckKO);
                        ImGui::Separator();
                        ImGui::Checkbox(_xor("###SmoothingAimbotCheckbox").c_str(), &Settings::Menu::AIM_UseSmooth); ImGui::SameLine();
                        ImGui::SliderFloat(_xor("Smoothing").c_str(), &Settings::Menu::AIM_Smooth, 0, 50, _xor("%.0f").c_str());
                        ImGui::Checkbox(_xor("###FoVAimbotCheckBox").c_str(), &Settings::Menu::AIM_UseFoV); ImGui::SameLine();
                        ImGui::SliderFloat(_xor("FoV").c_str(), &Settings::Menu::AIM_FoV, 40, (Settings::Sceen::ScreenY / 2.0f), _xor("%.0f").c_str());
                        if (Settings::Menu::AIM_UseFoV)
                        {
                            ImGui::RadioButton(_xor("Circle FoV").c_str(), (int*)&Settings::Menu::AIM_FoVType, 0);
                            ImGui::RadioButton(_xor("Square FoV").c_str(), (int*)&Settings::Menu::AIM_FoVType, 1);
                        }
                        ImGui::EndTabItem();
                    }
                    else if (Active_Tab == 1 && Active_SubTabRage == 2)
                    {
                        ImGui::Separator();
                        ImGui::Checkbox(_xor("Aim Automatic").c_str(), &Settings::Menu::AIM_AimAuto);
                        ImGui::Checkbox(_xor("Aim Lock").c_str(), &Settings::Menu::AIM_AimLock);
                        ImGui::Checkbox(_xor("Aim Silent").c_str(), &Settings::Menu::AIM_Silent);
                        ImGui::Checkbox(_xor("Shotgun Always Headshot").c_str(), &Settings::Menu::AIM_ShotgunAlwaysHead);
                        ImGui::Checkbox(_xor("Sniper Always Headshot").c_str(), &Settings::Menu::AIM_SniperalwaysHead);
                        ImGui::Separator();
                        ImGui::Text(_xor("Aim Bone").c_str());
                        ImGui::RadioButton(_xor("Head").c_str(), &Settings::Menu::AIM_Bone, 0);
                        ImGui::RadioButton(_xor("Neck").c_str(), &Settings::Menu::AIM_Bone, 1);
                        ImGui::RadioButton(_xor("Torso").c_str(), &Settings::Menu::AIM_Bone, 2);
                        ImGui::Separator();
                        ImGui::Text(_xor("Aim Key").c_str());
                        ImGui::RadioButton(_xor("Left Mouse").c_str(), &Settings::Menu::AIM_Key, 0);
                        ImGui::RadioButton(_xor("Right Mouse").c_str(), &Settings::Menu::AIM_Key, 1);
                        ImGui::RadioButton(_xor("Left Shift").c_str(), &Settings::Menu::AIM_Key, 2);
                        ImGui::RadioButton(_xor("Left Ctrl").c_str(), &Settings::Menu::AIM_Key, 3);
                        ImGui::RadioButton(_xor("Left Alt").c_str(), &Settings::Menu::AIM_Key, 4);
                    }
                    else if (Active_Tab == 2 && Active_SubTabVisuals == 1)
                    {
                        ImGui::Checkbox(_xor("Enable ESP").c_str(), &Settings::Menu::ESP_Enabled);
                        ImGui::Checkbox(_xor("Enable On Team").c_str(), &Settings::Menu::ESP_OnTeam);
                       // ImGui::Checkbox(_xor("Radar").c_str(), &Settings::Menu::bESP2D);
                        ImGui::Separator();
                        ImGui::Checkbox(_xor("Check Visibility").c_str(), &Settings::Menu::ESP_CheckVisible);
                        ImGui::Checkbox(_xor("Check Knocked Down").c_str(), &Settings::Menu::ESP_CheckKO);
                        ImGui::Separator();
                        ImGui::Checkbox(_xor("Name").c_str(), &Settings::Menu::ESP_Username);
                        ImGui::Checkbox(_xor("Bones").c_str(), &Settings::Menu::ESP_Bones);
                        ImGui::Checkbox(_xor("2D Box").c_str(), &Settings::Menu::ESP_UseBox);
                        if (Settings::Menu::ESP_UseBox)
                            ImGui::Checkbox(_xor("Fill Box").c_str(), &Settings::Menu::ESP_FillBox);
                        ImGui::Checkbox(_xor("Distance").c_str(), &Settings::Menu::ESP_Distance);
                        ImGui::Separator();
                        ImGui::Checkbox(_xor("Fill Text Background").c_str(), &Settings::Menu::ESP_FillText);
                        ImGui::SliderFloat(_xor("Font Size").c_str(), &Settings::Menu::ESP_FontSize, 10, 24, _xor("%.1f").c_str(), .5f);
                        ImGui::SliderFloat(_xor("Line Thickness").c_str(), &Settings::Menu::ESP_Thickness, .5f, 5, _xor("%.1f").c_str(), .25f);
                        ImGui::EndTabItem();
                    }
                    else if (Active_Tab == 2 && Active_SubTabVisuals == 2)
                    {
                        ImGui::Checkbox(_xor("Llama").c_str(), &Settings::Menu::MARK_Llama);
                        ImGui::Checkbox(_xor("Shark").c_str(), &Settings::Menu::MARK_Shark);
                        ImGui::Checkbox(_xor("Chest").c_str(), &Settings::Menu::MARK_Chest);
                        ImGui::Checkbox(_xor("Ammo Box").c_str(), &Settings::Menu::MARK_AmmoBox);
                        ImGui::Checkbox(_xor("Pickup Ammo").c_str(), &Settings::Menu::MARK_PickUpAmmo);
                        ImGui::Checkbox(_xor("Pickup Items").c_str(), &Settings::Menu::MARK_PickUpItem);
                        ImGui::Checkbox(_xor("Campfire").c_str(), &Settings::Menu::MARK_CampFire);
                        ImGui::Checkbox(_xor("Flopper Spawn").c_str(), &Settings::Menu::MARK_FlopperSpawn);
                        ImGui::Checkbox(_xor("XP Coin").c_str(), &Settings::Menu::MARK_XPCoin);
                        ImGui::Separator();
                        ImGui::Checkbox(_xor("Boat").c_str(), &Settings::Menu::MARK_Boat);
                        ImGui::Checkbox(_xor("Hoagie").c_str(), &Settings::Menu::MARK_Hoagie);
                        ImGui::Separator();
                        ImGui::SliderInt(_xor("Min Tier").c_str(), &Settings::Menu::MARK_MinTier, 1, 8, _xor("%d").c_str());
                        ImGui::SliderFloat(_xor("Max Distance").c_str(), &Settings::Menu::MARK_MaxDistance, 10, 250, _xor("%.1f").c_str());
                        ImGui::EndTabItem();
                    }
                    else if (Active_Tab == 3 && Active_SubTabMisc == 1)
                    {
                        ImGui::Checkbox(_xor("###CameraFoVCheckbox").c_str(), &Settings::Menu::Other_UseCameraFoV); ImGui::SameLine();
                        ImGui::SliderFloat(_xor("Camera FoV").c_str(), &Settings::Menu::Other_CameraFoV, 20, 200, _xor("%.0f").c_str());
                        ImGui::Separator();
                        ImGui::Checkbox(_xor("NoSpread").c_str(), &Settings::Menu::NoSpread);
                    //    ImGui::Checkbox(_xor("No Clip").c_str(), &Settings::Menu::Colission);
                        ImGui::Checkbox(_xor("FirstPerson").c_str(), &Settings::Menu::FirstPerson);
                        ImGui::Checkbox(_xor("Air Stuck [Space + Left Shift]").c_str(), &Settings::Menu::Other_AirStuck);
                        ImGui::Checkbox(_xor("Bullet Teleport [Only Sniper]").c_str(), &Settings::Menu::Other_BulletTP);
                        ImGui::Separator();
                        ImGui::Checkbox(_xor("Hoagie Booster").c_str(), &Settings::Menu::Other_HoagieBooster);
                        if (ImGui::IsItemHovered())
                            ImGui::SetTooltip(_xor("Unlimited hoagie boosts.").c_str());
                        ImGui::BeginGroup();
                        ImGui::Checkbox(_xor("###HoagieSpeedCheckBox").c_str(), &Settings::Menu::Other_HoagieSpeed); ImGui::SameLine();
                        ImGui::SliderFloat(_xor("Hoagie Speed").c_str(), &Settings::Menu::Other_HoagieSpeedMultiplier, 1, 8, _xor("%.0f").c_str());
                        ImGui::EndGroup();
                        if (ImGui::IsItemHovered())
                            ImGui::SetTooltip(_xor("Speedhack for hoagie that works on lifting and when strafing.").c_str());
                        ImGui::Separator();
                        ImGui::Checkbox(_xor("Boat Teleport [F7]").c_str(), &Settings::Menu::Other_BoatTeleport);
                        if (ImGui::IsItemHovered())
                            ImGui::SetTooltip(_xor("Teleport to the nearest real player using boat, No BOT.").c_str());
                        ImGui::Checkbox(_xor("Player Teleport [Shift]").c_str(), &Settings::Menu::Other_PlayerTP);
                        if (ImGui::IsItemHovered())
                            ImGui::SetTooltip(_xor("Teleport to the nearest real Player, No BOT.").c_str());
                        ImGui::BeginGroup();
                        ImGui::Checkbox(_xor("###BoatSpeedCheckBox").c_str(), &Settings::Menu::Other_BoatSpeed); ImGui::SameLine();
                        ImGui::SliderFloat(_xor("Boat Speed").c_str(), &Settings::Menu::Other_BoatSpeedMultiplier, 1, 8, _xor("%.0f").c_str());
                        ImGui::EndGroup();
                        if (ImGui::IsItemHovered())
                            ImGui::SetTooltip(_xor("Speedhack for boat that works on land and when boosting.\nAlso steering will be normal on land and when boosting.").c_str());
                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();
            ImGui::SetCursorPos(ImVec2{ 0, 410 });
            Style->Colors[ImGuiCol_ChildBg] = ImColor(45, 50, 54);
            Style->ChildRounding = 0;
            ImGui::BeginChild("##Text", ImVec2{ 640, 20 }, false);
            {
                ImGui::SetCursorPos(ImVec2{ 505, 3 });
                ImGui::Text("ProjectX");
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();
        ImGui::End();
    }
}

struct {
    FMinimalViewInfo Info;
    float ProjectionMatrix[4][4];
} view = { 0 };

FMinimalViewInfo& GetViewInfo() {
    return view.Info;
}

void Renderer::DrawCanvas()
{
#pragma region Init
    RenderHelper::DrawFoV();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text(_xor("\n\n\n\n\n\ FPS: %.1f").c_str(), (1.0f / io.DeltaTime));

    cCurrentClosest CurrentClosests;
    if (Settings::Menu::AIM_UseFoV) CurrentClosests.fClosestAimbot = Settings::Menu::AIM_FoV;
    EngineHelper::Instance::UWorld = (cUWorld*)(*(uintptr_t*)Settings::BaseOffsets::uWorld);
    if (!EngineHelper::Instance::UWorld) return;
    if (!EngineHelper::Instance::UWorld->GameInstance) return;
    if (!EngineHelper::Instance::UWorld->GameInstance->LocalPlayers[0]) return;
    if (!EngineHelper::Instance::UWorld->GameInstance->LocalPlayers[0]->PlayerController) return;
    EngineHelper::Instance::LocalController = EngineHelper::Instance::UWorld->GameInstance->LocalPlayers[0]->PlayerController;


#pragma endregion

#pragma region Local Actor
    if (EngineHelper::Instance::UWorld->GameInstance->LocalPlayers[0]->PlayerController->AcknowledgedPawn)
    {
        Settings::Local::bIsValid = true;
        Settings::Local::LocalPawn = EngineHelper::Instance::UWorld->GameInstance->LocalPlayers[0]->PlayerController->AcknowledgedPawn;

        if (Settings::Local::LocalPawn->PlayerState)
            Settings::Local::LocalTeam = Settings::Local::LocalPawn->PlayerState->TeamIndex;

        if (Settings::Local::LocalPawn->CurrentWeapon)
        {
            char* sWeaponName = EngineHelper::UObjects::GetObjectName(Settings::Local::LocalPawn->CurrentWeapon);
            Settings::Local::bHasSniper = (strstr(sWeaponName, _xor("Sniper").c_str()) || strstr(sWeaponName, _xor("NoScope").c_str()));
            Settings::Local::bHasShotgun = Settings::Local::bHasSniper ? false : strstr(sWeaponName, _xor("Shotgun").c_str());
        }
        else
        {
            Settings::Local::bHasSniper = false;
            Settings::Local::bHasShotgun = false;
        }

        Hook_GetViewPoint::Hook();
        Hook_ProcessEvent::Hook();
        Hook_CalculateShot::Hook();     
    }
    else
    {
        Settings::Local::bIsValid = false;
        Settings::Local::bHasShotgun = false;
        Settings::Local::bHasSniper = false;
        Settings::Local::LocalTeam = 0;
        Settings::Local::LocalPawn = nullptr;

        CheatAimbot::Details::pAimbotTarget = nullptr;
        CheatManipulation::Details::ClosestPlayer = nullptr;
        CheatManipulation::Details::ClosestBoat = nullptr;
        CheatManipulation::Details::ClosestHoagie = nullptr;
    }
#pragma endregion

#pragma region Level & Actor Loops
    // Level Loop
    for (int32_t nLevel = 0; nLevel < EngineHelper::Instance::UWorld->LevelCount; nLevel++)
    {
        if (!EngineHelper::Instance::UWorld) break;
        if (!EngineHelper::Instance::LocalController) break;

        cLevel* CurrentLevel = EngineHelper::Instance::UWorld->Levels[nLevel];
        if (!CurrentLevel) break;

        // Actor Loop
        for (int32_t nActor = 0; nActor < CurrentLevel->ActorsCount; nActor++)
        {
            if (!CurrentLevel) break;

            cActor* CurrentActor = CurrentLevel->Actors[nActor];
            if (SpoofCall(IsBadReadPtr, (const void*)CurrentActor, (UINT_PTR)8)) continue;
            if (!CurrentActor->SceneComponent) continue;
            if (CurrentActor == EngineHelper::Instance::LocalController->AcknowledgedPawn) continue;
            if (CurrentActor == EngineHelper::Instance::LocalController->SpectatorPawn) continue;

            char* CurrentActorName = EngineHelper::UObjects::GetObjectName((uintptr_t)CurrentActor);
            bool bIsPlayer = strstr(CurrentActorName, _xor("PlayerPawn_Athena_C").c_str());
            bool bIsBot = bIsPlayer ? false : (bool)strstr(CurrentActorName, _xor("PlayerPawn_Athena_Phoebe_C").c_str());
            bool bIsHenchMan = (bIsPlayer || bIsBot) ? false : (bool)strstr(CurrentActorName, _xor("MangPlayerPawn").c_str());
            bool bIsHenchManNEW = (bIsPlayer || bIsBot) ? false : (bool)strstr(CurrentActorName, _xor("BP_MangPlayerPawn_Default_Box_C").c_str());
            bool bIsHenchManGang1 = (bIsPlayer || bIsBot) ? false : (bool)strstr(CurrentActorName, _xor("BP_GibsonPlayerPawn_Heavy_C").c_str());
            bool bIsHenchManGang2 = (bIsPlayer || bIsBot) ? false : (bool)strstr(CurrentActorName, _xor("BP_GibsonPlayerPawn_Grunt_C").c_str());
            bool bIsHenchManGang3 = (bIsPlayer || bIsBot) ? false : (bool)strstr(CurrentActorName, _xor("BP_GibsonPlayerPawn_Elite_C").c_str());
            if (bIsPlayer || bIsBot || bIsHenchMan || bIsHenchManGang1 || bIsHenchManGang2 || bIsHenchManGang3)
            {
                if (Settings::Menu::ESP_Enabled)
                    CheatESP::DrawPlayer(CurrentActor, bIsBot, bIsHenchMan, bIsHenchManNEW, bIsHenchManGang1, bIsHenchManGang2, bIsHenchManGang3);

                if (EngineHelper::Check::CheckClosestPlayer(CurrentActor, CheatAimbot::GetBoneID(), CurrentClosests.fClosestAimbot))
                    CurrentClosests.pClosestAimbot = CurrentActor;

                if (bIsPlayer && EngineHelper::Check::CheckClosest(CurrentActor, CurrentClosests.fClosestPlayer))
                    CurrentClosests.pClosestPlayer = CurrentActor;
            }
            else if (Settings::Menu::MARK_Shark && strstr(CurrentActorName, _xor("NPC_Pawn_SpicySake_Parent_C").c_str()))
            {
                CheatESP::DrawMarker(CurrentActor, _xor("Shark").c_str());
            }
            else if (Settings::Menu::MARK_Llama && strstr(CurrentActorName, _xor("AthenaSupplyDrop_Llama_C").c_str()))
            {
                CheatESP::DrawMarker(CurrentActor, _xor("Llama").c_str());
            }
            else if (Settings::Menu::MARK_Chest && strstr(CurrentActorName, _xor("Tiered_Chest").c_str()))
            {
                if (!CurrentActor->IsSearched())
                    CheatESP::DrawMarker(CurrentActor, _xor("Chest").c_str());
            }
            else if (Settings::Menu::MARK_AmmoBox && strstr(CurrentActorName, _xor("Tiered_Ammo").c_str()))
            {
                if (!CurrentActor->IsSearched())
                    CheatESP::DrawMarker(CurrentActor, _xor("Ammo Box").c_str());
            }
            else if (Settings::Menu::MARK_CampFire && strstr(CurrentActorName, _xor("B_BGA_Athena_EnvCampFire").c_str()))
            {
                CheatESP::DrawMarker(CurrentActor, _xor("Campfire").c_str());
            }
            else if (Settings::Menu::MARK_FlopperSpawn && strstr(CurrentActorName, _xor("BGA_Athena_FlopperSpawn_C").c_str()))
            {
                CheatESP::DrawMarker(CurrentActor, _xor("Flopper Spawn").c_str());
            }


            else if ((Settings::Menu::MARK_PickUpAmmo || Settings::Menu::MARK_PickUpItem) && strstr(CurrentActorName, _xor("FortPickupAthena").c_str()))
            {
                if (!CurrentActor->ItemDefinition || !CurrentActor->ItemDefinition->DisplayName->c_str())
                    break;

                bool bIsAmmo = ((bool)strstr(CurrentActor->ItemDefinition->DisplayName->c_str(), _xor("Ammo:").c_str()));

                if (Settings::Menu::MARK_PickUpAmmo && bIsAmmo)
                    CheatESP::DrawMarker(CurrentActor, CurrentActor->ItemDefinition->DisplayName->c_str());

                if (Settings::Menu::MARK_PickUpItem && !bIsAmmo && CurrentActor->ItemDefinition->Tier >= Settings::Menu::MARK_MinTier)
                    CheatESP::DrawMarkerFromTier(CurrentActor, CurrentActor->ItemDefinition->DisplayName->c_str(), CurrentActor->ItemDefinition->Tier);
            }
          

            else if (Settings::Menu::MARK_XPCoin && (strstr(CurrentActorName, _xor("XPCoin").c_str()) || strstr(CurrentActorName, _xor("xpcoin").c_str())))
            {
                CheatESP::DrawMarker(CurrentActor, _xor("XP Coin").c_str());
            }
            else if (strstr(CurrentActorName, _xor("MeatballVehicle_L_C").c_str()))
            {
                if (EngineHelper::Check::CheckClosest(CurrentActor, CurrentClosests.fClosestBoat))
                    CurrentClosests.pClosestBoat = CurrentActor;

                if (Settings::Menu::MARK_Boat)
                    CheatESP::DrawMarker(CurrentActor, _xor("Boat").c_str());
            }
            else if (strstr(CurrentActorName, _xor("HoagieVehicle_C").c_str()))
            {
                if (EngineHelper::Check::CheckClosest(CurrentActor, CurrentClosests.fClosestHoagie))
                    CurrentClosests.pClosestHoagie = CurrentActor;

                if (Settings::Menu::MARK_Hoagie)
                    CheatESP::DrawMarker(CurrentActor, _xor("Helicoptor").c_str());
            }
        }
    }
#pragma endregion
#pragma region Set Targets
    // Set Targets
    if (Settings::Menu::AIM_AimLock && CheatAimbot::IsAimbotting())
    {
        if (!CheatAimbot::Details::pAimbotTarget)
            CheatAimbot::Details::pAimbotTarget = CurrentClosests.pClosestAimbot;   
    }
    else
    {
        ResetLookInput(EngineHelper::Instance::LocalController);
        CheatAimbot::Details::pAimbotTarget = CurrentClosests.pClosestAimbot;
    }

    CheatManipulation::Details::ClosestPlayer = CurrentClosests.pClosestPlayer;
    CheatManipulation::Details::ClosestBoat = CurrentClosests.pClosestBoat;
    CheatManipulation::Details::ClosestHoagie = CurrentClosests.pClosestHoagie;
#pragma endregion

#pragma region Exploits
    if (Settings::Menu::Other_AirStuck)
    {
        if (Settings::Local::LocalPawn)
        {
            if (DiscordHelper::GetAsyncKeyState(VK_SPACE) && DiscordHelper::GetAsyncKeyState(VK_LSHIFT))
                Settings::Local::LocalPawn->CustomTimeDilation = Settings::fAirStuckValue;
            else
                Settings::Local::LocalPawn->CustomTimeDilation = 1.0f;
        }
    }
    else
    {
        if (Settings::Local::LocalPawn)
            Settings::Local::LocalPawn->CustomTimeDilation = 1.0f;
    }

    if (Settings::Local::bIsValid)
    {
        CheatManipulation::HoagieExploits();
        CheatManipulation::BoatExploits();     
    }
#pragma endregion
}

HRESULT Renderer::DiscordHookCallback(IDXGISwapChain* dxSwapChain, const UINT syncInterval, UINT flags)
{
    if (Settings::bNeedUnHook)
    {
        UnHookDiscordScene();
    }
    else
    {
        static HWND GameHWND = 0;


        if (DiscordHelper::GetAsyncKeyState(VK_INSERT) & 1)
            Details::bShowMenu = !Details::bShowMenu;

        if (!Details::pD11Device || !Details::pD11DeviceContext)
        {
            ImGui::CreateContext();
            if (SUCCEEDED(dxSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&Details::pD11Device)))
            {
                dxSwapChain->GetDevice(__uuidof(Details::pD11Device), (void**)&Details::pD11Device);
                Details::pD11Device->GetImmediateContext(&Details::pD11DeviceContext);
            }

            DXGI_SWAP_CHAIN_DESC desc;
            dxSwapChain->GetDesc(&desc);

            ImGui_ImplWin32_Init(desc.OutputWindow);
            ImGui_ImplDX11_Init(Details::pD11Device, Details::pD11DeviceContext);
            Details::oWndReturn = (WNDPROC)SetWindowLongPtrW(desc.OutputWindow, GWLP_WNDPROC, (LONG_PTR)Renderer::ImGuiHookCallback);

            GameHWND = desc.OutputWindow;

            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontFromMemoryTTF(Fonts::DefaultFont, 16.0f, 16.0f);
            io.Fonts->AddFontFromMemoryTTF(Fonts::SecondaryFont, 12.0f, 12.0f);
            io.Fonts->AddFontFromMemoryTTF(Fonts::WeaponFont, 20.0f, 20.0f);



        }
        else
        {
            ID3D11Texture2D* renderTargetTexture = nullptr;
            if (!Details::pD11RenderTargetView)
            {
                if (SUCCEEDED(dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture))))
                {
                    Details::pD11Device->CreateRenderTargetView(renderTargetTexture, nullptr, &Details::pD11RenderTargetView);
                    renderTargetTexture->Release();
                }
            }
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (!Settings::bNeedUnHook)
        {
            ImGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(Settings::Sceen::ScreenX, Settings::Sceen::ScreenY), ImGuiCond_Always);
            ImGui::SetNextWindowBgAlpha(-1.0f);

            if (ImGui::Begin(_xor("Penis").c_str(), NULL, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                RenderHelper::pDrawList = ImGui::GetWindowDrawList();

                Renderer::DrawCanvas();

                ImGui::End();
            }
        }

        if (GameHWND != 0)
        {
            RECT rScreen;
            if (SpoofCall(GetWindowRect, GameHWND, &rScreen))
            {
                Settings::Sceen::ScreenX = rScreen.right - rScreen.left;
                Settings::Sceen::ScreenY = rScreen.bottom - rScreen.top;
            }
        }
    
        if (Details::bShowMenu)
            Renderer::DrawMenu();

        Details::pD11DeviceContext->OMSetRenderTargets(1, &Details::pD11RenderTargetView, nullptr);
        ImGui::Render();

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    return Details::OPresentReturn(dxSwapChain, syncInterval, flags);
}

HRESULT Renderer::ResizeCallback(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    if (Details::pD11RenderTargetView)
    {
        Details::pD11RenderTargetView->Release();
        Details::pD11RenderTargetView = nullptr;
    }

    return SpoofCall(Details::OResizeReturn, pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

LRESULT WINAPI Renderer::ImGuiHookCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (Details::bShowMenu)
        if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
            return true;

    return CallWindowProc(Details::oWndReturn, hwnd, uMsg, wParam, lParam);
}
