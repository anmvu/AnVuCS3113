#ifndef MAP
#define MAP

#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

#define TILE_SIZE 22
#define LEVEL_HEIGHT 34
#define LEVEL_WIDTH 42

class Map{
public:
	~Map();
	void readFromFile();
	bool readHeader(ifstream &fs);
	void readEntityData(ifstream &fs);
	void readLayerData(ifstream &fs);
	void placeEntity(string s, int x, int y);
	unsigned char** levelData;
	
private:
	char* levels[3];
	int level;
	int mapWidth;
	int mapHeight;
	
};


#endif