#ifndef PLATFORMER
#define PLATFORMER

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include <iostream>
#include <vector>

#include "Entity.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define GRAVITY -10
#define MAX_TILES 10000
#define COLLISION_OFFSET 0.000000000001f;

class Platformer{
public:
	Platformer();
	~Platformer();

	ShaderProgram program;
	Matrix PlatformModelMatrix, platformProjectionMatrix, platformViewMatrix;

	void init();
	void buildLevel();
	bool updateAndRender();
	void render();
	void update(float elapsed);
	GLuint loadTexture(const char* image_path);
	void drawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing);
	void drawSprite(Entity e);

private:
	const char* spriteSheet;
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;
	float timeLeftOver = 0.0f;
	const Uint8* keys;
	std::vector<Entity*> entities;
	Entity tiles[MAX_TILES];
	SDL_Event event;
};


#endif