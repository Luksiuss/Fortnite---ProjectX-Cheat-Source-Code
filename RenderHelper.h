#pragma once
namespace RenderHelper
{
	ImFont* pFonts[3];
	ImDrawList* pDrawList;

	void GetFonts();
	void DrawFoV();

	void Box(Vector2 vPos, Vector2 vSize, float fThickness, Vector4 vColor);
	void Box(Vector2 vPos, Vector2 vSize, float fThickness, Vector4 vColor, Vector4 vBorderColor);

	void Cirlce(Vector2 vPos, float fThickness, float fRadius, Vector4 vColor);
	void Cirlce(Vector2 vPos, float fThickness, float fRadius, Vector4 vColor, Vector4 vBorderColor);

	void Line(Vector2 vPos, Vector2 vPosEnd, float fThickness, Vector4 vColor);

	void Text(ImFont* pFont, Vector2 vPos, const char* sMessage, Vector4 vColor, float fFontSize = 16.0f, bool bIsCenter = false);
	void Text(ImFont* pFont, Vector2 vPos, const char* sMessage, Vector4 vColor, Vector4 vBackgrondColor, float fFontSize = 16.0f, bool bIsCenter = false);
}

void RenderHelper::GetFonts()
{
	ImGuiIO& io = ImGui::GetIO();
	pFonts[0] = io.Fonts->Fonts[0];  // Default Font.
	pFonts[1] = io.Fonts->Fonts[1];  // Secondary Font.
	pFonts[2] = io.Fonts->Fonts[2];  // Weapon Font.
}

void RenderHelper::DrawFoV()
{
	// FoV
	if (Settings::Menu::AIM_Enabled && Settings::Menu::AIM_UseFoV)
	{
		if (Settings::Menu::AIM_FoVType == 1)
			RenderHelper::Box(Vector2(Settings::Sceen::ScreenX / 2 - Settings::Menu::AIM_FoV, Settings::Sceen::ScreenY / 2 - Settings::Menu::AIM_FoV), Vector2((Settings::Menu::AIM_FoV * 2), (Settings::Menu::AIM_FoV * 2)), Settings::Menu::ESP_Thickness, Settings::Menu::MiscColor);
		else
			RenderHelper::Cirlce(Vector2(Settings::Sceen::ScreenX / 2, Settings::Sceen::ScreenY / 2), Settings::Menu::ESP_Thickness, Settings::Menu::AIM_FoV, Settings::Menu::MiscColor);

		RenderHelper::Cirlce(Vector2(Settings::Sceen::ScreenX / 2, Settings::Sceen::ScreenY / 2), Settings::Menu::ESP_Thickness, 3, Vector4(Settings::Menu::MiscColor.x, Settings::Menu::MiscColor.y, Settings::Menu::MiscColor.z, Settings::Menu::MiscColor.w).ToColor(.4f), Settings::Menu::MiscColor);
	}
}

void RenderHelper::Box(Vector2 vPos, Vector2 vSize, float fThickness, Vector4 vColor)
{
	pDrawList->AddRect(vPos.ToImVec(), ImVec2(vPos.x + vSize.x, vPos.y + vSize.y), ImGui::GetColorU32(vColor.ToImVec()), 0, 0, fThickness);
}

void RenderHelper::Box(Vector2 vPos, Vector2 vSize, float fThickness, Vector4 vColor, Vector4 vBorderColor)
{
	pDrawList->AddRectFilled(vPos.ToImVec(), ImVec2(vPos.x + vSize.x, vPos.y + vSize.y), ImGui::GetColorU32(vColor.ToImVec()), 0);
	pDrawList->AddRect(vPos.ToImVec(), ImVec2(vPos.x + vSize.x, vPos.y + vSize.y), ImGui::GetColorU32(vBorderColor.ToImVec()), 0, 0, fThickness);
}

void RenderHelper::Cirlce(Vector2 vPos, float fThickness, float fRadius, Vector4 vColor)
{
	pDrawList->AddCircle(vPos.ToImVec(), fRadius, ImGui::GetColorU32(vColor.ToImVec()), 55, fThickness);
}

void RenderHelper::Cirlce(Vector2 vPos, float fThickness, float fRadius, Vector4 vColor, Vector4 vBorderColor)
{
	pDrawList->AddCircleFilled(vPos.ToImVec(), fRadius, ImGui::GetColorU32(vColor.ToImVec()), 55);
	pDrawList->AddCircle(vPos.ToImVec(), fRadius, ImGui::GetColorU32(vBorderColor.ToImVec()), 55, fThickness);
}

void RenderHelper::Line(Vector2 vPos, Vector2 vPosEnd, float fThickness, Vector4 vColor)
{
	if (vPos.x < 1 || vPosEnd.x < 1)
		return;

	pDrawList->AddLine(vPos.ToImVec(), vPosEnd.ToImVec(), ImGui::GetColorU32(vColor.ToImVec()), fThickness);
}

void RenderHelper::Text(ImFont* pFont, Vector2 vPos, const char* sMessage, Vector4 vColor, float fFontSize, bool bIsCenter)
{
	if (pFonts[0] == nullptr)
		GetFonts();

	if (pFont == nullptr)
		pFont = pFonts[0];

	ImVec2 vTextSize;
	if (bIsCenter)
		vTextSize = pFont->CalcTextSizeA(fFontSize, FLT_MAX, FLT_MAX, sMessage);

	if (bIsCenter)
		vPos.x -= (vTextSize.x / 2);

	pDrawList->AddText(pFont, fFontSize, vPos.ToImVec(), ImGui::GetColorU32(vColor.ToImVec()), sMessage);
}

void RenderHelper::Text(ImFont* pFont, Vector2 vPos, const char* sMessage, Vector4 vColor, Vector4 vBackgrondColor, float fFontSize, bool bIsCenter)
{
	if (pFonts[0] == nullptr)
		GetFonts();

	if (pFont == nullptr)
		pFont = pFonts[0];

	ImVec2 vTextSize = pFont->CalcTextSizeA(fFontSize, FLT_MAX, FLT_MAX, sMessage);

	if (bIsCenter)
		vPos.x -= (vTextSize.x / 2);

	const float XOffset = 2.0f;
	const float YOffset = 1.0f;
	Box(vPos, Vector2(vTextSize.x + (XOffset * 2), vTextSize.y + (YOffset * 2)), 0, vBackgrondColor, vBackgrondColor);
	vPos.x += XOffset;
	vPos.y += YOffset;

	pDrawList->AddText(pFont, fFontSize, vPos.ToImVec(), ImGui::GetColorU32(vColor.ToImVec()), sMessage);
}
