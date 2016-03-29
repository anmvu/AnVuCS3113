#ifndef Platformer
#define Platformer

#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <vector>

#include "Entity.h"

#define MAX_TILES 124
#define GRAVITY -10
#define COLLISION_OFFSET 0.000001f;

class PlatformerGame{
public:
	PlatformerGame();
	~PlatformerGame();

	GLuint loadTexture(const char* image_path, GLenum image_format = GL_BGRA, GLint texParam = GL_NEAREST);
	void drawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);
	void handleCollisionX(Entity* entity, std::vector<Entity*> entities);
	void handleCollisionY(Entity* entity, std::vector<Entity*> entities);
	
	void init();
	void buildLevel();
	bool updateAndRender();
	void render();
	void update(float elapsed);
	void fixedUpdate();
	
private:
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;
	float timeLeftOver = 0.0f;
	const Uint8* keys;
	std::vector<Entity*> entities;
	Entity player;
	Entity tiles[MAX_TILES];
	SDL_Event event;
};

#endif