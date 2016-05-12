#ifndef Platformer
#define Platformer

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "Entity.h"
#include "Player.h"
using namespace std;



#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define GRAVITY 0
#define COLLISION_OFFSET 0.000001f
#define TILE_SIZE 0.22f
#define LEVEL_HEIGHT 34
#define LEVEL_WIDTH 42
#define SPRITE_COUNT_X 30
#define SPRITE_COUNT_Y 16
#define TITLE_MENU_ITEM_COUNT 3
#define UV_OFFSET 0.001f
#define MIN_SCORE 10
#define SPIKE_SCORE 50
#define COLLISION_DAMP 1.1

class PlatformerGame{
public:
	PlatformerGame(ShaderProgram& program);
	~PlatformerGame();
	void init();
	void initGame();
	bool updateAndRender();
	
	void build();
	float randRange(float min, float max);
	void buildLevel(int level);
	void reset();
	void buildMap(const std::string& tileMap);
	void spawn();

	void render();
	void renderTitle();
	void renderGameOver();
	void renderWin();
	void renderLevel();
	void renderLayout();

	void update(float elapsed);
	void updateTitle(float elapsed);
	void updateGame(float elapsed);
	void updateGameOver(float elapsed);
	void updateWin(float elapsed);
	void updateInput();
	void fixedUpdate();

	void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);

	float checkCollisionX(float x, float y);
	float checkCollisionY(float x, float y);
	void handleCollisionX(Entity* entity);
	void handleCollisionY(Entity* entity);

	bool readHeader(std::ifstream& stream);
	bool readLayerData(std::ifstream& stream, const std::string& tileLayer);
	bool readEntityData(std::ifstream& stream);
	void placeEntity(std::string type, float placeX, float placeY);

	void playerOnPlayer();
	void playerOnSpike();
	void playerOnLadder();
	void playerOnDiamond();

	void dropSpike();


	GLuint LoadTexture(const char* imagepath, GLenum image_format = GL_BGRA, GLint texParam = GL_LINEAR);
	void DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing);

private:
	ShaderProgram program;
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;
	float timeLeft;
	const Uint8* keys;
	SDL_Event event;
	enum state{ TITLE_SCREEN, LEVEL_1, LEVEL_2, LEVEL_3, GAME_OVER, YOU_WIN, YOU_LOSE };
	int state;

	GLuint spriteSheetTextureID;
	GLuint fontSheetTextureID;
	
	unsigned char** levelData;
	int mapWidth;
	int mapHeight;
	float tileWidth;
	float tileHeight;

	float scale;
	float x;
	float y;
	float moveX;
	float moveY;

	int level;

	std::vector<Entity*> entities;
	Player player1;
	Player player2;
	int timer;

	int sheetCountX;
	int sheetCountY;
	int currMenuSel;


};

#endif