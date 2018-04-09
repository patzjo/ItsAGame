#pragma once
#include <string>

enum AssetType {FONT, TEXTURE, SOUND};

struct AssetInfo
{
	char filename[255];
	int type;
};

constexpr AssetInfo assets[]
{
// FONTS
	"arial.ttf", FONT,	// 0

// TEXTURES
	"tank1.png",  TEXTURE,	// 0

// SOUNDS
	

};