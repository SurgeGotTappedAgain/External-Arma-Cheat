#pragma once
#include <cstdint>

typedef struct _player_t
{
	std::uint64_t EntityPtr;
	std::uint64_t EntityType;
} player_t;

struct globals
{
	uint64_t World;

	// menu 
	float width = 1920.f;
	float height = 1080.f;
	bool menu = false;
};

extern globals Globals;