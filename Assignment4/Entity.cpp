#include "Entity.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

Entity::Entity(GLuint textureID, float width, float height, float x, float y, 
	float angle, bool visible, float xScale, float yScale) : 
	textureId(textureId), width(width), height(height), x(x), y(y), 
	angle(angle), visible(visible),xScale(xScale), yScale(yScale){}


void Entity::drawSprite(){
	if (alive()){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(x, y, 0.0);
		glRotatef(angle, 0.0, 0.0, 1.0);
		glScalef(xScale, yScale, 0.0);

		float quads[] = {
			-width, height,
			-width, -height,
			width, -height,
			width, height
		};

		glVertexPointer(2, GL_FLOAT, 0, quads);
		glEnableClientState(GL_VERTEX_ARRAY);

		GLfloat coords[] = {
			0.0, 0.0,
			0.0, 1.0,
			1.0, 1.0,
			1.0, 0.0
		};
		glEnableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, coords);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}

}

void Entity::drawFromSprite(int spriteX, int spriteY){
	if (alive()){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(x, y, 0.0);
		glRotatef(angle, 0.0, 0.0, 1.0);
		glScalef(xScale, yScale, 0.0);
		float quads[] = {
			-width, height,
			-width, -height,
			width, -height,
			width, height
		};

		glVertexPointer(2, GL_FLOAT, 0, quads);
		glEnableClientState(GL_VERTEX_ARRAY);

		float u = (float)(((int)spriteIndex) % spriteX) / (float)spriteX;
		float v = (float)(((int)spriteIndex) / spriteX) / (float)spriteY;
		float spriteWidth = 1.0 / (float)spriteX;
		float spriteHeight = 1.0 / (float)spriteY;

		GLfloat coords[] = {
			u, v,
			u, v + spriteHeight,
			u + spriteWidth, v + spriteHeight,
			u + spriteWidth, v
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, coords);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_TEXTURE_2D);
	}
}

void Entity::setSize(float height, float width){
	this->height = height;
	this->width = width;
}

void Entity::scale(float scaleSize){
	xScale *= scaleSize;
	yScale *= scaleSize;
}

void Entity::die(){
	visible = false;
}

bool Entity::alive(){
	return visible;
}

void Entity::move(float elapsed){
	x += xDir * speed * elapsed;
	y += yDir * speed * elapsed;
}