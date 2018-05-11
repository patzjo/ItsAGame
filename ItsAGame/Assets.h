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
// FONTS
	"arial.ttf",		FONT,		// start index 0

// TEXTURES
	"tank1.png",		TEXTURE,	// start index 0 

// SOUNDS
	"explode.wav",		SOUND,		// start index 0
	"explodemini.wav",	SOUND, 

// MUSICS

};