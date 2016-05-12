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


class SheetSprite{

public:
	SheetSprite();
	SheetSprite(GLuint textureId, float index, int countX, int countY, float x_scale = 0.5f, float y_scale = 0.5f); //uniform sheet
	SheetSprite(GLuint textureId, float u, float v, float width, float height); //non-uniform sheet

	void Draw(float objWidth, float objHeight);
	void FlipX();

	GLuint textureId;

	float x_scale;
	float y_scale;

	float u;
	float v;
	float width;
	float height;

	float index;
	int countX;
	int countY;
};


#endif