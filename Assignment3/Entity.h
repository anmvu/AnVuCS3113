#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity{
public:

	Entity(GLuint textureID = 0, float width = 0.1f, float height = 0.1f,float x = 0.0f, float y = 0.0f, 
		float angle = 0.0f, bool visible = true, float xScale = 1.0f, float yScale = 1.0f);

	void drawSprite();
	void drawFromSprite(int spriteX, int spriteY);

	void setSize(float height, float width);
	void scale(float scaleSize);
	void die();
	bool alive();
	void move(float elapsed);

	GLuint textureId;
	float spriteIndex;
	int startSprite;
	int maxSprite;
	float speed;
	float animation;
	bool visible;
	float x;
	float y;
	float xScale;
	float yScale;
	float rotation;
	float width;
	float height;
	float angle;
	float max_speed;
	float xDir;
	float yDir;
	float lives;

};

#endif