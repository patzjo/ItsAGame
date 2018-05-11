#pragma once

#include <string>

enum AssetType {FONT, TEXTURE, SOUND, MUSIC};

extern const char *assetsDirectory;
extern const char *fontsDirectory;
extern const char *texturesDirectory;
extern const char *soundsDirectory;
extern const char *musicsDirectory;
extern const char *stencilsDirectory;

struct AssetInfo
{
	char filename[255];
	int type;
};

const struct AssetInfo assets[]
{
//  FILENAME            TYPE        INDEX NUMBER
// FONTS
	"arial.ttf",		FONT,		// 0

// TEXTURES
	"tank1.png",		TEXTURE,	// 0

// SOUNDS
	"explodemini.wav",	SOUND,		// 0 
	"explode.wav",		SOUND,		// 1

// MUSICS
	"rauha.wav",		MUSIC,		// 0
};