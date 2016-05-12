#include "Player.h"
#include "Entity.h"
#include <cmath>

Player::Player() {
	canDrop = false;
	buildSpike();
	score = 0;
	scoreTextSize = 0.05f;
	collidedBottom = true;
	health = 100;
}

void Player::update(float elapsed) {

	//handle double jump
	if (collidedBottom) {
		canDoubleJump = true;
	}

	if (spawnDelay > -1) {
		spawnDelay--;
	}
	if (spawnDelay == 0) {
		respawn();
	}
	else {
		canDrop = false;
	}

	
	if (collidedBottom) {
		sprite.index = animStart;
	}
	else {
		sprite.index = animStart + 1;
	}

	if (scoreTextSize > 0.05f) {
		scoreTextSize -= 0.01f;
	}
	else {
		scoreTextSize = 0.05;
	}
}


void Player::setAnimation(int animStart, int animLength, float animSpeed) {
	this->animStart = animStart;
	animEnd = animStart + animLength;
	animStartOrig = animStart;
	animEndOrig = animEnd;
	this->animLength = animLength;
	this->animSpeed = animSpeed;
}

void Player::buildSpike() {

	spike.width = 0.1f;
	spike.height = 0.1f;
	spike.hasGrav = false;
	spike.isStatic = true;
	spike.isVisible = false;
	spike.enableCollisions = true;

}

void Player::placeSpike() {
	if (collidedBottom) {
		spike.x = x;
		spike.y = y;
		spike.isVisible = true;
	}
}

void Player::jump() {
	if (collidedBottom) {
		velocityY = JUMP_SPEED;
	}
	if (!collidedBottom && canDoubleJump) {
		velocityY = JUMP_SPEED - 1;
		canDoubleJump = false;
	}
}


void Player::respawn() {
	isVisible = true;

}