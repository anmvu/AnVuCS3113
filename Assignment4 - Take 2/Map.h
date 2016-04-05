#ifndef MAP
#define MAP

#include <stdlib.h>
#include <vector>
using namespace std;

class Map{
public:
	Map(float chance, int newWidth, int newHeight, int death, int birth, int numOfSteps);
	vector<vector<bool>> initializeMap(vector<vector<bool>> map);
	int countAliveNeighbors(vector<vector<bool>> map, int x, int y);
	vector<vector<bool>> doSimulationStep(vector<vector<bool>> oldmap);
	vector<vector<bool>> generateMap();
	float getChance();
	int getHeight();
	int getWidth();
	vector<vector<bool>> getMap();
	vector<pair<int, int>>placeTreasure(vector<vector<bool>> world);
	
private:
	float chancetoStartAlive;
	float starvationLimit;
	float overpopLimit;
	int birthLimit;
	int deathLimit;
	int numberOfSteps;
	int width;
	int height;
	vector <vector<bool>> map;
};


#endif