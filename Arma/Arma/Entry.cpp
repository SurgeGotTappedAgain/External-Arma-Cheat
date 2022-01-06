#include "Driver.hpp"
#include "Overlay.hpp"
#include "Globals.hpp"
#include "SDK.hpp"
HWND hwnd = nullptr;
globals Globals;

// This Only Loops Nearby Entities, You Can Add Far Entities Yourself
void NearTableList()
{
	while (true)
	{
		std::vector<player_t> tmp{};
		uint32_t NearTableSize = driver::Read<uint32_t>(SDK::GetWorld() + 0x18F0 + 0x8);
		uint32_t FarTableSize = driver::Read<uint32_t>(SDK::GetWorld() + 0x1B48 + 0x8);

		for (uint32_t i = 0; i <= NearTableSize; i++)
		{
			uint64_t EntityTable = driver::Read<uint64_t>(SDK::GetWorld() + 0x18F0);
			if (!EntityTable) continue;

			uint64_t Entity = driver::Read<uint64_t>(EntityTable + (i * 0x8));
			if (!Entity) continue;

			uint64_t EntityType = driver::Read<uint16_t>(driver::Read<uint64_t>(driver::Read<uint64_t>(Entity + 0x150) + 0xD0) + 0x10);
			if (!EntityType) continue;

			if (EntityType != Types::soldier) continue;

			player_t Player{};
			Player.EntityPtr = Entity;
			Player.EntityType = EntityType;

			tmp.push_back(Player);
		}
		Render::entities = tmp;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

}

int main()
{
	// add your own driver with read/write
	if (driver::openMemoryHandle())
	{
		std::cout << "[Communication Established]" << std::endl;
	}

	while (!hwnd)
	{
		hwnd = FindWindowA("Arma 3", NULL);
	}

	std::cout << "[FOUND ARMA]" << std::endl;

	driver::getProcessID("arma3_x64.exe");
	Base = driver::getModuleBase("arma3_x64.exe");

	std::cout << "[BASE] 0x" << std::hex << Base << std::endl;

	std::thread(NearTableList).detach();

	setup_window();
	setup_directx(MyWnd);

	while (true)
	{
	     main_loop();
	}

	std::getchar();
}
