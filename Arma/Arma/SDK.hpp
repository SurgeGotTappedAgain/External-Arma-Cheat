#pragma once
#include "Driver.hpp"
#include "Globals.hpp"
#include "Vector.hpp"

struct Types
{
	static const uint16_t soldier = 28531;
	static const uint16_t hellicopter = 25960;
	static const uint16_t car = 24931;
	static const uint16_t tank = 24948;
	static const uint16_t airplane = 26977;
	static const uint16_t ship = 26739;
};


namespace SDK
{
	uint64_t GetWorld()
	{
		return driver::Read<uint64_t>(Base + 0x25F12A0);
	}
	uint64_t GetLocalPlayer()
	{
		return driver::Read<uint64_t>(GetWorld() + 0x1420);
	}
	uint32_t GetNearTableSize()
	{
		return driver::Read<uint32_t>(GetWorld() + 0x18F0) + 0x8;
	}
	uint32_t GetFarTableSize()
	{
		return driver::Read<uint32_t>(GetWorld() + 0x1C18) + 0x8;
	}
	uint32_t GetFarFarTableSize()
	{
		return driver::Read<uint32_t>(GetWorld() + 0x1F38) + 0x8;
	}
	uint64_t GetNearTable()
	{
		return driver::Read<uint32_t>(GetWorld() + 0x18F0);
	}
	uint64_t GetFarTable()
	{
		return driver::Read<uint32_t>(GetWorld() + 0x1C18);
	}
	uint64_t GetFarFarTable()
	{
		return driver::Read<uint32_t>(GetWorld() + 0x1B48);
	}
	uint64_t GetCameraOn()
	{
		return driver::Read<uint64_t>(GetWorld() + 0x28E0);
	}
	uint64_t GetManVisualState()
	{
		return driver::Read<uint64_t>(GetLocalPlayer() + 0xD0);
	}
	uint64_t GetRenderVisualState()
	{
		return driver::Read<uint64_t>(GetLocalPlayer() + 0x190);
	}
	uint64_t GetCamera()
	{
		return driver::Read<uint64_t>(GetWorld() + 0xA08);
	}
	uint64_t GetEntity(uint64_t PlayerList, uint64_t SelectedPlayer)
	{
		return driver::Read<uint64_t>(PlayerList + SelectedPlayer * 0x8);
	}
	uint64_t GetType(uint64_t Entity)
	{
		// rendererentitytype + configname
		return driver::Read<uint16_t>(driver::Read<uint64_t>(driver::Read<uint64_t>(Entity + 0x150) + 0xD0) + 0x10);
	}
	std::vector<uint64_t> GetNearEntityes()
	{
		std::vector<uint64_t> arrayList;

		for (uint64_t playerId = NULL; playerId < GetNearTableSize(); ++playerId) 
		{
			arrayList.push_back(GetEntity(GetNearTable(), playerId));
		}
		return arrayList;
	}
	Vector3 GetPosition(uintptr_t entity)
	{
		if (entity == SDK::GetLocalPlayer())
		{
			return Vector3(driver::Read<Vector3>(driver::Read<uintptr_t>(entity + 0xD0) + 0x2C));
		}
		else
		{
			return Vector3(driver::Read<Vector3>(driver::Read<uintptr_t>(entity + 0x190) + 0x2C));
		}
	}
	std::vector<uint64_t> GetFarEntityes()
	{
		std::vector<uint64_t> arrayList;

		for (uint64_t playerId = NULL; playerId < GetFarTableSize(); ++playerId)
		{
			arrayList.push_back(GetEntity(GetFarTable(), playerId));
		}
		return arrayList;
	}
	Vector3 GetInvertedViewRight() 
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0x8));
	}
	Vector3 GetInvertedViewUp() 
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0x14));
	}
	Vector3 GetInvertedViewForward() 
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0x20));
	}
	Vector3 GetViewportSize() 
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0x58));
	}
	Vector3 GetInvertedViewTranslation()
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0x2C));
	}
	Vector3 GetProjectionD1()
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0xD0));
	}
	Vector3 GetProjectionD2() 
	{
		return Vector3(driver::Read<Vector3>(GetCamera() + 0xDC));
	}
	Vector3 WorldToScreen(Vector3 Position)
	{
		if (!GetCamera()) return Vector3();

		Vector3 temp = Position - GetInvertedViewTranslation();

		float x = temp.Dot(GetInvertedViewRight());
		float y = temp.Dot(GetInvertedViewUp());
		float z = temp.Dot(GetInvertedViewForward());

		if (z > 0)
		{
			return Vector3(GetViewportSize().x * (1 + (x / GetProjectionD1().x / z)), GetViewportSize().y * (1 - (y / GetProjectionD2().y / z)), z);
		}

	}
	uint64_t AIUnitInfo(uint64_t Entity)
	{
		return driver::Read<uint64_t>(Entity + 0xA90);
	}
	std::string GetPlayerName(uint64_t Entity)
	{
		return driver::ReadArmaString(AIUnitInfo(Entity));
	}
	uint64_t GetRawTotalDamage(uint64_t Entity)
	{
		return driver::Read<uint64_t>(Entity + 0xF0);
	}

	Vector3 GetHeadPosition(uint64_t Entity)
	{
		return Vector3(driver::Read<Vector3>(driver::Read<uint64_t>(Entity + 0x190) + 0x168));
	}

	uint8_t GetHealth(uint64_t Entity)
	{
		uint8_t _tempHealth = (uint8_t)abs(
			driver::Read<float>(GetRawTotalDamage(Entity) + 0x0) * 100 - 100);

		if (!GetRawTotalDamage(Entity))
		{
			return 100;
		}

		if (_tempHealth > 100) 
		{
			return 100;
		}

		if (_tempHealth < 0) 
		{
			return 100;
		}

		return _tempHealth;
	}


	float RadiansToDegrees(float radians) 
	{
		float degrees;
		degrees = ((radians * 180) / 3.14159265358979323846);
		return degrees;
	}
}
