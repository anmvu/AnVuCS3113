#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include "Entity.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

Entity::Entity() {}

void Entity::render(ShaderProgram &program) {
	sprite.draw(program, textureId,width, height, x, y, 1);
}

float Entity::lerp(float v0, float v1, float t){
	return (1.0 - t)*v0 + t * v1;
}

bool Entity::collides(Entity* entity) {
	return (
		x + (width * 0.5) > entity->x - (entity->width * 0.5) &&
		x - (width * 0.5) < entity->x + (entity->width * 0.5) &&
		y + (height * 0.5) > entity->y - (entity->height * 0.5) &&
		y - (height * 0.5) < entity->y + (entity->height * 0.5)
		);
}

void Entity::fixedUpdate() {
	if (!isStatic) {
		velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
		velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * friction_y);
		velocity_x += acceleration_x * FIXED_TIMESTEP;
		velocity_y += acceleration_y * FIXED_TIMESTEP;
	}
}
