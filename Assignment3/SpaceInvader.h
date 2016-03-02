#ifndef SPACEINVADER_H
#define SPACEINVADER_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include "Entity.h"


#define MAX_PLAYER_SHOTS 25
#define MAX_ENEMIES 44
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
	void pokeShoot(int i);
	bool collision(Entity point, Entity object);
	GLuint LoadTexture(const char* image_path, GLenum image_format = GL_BGRA, GLint texParam = GL_NEAREST);
	void drawText(GLuint texture, std::string text, float size, float spacing, float r,
		float g, float b, float a, float x, float y);
	void drawBackground(GLuint texture);
	
private:
	
	bool over;
	float lastFrameTicks;
	SDL_Window* displayWindow;
	SDL_Event event;
	int state;
	const Uint8* keys;
	GLuint playerSpriteSheet;
	GLuint enemySpriteSheet;
	GLuint pokeballTexture;
	GLuint backgroundTexture;
	const char* titleFontSheetPath = "assets/fonts/pixel_font.png";
	const char* gameFontSheetPath = "assets/fonts/pixel_font.png";
	GLuint titleFontSheetTextureId;
	GLuint gameFontSheetTextureId;
	float red, green, blue;
	float redTitle, greenTitle, blueTitle;
	float redEnter, greenEnter, blueEnter, sizeEnter;
	Entity player = Entity();
	Entity rareEnemy = Entity();
	Entity enemies[MAX_ENEMIES];
	int totalEnemy;
	Entity playerShots[MAX_PLAYER_SHOTS];
	Entity enemyShots[MAX_ENEMY_SHOTS];
	int playerShotIndex;
	int enemyShotIndex;
	Entity gameBackground = Entity();
	float increment;
	int score;
	bool slow;
	float scoreSize = 0.1f;

};

#endif