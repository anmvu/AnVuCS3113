#ifndef SpriteSheet
#define SpriteSheet

#include <SDL_opengl.h>

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class SheetSprite {
public:
	SheetSprite();
	SheetSprite(GLuint textureId, float spriteIndex, int spriteCountX, int spriteCountY); //uniform sheet
	SheetSprite(GLuint textureId, float u, float v, float width, float height); //non-uniform sheet

	void Draw(float objWidth, float objHeight, float x = 0.0f, float y = 0.0f, float scale = 0.5f); //sprite_index?

	float scale;
	GLuint textureId;
	float u;
	float v;
	float width;
	float height;
};

#endif