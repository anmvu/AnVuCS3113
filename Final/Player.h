#ifndef PLAYER
#define PLAYER

#define MAX_SPIKES 8
#define JUMP_SPEED 4.0f
#define SPAWN_DELAY 50

#include "Entity.h"
#include <vector>

class Player : public Entity {
public:

	Player();

	void update(float elapsed);
	void setAnimation(int animStart, int animLength, float animSpeed);
	void buildSpike();
	void placeSpike();
	void jump();
	void respawn();

	bool collidedBottom;
	int start;
	int health;
	int animStartOrig;
	int animEndOrig;
	bool canDoubleJump;
	bool canDrop;
	Entity spike;
	int spikeIndex;
	float sinInc;
	float spawnDelay;
	std::vector<std::pair<float, float>> spawnPoints;
	int score;
	float scoreTextSize;
};
#endif