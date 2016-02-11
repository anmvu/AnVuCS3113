#include "Entity.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

Entity::Entity(float width = 0.1, float height = 0.1) : width(width), height(height){}


void Entity::draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);

	float quads[] = {
		-width, height,
		-width, height,
		-width, -height,
		width, height,
		-width, -height,
		width, -height
	};

	glVertexPointer(2,GL_FLOAT,0,quads);

	GLfloat coords[] = { 
		0.0,0.0,
		0.0,1.0,
		1.0,1.0,
		1.0,0.0 
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, coords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);

}