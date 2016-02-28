#ifndef SPACEINVADER_H
#define SPACEINVADER_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include "Entity.h"


#define MAX_PLAYER_SHOTS 15
#define MAX_ENEMIES 15
#define MAX_ENEMY_SHOTS 30

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class SpaceInvader{
public:
	SpaceInvader();
	~SpaceInvader();

	void init();
	void initGame();
	bool updateAndRender();

	void render();
	void renderTitle();
	void renderGame();
	void renderGameOver();

	void update(float elapsed);
	void updateTitle(float elapsed);
	void updateGame(float elapsed);
	void updateGameOver(float elapsed);

	void playerShoot();
	bool collision(Entity point, Entity object);
	GLuint LoadTexture(const char* image_path, GLenum image_format = GL_BGRA, GLint texParam = GL_NEAREST);
	void drawText(GLuint texture, std::string text, float size, float spacing, float r,
		float g, float b, float a, float x, float y);
	
private:
	
	bool over;
	float lastFrameTicks;
	SDL_Window* displayWindow;
	SDL_Event event;
	int state;
	const Uint8* keys;
	GLuint playerSpriteSheet;
	GLuint enemySpriteSheet;
	const char* titleFontSheetPath = "assets/fonts/pixel_font.png";
	const char* gameFontSheetPath = "assets/fonts/pixel_font.png";
	GLuint titleFontSheetTextureId;
	GLuint gameFontSheetTextureId;
	float red, green, blue;
	float redTitle, greenTitle, blueTitle;
	float redEnter, greenEnter, blueEnter, sizeEnter;
	Entity player = Entity();
	Entity enemies[MAX_ENEMIES];
	int totalEnemy;
	Entity playerShots[MAX_PLAYER_SHOTS];
	int playerShotIndex;
	float increment;
	int score;
	bool slow;
	float scoreSize = 0.1f;
	float scoreDamper;

};

#endif