#ifndef SpriteSheet
#define SpriteSheet

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL_opengl.h>
#include "Matrix.h"
#include "ShaderProgram.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class SheetSprite {
public:
	SheetSprite();
	SheetSprite(float spriteIndex, int spriteCountX, int spriteCountY); //uniform sheet

	void draw(ShaderProgram& program, GLuint textureId, float objWidth, float objHeight, float x, float y, float scale);

	float u;
	float v;
	float width;
	float height;
};

#endif