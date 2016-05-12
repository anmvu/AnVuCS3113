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
#define TILE_SIZE 0.22f
#define LEVEL_HEIGHT 34
#define LEVEL_WIDTH 42
#define SPRITE_COUNT_X 30
#define SPRITE_COUNT_Y 16


class SheetSprite{

public:
	SheetSprite();
	SheetSprite(GLuint textureId, float index, int countX, int countY); //uniform sheet
	SheetSprite(GLuint textureId, float u, float v, float width, float height); //non-uniform sheet

	void draw(ShaderProgram& program,float objWidth, float objHeight);

	void scale(float factor);

	GLuint textureId;

	float xScale = 0.5f;
	float yScale = 0.5f;

	float u;
	float v;
	float width;
	float height;

	float index;
	int countX;
	int countY;

};


#endif