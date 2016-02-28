#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "SpaceInvader.h"

/*
Make Space Invader!
2 Screens: Title and Game
Display Text
Use Sprite Sheets
Any Graphics
*/

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

int main(int argc, char *argv[])
{
	SpaceInvader game;
	while (!game.updateAndRender()) {}
	return 0;
}
