#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "SpriteSheet.h"

class Entity{
public:

	Entity();

	void update(float elapsed);
	void render();
	bool collides(Entity* e);
	void fixedUpdate();
	void drawSprite();
	void drawFromSprite(int spriteX, int spriteY);
	float lerp(float v0, float v1, float t);

	SheetSprite sprite;
	GLuint textureId;
	float spriteIndex;
	int spriteCountX;
	int spriteCountY;
	float x;
	float y;

	float width;
	float height;
	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	float friction_x;
	float friction_y;
	float mass;
	bool isStatic;

	bool enableCollisions;

	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

};

#endif