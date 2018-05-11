#pragma once

#include <string>

enum AssetType {FONT, TEXTURE, SOUND, MUSIC};

const char *assetsDirectory		= "Assets/";
const char *fontsDirectory		= "Fonts/";
const char *texturesDirectory	= "Textures/";
const char *soundsDirectory		= "Sounds/";
const char *musicsDirectory		= "Musics/";
const char *stencilsDirectory	= "Stencils/";

struct AssetInfo
{
	char filename[255];
	int type;
};

constexpr AssetInfo assets[]
{
// FONTS
	"arial.ttf",		FONT,		// start index 0

// TEXTURES
	"tank1.png",		TEXTURE,	// start index 0 

// SOUNDS
	"sound.wav",		SOUND		// start index 0

// MUSICS

};