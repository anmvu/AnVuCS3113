#ifndef ENTITY_H
#define ENTITY_H

class Entity{
public:

	Entity(float width = 0.1f, float height = 0.1f);

	void draw();

	float x;
	float y;
	float rotation;

	int textureID;

	float width;
	float height;
	
	float angle;
	float speed;
	float max_speed;
	float direction_x;
	float direction_y;

};

#endif