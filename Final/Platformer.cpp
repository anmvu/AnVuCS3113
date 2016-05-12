#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include "Platformer.h"
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

#include "Matrix.h"
#include "ShaderProgram.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

float PlatformerGame::randRange(float min, float max) {
	return (float(rand()) / float(RAND_MAX) * (max - min)) + min;
}

PlatformerGame::PlatformerGame(ShaderProgram& theProgram){
	init();
	done = false;
	lastFrameTicks = 0.0f;
	timeLeft = 0.0f;
	keys = SDL_GetKeyboardState(nullptr);
	state = TITLE_SCREEN;
	level = 1;

	
	program = theProgram;

	currMenuSel = 0;

	build();

	void playerStuff(SDL_Event& event, Player& plr);

	bool readHeader(std::ifstream& stream);
	bool readLayerData(std::ifstream& stream, const std::string& tileLayer);
	bool readEntityData(std::ifstream& stream);
	void placeEntity(std::string type, float x, float y);
	
	void playerOnSpike();
	void playerOnDiamond();
	void playerOnLadder();
	void tileCoordinates(float worldX, float worldY, int* gridX, int* gridY);
	float checkCollisionX(float x, float y);
	float checkCollisionY(float x, float y);
	void handleCollisionX(Entity*entity);
	void handleCollisionY(Entity* entity);

}

PlatformerGame::~PlatformerGame(){
	SDL_Quit();
}

void PlatformerGame::init(){
	
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Fight for the Diamonds!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 900, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 1200, 900);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void PlatformerGame::build(){
	spriteSheetTextureID = LoadTexture("assets/alien_sheet.png");
	sheetCountX = 30;
	sheetCountY = 16;
	fontSheetTextureID = LoadTexture("assets/font1.png");
}

void PlatformerGame::buildLevel(int level){
	reset();
	string file = "assets/level" + level;
	file += ".txt";
	buildMap(file);
	spawn();
}

void PlatformerGame::reset(){
	player1.setAnimation(0, 5, 0.8f);
	player1.sprite = SheetSprite(spriteSheetTextureID, player1.start, sheetCountX, sheetCountY);
	player1.width = 0.1f;
	player1.height = 0.1f;
	player1.isStatic = false;
	player1.isVisible = true;
	player1.enableCollisions = true;
	player1.velocityX = 0.0f;
	player1.velocityY = 0.0f;
	player1.accelerationX = 0.0f;
	player1.accelerationY = 0.0f;
	player1.frictionX = 5.0f;
	player1.frictionY = 4.0f;
	entities.push_back(&player1);

	player2.setAnimation(0, 5, 0.8f);
	player2.sprite = SheetSprite(spriteSheetTextureID, player2.start, sheetCountX, sheetCountY);
	player2.width = 0.1f;
	player2.height = 0.1f;
	player2.isStatic = false;
	player2.isVisible = true;
	player2.enableCollisions = true;
	player2.velocityX = 0.0f;
	player2.velocityY = 0.0f;
	player2.accelerationX = 0.0f;
	player2.accelerationY = 0.0f;
	player2.frictionX = 5.0f;
	player2.frictionY = 4.0f;
	entities.push_back(&player2);
}

void PlatformerGame::buildMap(const string& tileMapPath){
	ifstream infile(tileMapPath);
	string line;

	while (getline(infile, line)){
		if (line == "[header]"){
			if (!readHeader(infile)){
				return;
			}
		}
		else if (line == "[layer]"){
			readLayerData(infile, "1");
		}
		else{
			readEntityData(infile);
		}
	}
}

void PlatformerGame::spawn(){
	player1.respawn();
	player2.respawn();
}

void PlatformerGame::render(){
	switch (state){
	case TITLE_SCREEN:
		renderTitle();
		break;
	case LEVEL_1:
		renderLevel();
		break;
	case LEVEL_2:
		renderLevel();
		break;
	case LEVEL_3:
		renderLevel();
		break;
	case GAME_OVER:
		renderGameOver();
		break;
	case YOU_WIN:
		renderWin();
		break;
	}

	SDL_GL_SwapWindow(displayWindow);
}

void PlatformerGame::renderTitle(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	DrawText(program,fontSheetTextureID, "GET THE DIAMONDS", 0.08f, 0.0f);
	DrawText(program,fontSheetTextureID, "1_PLAYER", 0.04f, 0.0f);
	DrawText(program,fontSheetTextureID, "2-PLAYER", 0.04f, 0.0f);
	DrawText(program,fontSheetTextureID, "QUIT", 0.04f, 0.0f);
	
}

void PlatformerGame::renderLevel(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (size_t i = 0; i < entities.size(); i++){
		entities[i]->render();
	}

	renderLayout();

	DrawText(program, fontSheetTextureID, "Player 1", 0.08f, 0.0f);
	DrawText(program, fontSheetTextureID, to_string(player1.score), 0.08f, 0.0f);
	DrawText(program, fontSheetTextureID, "Player 2", 0.08f, 0.0f);
	DrawText(program, fontSheetTextureID, to_string(player2.score), 0.08f, 0.0f);
}

void PlatformerGame::renderLayout(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spriteSheetTextureID);

	vector<float> vertexData;
	vector<float> texCoordData;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			float u = (float)(((int)levelData[y][x] - 1) % sheetCountX) / (float)sheetCountX; // - 1 to account for Tiled's index strangeness
			float v = (float)(((int)levelData[y][x] - 1) / sheetCountX) / (float)sheetCountY;
			float width = 1.0f / (float)sheetCountX;
			float height = 1.0f / (float)sheetCountY;

			if (levelData[y][x] != 0) {
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});

				texCoordData.insert(texCoordData.end(), {
					u + UV_OFFSET, v + UV_OFFSET,
					u + UV_OFFSET, v + height - UV_OFFSET,
					u + width - UV_OFFSET, v + height - UV_OFFSET,
					u + width - UV_OFFSET, v + UV_OFFSET
				});
			}
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, (GLsizei)vertexData.size() / 2);
	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}

void PlatformerGame::renderGameOver(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	DrawText(program, fontSheetTextureID, "GAME OVER", 0.08f, 0.0f);
	DrawText(program, fontSheetTextureID, "WOMP WOMP WOMP", 0.08f, 0.0f);
}

void PlatformerGame::renderWin(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	DrawText(program, fontSheetTextureID, "YOU WIN", 0.08f, 0.0f);
	DrawText(program, fontSheetTextureID, "CONGRATS ON WORKING TOGETHER", 0.08f, 0.0f);
}

void PlatformerGame::updateTitle(float elapsed){
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
			done = true;
		}
		else if (event.type == SDL_KEYDOWN){
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_DOWN:
				++currMenuSel;
				if (currMenuSel > TITLE_MENU_ITEM_COUNT - 1) {
					currMenuSel = 0;
				}
				break;
			case SDL_SCANCODE_UP:
				--currMenuSel;
				if (currMenuSel < 0) {
					currMenuSel = TITLE_MENU_ITEM_COUNT - 1;
				}
				break;
			case SDL_SCANCODE_RETURN:
				switch (currMenuSel) {
				case 0:
					buildLevel(level);
					state = LEVEL_1;
					break;
				case 1:
					buildLevel(level);
					state = LEVEL_1;
					break;
				case 2:
					done = true;
				default:
					break;
				}
			default:
				break;
			}
		}
	}
}

void PlatformerGame::updateGame(float elapsed){

	updateInput();

	for (int i = 0; i < entities.size(); i++) {
		entities[i]->update(elapsed);
	}

	//playerOnDiamond();
	//playerOnLadder();
	playerOnSpike();
	playerOnPlayer();

	if (player1.health == 0 || player2.health == 0){
		state = GAME_OVER;
	}
}

void PlatformerGame::updateGameOver(float elapsed){
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				state = TITLE_SCREEN;
			}
		}
	}

	entities.erase(entities.begin(), entities.end());
}

void PlatformerGame::update(float elapsed){
	switch (state){
	case TITLE_SCREEN:
		updateTitle(elapsed);
		break;
	case LEVEL_1:
		updateGame(elapsed);
		break;
	case LEVEL_2:
		updateGame(elapsed);
		break;
	case LEVEL_3:
		updateGame(elapsed);
		break;
	case GAME_OVER:
		updateGameOver(elapsed);
		break;
	}
}

void PlatformerGame::updateInput(){
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				entities.erase(entities.begin(), entities.end());
				state = TITLE_SCREEN;
			}
			if (!event.key.repeat) {
				if (player1.isVisible) {
					if (event.key.keysym.scancode == SDL_SCANCODE_W) {
						player1.jump();
					}
					if ((event.key.keysym.scancode == SDL_SCANCODE_D || event.key.keysym.scancode == SDL_SCANCODE_A) && player1.collidedBottom) {
						player1.sprite.index = player1.animStart + 1;
					}
				}
				if (player2.isVisible) {
					if (event.key.keysym.scancode == SDL_SCANCODE_UP) {
						player2.jump();
					}
					if ((event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_LEFT) && player1.collidedBottom) {
						player2.sprite.index = player2.animStart + 1;
					}
				}
			}
		}
		if (player1.isVisible) {
			if (keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A]) {
				player1.accelerationX = 10.0f;
			}
			else if (keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) {
				player1.accelerationX = -10.0f;
			}
			else {
				player1.accelerationX = 0.0f;
			}
		}

		if (player2.isVisible) {
			if (keys[SDL_SCANCODE_RIGHT] && !keys[SDL_SCANCODE_LEFT]) {
				player2.accelerationX = 10.0f;
			}
			else if (keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_RIGHT]) {
				player2.accelerationX = -10.0f;
			}
			else {
				player2.accelerationX = 0.0f;
			}
		}
	}
}

void PlatformerGame::fixedUpdate(){
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->fixedUpdate();

		entities[i]->collidedTop = false;
		entities[i]->collidedBottom = false;
		entities[i]->collidedLeft = false;
		entities[i]->collidedRight = false;

		if (entities[i]->hasGrav) {
			entities[i]->velocityY += GRAVITY * FIXED_TIMESTEP;
		}
	}

	// handle Y collisions
	for (int i = 0; i < entities.size(); i++) {
		if (!entities[i]->isStatic) {
			entities[i]->y += entities[i]->velocityY * FIXED_TIMESTEP;
			if (entities[i]->enableCollisions) {
				handleCollisionY(entities[i]);
			}
		}
	}

	// handle X collisions
	for (int i = 0; i < entities.size(); i++) {
		if (!entities[i]->isStatic) {
			entities[i]->x += entities[i]->velocityX * FIXED_TIMESTEP;
			if (entities[i]->enableCollisions) {
				handleCollisionX(entities[i]);
			}
		}
	}
}

bool PlatformerGame::updateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeft;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		fixedUpdate();
	}
	timeLeft = fixedElapsed;

	update(elapsed);
	render();
	return done;
}

bool PlatformerGame::readHeader(ifstream& stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;

	while (getline(stream, line)) {

		if (line == "") {
			break;
		}

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height") {
			mapHeight = atoi(value.c_str());
		}
		else if (key == "tilewidth") {
			tileWidth = atoi(value.c_str());
		}
		else if (key == "tileheight") {
			tileHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else {
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; i++) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool PlatformerGame::readLayerData(std::ifstream& stream, const string& tileLayer) {
	string line;

	while (getline(stream, line)) {
		if (line == "") {
			break;
		}

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (value == tileLayer) {
			getline(stream, line); //skip 'data=' line
			//        }
			//        if (key == "data") {
			for (int y = 0; y < mapHeight; y++) { //get rows
				getline(stream, line);
				istringstream lineStream(line);
				string tile;

				for (int x = 0; x < mapWidth; x++) { //get row values
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						//flare map file tiles are indexed from 1 (not 0)
						if (val == 126) { //126 = crate sprite tile
							switch (rand() % 2) {
							case 0:
								levelData[y][x] = randRange(val, val + 3);
								break;
							case 1:
								levelData[y][x] = randRange(val + 16, val + 16 + 3);
								break;
							}
						}
						else {
							levelData[y][x] = val;
						}
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}

bool PlatformerGame::readEntityData(std::ifstream& stream) {

	string line;
	string type;

	while (getline(stream, line)) {
		if (line == "") {
			break;
		}

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');

			float placeX = atoi(xPosition.c_str()) / tileWidth * TILE_SIZE + (TILE_SIZE / 2);
			float placeY = atoi(yPosition.c_str()) / tileHeight * -TILE_SIZE + (TILE_SIZE / 2);

		}
	}
	return true;
}

void PlatformerGame::playerOnPlayer() {
	if (player1.collidesWith(&player2) && player1.isVisible && player2.isVisible) {
		if (player1.x < player2.x) {
			player1.velocityX = -2.0f;
			player2.velocityX = 2.0f;
		}
		else if (player2.x < player1.x) {
			player1.velocityX = 2.0f;
			player2.velocityX = -2.0f;
		}

		if (player1.y > player2.y) {
			player1.velocityY = 2.0f;
		}
		else if (player2.y > player1.y) {
			player2.velocityY = 2.0f;
		}
	}
}

//void PlatformerGame::playerOnDiamond() {
//	if (player1.collidesWith(&diamond) && player1.isVisible) {
//		player1.score += MIN_SCORE;
//		player1.scoreTextSize *= 3;
//	}
//	if (player2.collidesWith(&diamond) && player2.isVisible) {
//		player1.score += MIN_SCORE;
//		player1.scoreTextSize *= 3;
//	}
//}

void PlatformerGame::playerOnSpike() {
	if (player1.collidesWithCenter(&player2.spike) && player2.spike.isVisible) {
		player2.score += SPIKE_SCORE;
		player2.scoreTextSize *= 3;
		player2.spike.isVisible = false;
	}
	if (player2.collidesWithCenter(&player1.spike) && player1.spike.isVisible) {
		player1.score += SPIKE_SCORE;
		player1.scoreTextSize *= 3;
		player1.spike.isVisible = false;
	}
}

void PlatformerGame::worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY) {
	*gridX = (int)(worldX / TILE_SIZE);
	*gridY = (int)(-worldY / TILE_SIZE);
}

float PlatformerGame::checkCollisionX(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);

	if (gridX < 0 || gridX > mapWidth - 1 || gridY < 0 || gridY > mapHeight - 1) {
		return 0.0f;
	}


	return 0.0f;
}

float PlatformerGame::checkCollisionY(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);

	if (gridX < 0 || gridX > mapWidth - 1 || gridY < 0 || gridY > mapHeight - 1) {
		return 0.0f;
	}


	return 0.0f;
}

void PlatformerGame::handleCollisionX(Entity* entity) {

	//left
	float penetration = checkCollisionX(entity->x - (entity->width * entity->xScale * 0.5), entity->y);
	if (penetration != 0.0f) {
		entity->x += penetration / COLLISION_DAMP;
		entity->velocityX = 0.0f;
		entity->collidedLeft = true;
	}

	//right
	penetration = checkCollisionX(entity->x + (entity->width * entity->xScale * 0.5), entity->y);
	if (penetration != 0.0f) {
		entity->x += (penetration - TILE_SIZE) / COLLISION_DAMP;
		entity->velocityX = 0.0f;
		entity->collidedRight = true;
	}
}

void PlatformerGame::handleCollisionY(Entity* entity) {

	//    //bottom
	//    float penetration = checkCollisionY(entity->x, entity->y - (entity->height * entity->yScale * 0.5));
	//    if (penetration != 0.0f) {
	//        entity->y += penetration;
	//        entity->velocityY = 0.0f;
	//        entity->collidedBottom = true;
	//    }
	//    
	//    //top
	//    penetration = checkCollisionY(entity->x, entity->y + (entity->height * entity->yScale * 0.5));
	//    if (penetration != 0.0f) {
	//        entity->y += (penetration - TILE_SIZE);
	//        entity->velocityY = 0.0f;
	//        entity->collidedTop = true;
	//    }

	//bottom right
	float penetration = checkCollisionY(entity->x + (entity->width * 0.3), entity->y - (entity->height * entity->yScale * 0.5));
	if (penetration != 0.0f) {
		entity->y += penetration;
		entity->velocityY = 0.0f;
		entity->collidedBottom = true;
	}
	//bottom left
	penetration = checkCollisionY(entity->x - (entity->width * 0.3), entity->y - (entity->height * entity->yScale * 0.5));
	if (penetration != 0.0f) {
		entity->y += penetration;
		entity->velocityY = 0.0f;
		entity->collidedBottom = true;
	}
	//top right
	penetration = checkCollisionY(entity->x + (entity->width * 0.3), entity->y + (entity->height * entity->yScale * 0.5));
	if (penetration != 0.0f) {
		entity->y += (penetration - TILE_SIZE);
		entity->velocityY = 0.0f;
		entity->collidedTop = true;
	}
	//top left
	penetration = checkCollisionY(entity->x - (entity->width * 0.3), entity->y + (entity->height * entity->yScale * 0.5));
	if (penetration != 0.0f) {
		entity->y += (penetration - TILE_SIZE);
		entity->velocityY = 0.0f;
		entity->collidedTop = true;
	}
}

GLuint PlatformerGame::LoadTexture(const char* image_path, GLenum image_format, GLint texParam) {

	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, image_format, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParam);

	SDL_FreeSurface(surface);

	return textureID;
}

void PlatformerGame::DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing){
	float texture_size = 1.0 / 16.0f;     std::vector<float> vertexData;     std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		float textureX = (float)(((int)text[i]) % 16) / 16.0f;
		float textureY = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(),
		{ ((size + spacing) * i) + (-0.5f * size), 0.5f * size,
		((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		((size + spacing) * i) + (0.5f * size), 0.5f * size,
		((size + spacing) * i) + (0.5f * size), -0.5f * size,
		((size + spacing) * i) + (0.5f * size), 0.5f * size,
		((size + spacing) * i) + (-0.5f * size), -0.5f * size, });

		texCoordData.insert(texCoordData.end(),
		{ textureX, textureY, textureX, textureY + texture_size, textureX + texture_size,
		textureY, textureX + texture_size, textureY + texture_size, textureX + texture_size,
		textureY, textureX, textureY + texture_size, });
	}
	glUseProgram(program.programID);
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program.texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);

}

