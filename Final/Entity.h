#ifndef ENTITY_H
#define ENTITY_H
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Vector.h"

#include "Matrix.h"
#include "SpriteSheet.h"
#include <string>
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Entity {
public:

	Entity();
	virtual void update(float elapsed);
	void render();

	bool collidesWith(Entity* entity, float x_offset = 0.0f, float y_offset = 0.0f);
	bool collidesWithCenter(Entity* entity);

	void fixedUpdate(); 
	void scale(float factor);

	SheetSprite sprite;
	ShaderProgram program;
	float x;
	float y;
	float z;
	float width;
	float height;
	float xScale = 1;
	float yScale = 1;

	float velocityX;
	float velocityY;
	float accelerationX;
	float accelerationY;
	float frictionX;
	float frictionY;
	bool hasGrav;
	bool isStatic;
	bool isVisible;
	bool isAnimated;
	int animStart;
	int animEnd;
	int animLength;
	float animSpeed;
	bool enableCollisions;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

};

#endif