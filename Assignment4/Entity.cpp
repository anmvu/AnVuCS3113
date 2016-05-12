#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include "Entity.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>

Entity::Entity() {}

void Entity::Render() {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	sprite.Draw(width, height);
	glPopMatrix();
}

bool Entity::CollidesWith(Entity* entity) {
	return (
		x + (width * 0.5) > entity->x - (entity->width * 0.5) &&
		x - (width * 0.5) < entity->x + (entity->width * 0.5) &&
		y + (height * 0.5) > entity->y - (entity->height * 0.5) &&
		y - (height * 0.5) < entity->y + (entity->height * 0.5)
		);
}

void Entity::FixedUpdate() {
	if (!isStatic) {
		velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
		velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
		velocity_x += acceleration_x * FIXED_TIMESTEP;
		velocity_y += acceleration_y * FIXED_TIMESTEP;
	}
}

float Entity::lerp(float v0, float v1, float t) {
	return (1.0 - t) * v0 + t * v1;
}