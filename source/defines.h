#pragma once

#include "SDL.h"

extern SDL_Window *window;
extern SDL_Renderer *renderer;

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 544

#define PLAYFIELD_X_OFFSET ((SCREEN_WIDTH-640)/2)
#define PLAYFIELD_Y_OFFSET 64

#define SCREEN_X_TO_PLAYFIELD_X(x) ((x)-PLAYFIELD_X_OFFSET)
#define SCREEN_Y_TO_PLAYFIELD_Y(y) ((y)-PLAYFIELD_Y_OFFSET)

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#ifdef VITA
#define IS_PLATFORM_VITA true
#else
#define IS_PLATFORM_VITA false
#endif

/*
 * NOTE! In DEF_*Directory/DEF_*Subdirectory defines, trailing slash is mandatory!
 */

#ifdef VITA
#define DEF_DataDirectory "ux0:/data/osu/"
#define DEF_BeatmapsSubdirectory "maps/"
#define DEF_SkinsSubdirectory "skins/"
#define DEF_LanguagesSubdirectory "languages/"
#define DEF_SettingsFilename "settings.json"
#define DEF_HighscoresFilename "highscores.json"
#define DEF_BeatmapsIndexFilename "beatmaps.json"
#define DEF_BeatmapsStateFilename "beatmaps_state.json"
#endif

#ifdef DESKTOP
#define DEF_DataDirectory "./"
#define DEF_BeatmapsSubdirectory "maps/"
#define DEF_SkinsSubdirectory "skins/"
#define DEF_LanguagesSubdirectory "languages/"
#define DEF_SettingsFilename "settings.json"
#define DEF_HighscoresFilename "highscores.json"
#define DEF_BeatmapsIndexFilename "beatmaps.json"
#define DEF_BeatmapsStateFilename "beatmaps_state.json"
#endif
