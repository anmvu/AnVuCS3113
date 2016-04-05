#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "PlatformGame.h"

/*
Make A Platformer!
Use tilemap or static/dynamic entities
scroll
procedurally generated/load levels from a file.

*/

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

int main(int argc, char *argv[])
{
	Platformer game;
	while (!game.updateAndRender()) {}
	return 0;
}
