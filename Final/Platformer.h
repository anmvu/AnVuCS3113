#ifndef Platformer
#define Platformer

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "Entity.h"
using namespace std;



#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define GRAVITY 0
#define COLLISION_OFFSET 0.000001f;
#define TILE_SIZE 0.22f
#define LEVEL_HEIGHT 34
#define LEVEL_WIDTH 42
#define SPRITE_COUNT_X 30
#define SPRITE_COUNT_Y 16

class PlatformerGame{
public:


};

#endif