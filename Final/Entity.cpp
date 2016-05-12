#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include "Entity.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>



Entity::Entity() {}

float lerp(float from, float to, float time) {
	return (1.0 - time)*from + time*to;
}

void Entity::update(float elapsed) {
	return;
}

void Entity::render() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	if (isVisible) {
		sprite.draw(program, width * xScale, height * yScale);
	}
	glPopMatrix();
}

bool Entity::collidesWith(Entity* entity, float x_offset, float y_offset) {
	return (
		x + (width * 0.5) > (entity->x + x_offset) - (entity->width * 0.5) &&
		x - (width * 0.5) < (entity->x + x_offset) + (entity->width * 0.5) &&
		y + (height * 0.5) > (entity->y + y_offset) - (entity->height * 0.5) &&
		y - (height * 0.5) < (entity->y + y_offset) + (entity->height * 0.5)
		);
}

bool Entity::collidesWithCenter(Entity* entity) {
	return (
		x + (width * 0.5) > entity->x &&
		x - (width * 0.5) < entity->x &&
		y + (height * 0.5) > entity->y &&
		y - (height * 0.5) < entity->y
		);
}

void Entity::fixedUpdate() {
	if (!isStatic) {
		velocityX = lerp(velocityX, 0.0f, FIXED_TIMESTEP * frictionX);
		velocityY = lerp(velocityY, 0.0f, FIXED_TIMESTEP * frictionY);
		velocityX += accelerationX * FIXED_TIMESTEP;
		velocityY += accelerationY * FIXED_TIMESTEP;
	}
}

void Entity::scale(float factor) {
	xScale *= factor;
	yScale *= factor;
}