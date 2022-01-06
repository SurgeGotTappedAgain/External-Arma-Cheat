#include "Driver.hpp"
#include "vector.hpp"
#include "SDK.hpp"
#include "Overlay.hpp"

ImFont* m_pFont;
LPDIRECT3DDEVICE9 d3ddev;
ID3DXLine* dx_Line;
ID3DXFont* dx_Font;
IDirect3DDevice9Ex* p_Device;
DWORD blackColor = D3DCOLOR_ARGB(255, 0, 0, 0);
DWORD healthBarColor = D3DCOLOR_ARGB(200, 000, 255, 000);
DWORD Name = D3DCOLOR_ARGB(255, 255, 255, 255);


std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}
void DrawCorneredBox(int X, int Y, int W, int H, const ImU32& color, int thickness)
{
	float lineW = (W / 3);
	float lineH = (H / 3);

	////black outlines
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);

	//corners
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}
void DrawBox(float X, float Y, float W, float H, const ImU32 color)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(X + 1, Y + 1), ImVec2(((X + W) - 1), ((Y + H) - 1)), ImGui::GetColorU32(color));
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), ImGui::GetColorU32(color));
}
void DrawFilledRect(float x, float y, float w, float h, D3DCOLOR color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, NULL, NULL);
}
void DrawStrokeText(int x, int y, const char* str)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);

	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x - 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(255, 255, 255, 255)), utf_8_2.c_str());
}

void Line(ImVec2 pos, ImVec2 size, ImU32 color, float thickness)
{
	ImGui::GetWindowDrawList()->AddLine(pos, size, color, thickness);
}

void crosshair()
{
	float MidX = 1920 / 2;
	float MidY = 1080 / 2;

	Line({ MidX - 10, MidY }, { MidX - 0, MidY }, ImColor(255.f, 255.f, 255.f, 1.f), 1.6);
	Line({ MidX, MidY - 10 }, { MidX, MidY - 0 }, ImColor(255.f, 255.f, 255.f, 1.f), 1.6);
	Line({ MidX + 10, MidY }, { MidX + 0, MidY }, ImColor(255.f, 255.f, 255.f, 1.f), 1.6);
	Line({ MidX, MidY + 10 }, { MidX, MidY + 0 }, ImColor(255.f, 255.f, 255.f, 1.f), 1.6);
}

namespace Render
{
	std::vector<player_t> entities = {};

	void Cache()
	{
		for (uint32_t i = 0; i < entities.size(); i++)
		{
			auto Entities = entities[i];

			Vector3 worldPosition = SDK::GetPosition(Entities.EntityPtr);
			Vector3 headPosition = SDK::GetHeadPosition(Entities.EntityPtr);

			Vector3 screenPosition = SDK::WorldToScreen(worldPosition);
			Vector3 screenHeadPosition = SDK::WorldToScreen(headPosition);

			float height = abs(screenHeadPosition.y - screenPosition.y);
			float width = height * 0.65f;

			uint8_t Health = SDK::GetHealth(Entities.EntityPtr);
			float CurrentHealth = Health / 100.0f;
			std::string HP = std::to_string(Health).c_str();
			uint64_t EntityType = Entities.EntityType;

			if (EntityType != Types::soldier) continue;
			if (Health < 0 || Health >= 1) Health = 1;
			if (CurrentHealth == 0) continue;
			if (Entities.EntityPtr == SDK::GetLocalPlayer()) continue;

			if (EntityType == Types::soldier)
			{
				// Box
				DrawCorneredBox(screenHeadPosition.x - (width / 2) - 1, screenHeadPosition.y - (height / 5) - 1, width + 2, height + 12, ImColor(255, 255, 255, 255), 0.5);
				DrawFilledRect(screenHeadPosition.x - (width / 2) - 1, screenHeadPosition.y - (height / 5) - 1, width + 2, height + 12, D3DCOLOR_ARGB(93, 0, 0, 0));

				// Healthbar
				DrawFilledRect(screenHeadPosition.x - (width / 2) - 6 - 1, screenHeadPosition.y - (height / 5) - 1, 5, height + 10 + 2, blackColor);
				DrawFilledRect(screenHeadPosition.x - (width / 2) - 6, screenHeadPosition.y - (height / 5), 3, (height + 10) * Health, healthBarColor);
			}
		}
	}
}