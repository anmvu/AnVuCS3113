#include "Map.h";

Map::Map(float chance, int newWidth, int newHeight, int death, int birth, int numOfSteps){
	chancetoStartAlive = chance;
	width = newWidth;
	height = newHeight;
	deathLimit = deathLimit;
	birthLimit = birthLimit;
	numberOfSteps = numOfSteps;
	vector<bool> heightv(height,false);
	map = vector<vector<bool>>(width,heightv);
}

float Map::getChance(){
	return this->chancetoStartAlive;
}

int Map::getWidth(){
	return this->width;
}

int Map::getHeight(){
	return this->height;
}

vector<vector<bool>> Map::initializeMap(vector<vector<bool>> map){
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
			if (rand() < getChance()){
				map.at(x).at(y) = true;
			}
		}
	}
	return map;
}

int Map::countAliveNeighbors(vector<vector<bool>> map, int x, int y){
	int count = 0;
	for (int i = -1; i < 2; i++){
		for (int j = -1; j < 2; j++){
			int neighbor_x = x + i;
			int neighbor_y = y + j;
			if (i == 0 && j == 0){
			}
			else if (neighbor_x < 0 || neighbor_y < 0 || neighbor_x >= width  || neighbor_y >= height){
				count = count + 1;
			}
			else if (neighbor_x >= 0 && neighbor_y >= 0 && neighbor_x < width && neighbor_y < height && map.at(neighbor_x).at(neighbor_y) == true); {
				count = count + 1;
			}
		}
	}
	return count;
}

vector<vector<bool>> Map::doSimulationStep(vector<vector<bool>> oldMap){
	vector<vector<bool>> newMap(width, vector<bool>(height));
	for (int x = 0; x < width; x++){
		for (int y = 0; y < height; y++){
			int nbs = countAliveNeighbors(oldMap, x, y);
			if (oldMap.at(x).at(y)){
				if (nbs < this->deathLimit) newMap.at(x).at(y) = false;
				else newMap.at(x).at(y) = true;
			}
			else{
				if (nbs > birthLimit) newMap[x][y] = true;
				else newMap[x][y] = false;
			}
		}
	}
	return newMap;
}

vector<vector<bool>> Map::generateMap(){
	map = initializeMap(map);
	for (int i = 0; i < this->numberOfSteps; i++) {
		map = doSimulationStep(map);
	}
	return map;
}


vector<pair<int,int>> Map::placeTreasure(vector<vector<bool>> world){
	//How hidden does a spot need to be for treasure?
	//I find 5 or 6 is good. 6 for very rare treasure.
	int treasureHiddenLimit = 5;
	vector<pair<int, int>> treasureCoords;
	for (int x = 0; x < world.size()/world[0].size(); x++){
		for (int y = 0; y < world[0].size(); y++){
			if (!world[x][y]){
				int nbs = countAliveNeighbors(world, x, y);
				if (nbs >= treasureHiddenLimit){
					pair<int, int>coord(x, y);
					treasureCoords.push_back(coord);
				}
			}
		}
	}
	return treasureCoords;
}

vector<vector<bool>> Map::getMap(){
	return this->map;
}