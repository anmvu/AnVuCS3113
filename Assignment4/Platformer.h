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
using namespace std;



#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define GRAVITY 0
#define COLLISION_OFFSET 0.000001f;
#define TILE_SIZE 0.2f
#define LEVEL_HEIGHT 34
#define LEVEL_WIDTH 42
#define SPRITE_COUNT_X 30
#define SPRITE_COUNT_Y 16

class PlatformerGame{
public:
public:
	PlatformerGame();
	~PlatformerGame();

	void Init();
	void BuildLevel();
	bool UpdateAndRender();

	void Render();
	void RenderLevel();
	void Update(float elapsed);
	void FixedUpdate();

	//tile map methods
	bool readHeader(std::ifstream& stream);
	bool readLayerData(std::ifstream& stream);
	bool readEntityData(std::ifstream& stream);
	void placeEntity(std::string type, float placeX, float placeY);
	bool isSolid(unsigned char tile);

	//collision related
	void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);
	float checkPointCollisionX(float x, float y);
	float checkPointCollisionY(float x, float y);
	void handleCollisionX(Entity* entity);
	void handleCollisionY(Entity* entity); 
	GLuint LoadTexture(const char* image_path, GLenum image_format = GL_RGB, GLint texParam = GL_NEAREST);
	void DrawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);
	float lerp(float v0, float v1, float t);

private:
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;
	float timeLeftOver = 0.0f;
	const Uint8* keys;

	unsigned char** levelData;
	int mapWidth;
	int mapHeight;
	float tileWidth = 0.1;
	float tileHeight = 0.1;
	int tileCountX = 8;
	int tileCountY = 3;
	GLuint tilesTextureId;

	std::vector<Entity*> entities;
	Entity plr;

};

#endif