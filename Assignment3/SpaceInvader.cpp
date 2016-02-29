#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "SpaceInvader.h"
#include "Entity.h"

#include <vector>
#include <string>
#include <iostream>
using namespace std;

#ifndef MAX_PLAYER_SHOTS
#define MAX_PLAYER_SHOTS 15
#endif
#ifndef MAX_ENEMIES
#define MAX_ENEMIES 15
#endif
#ifndef MAX_ENEMY_SHOTS
#define MAX_ENEMY_SHOTS 30
#endif

#define PI 3.14159265

SpaceInvader::SpaceInvader(){
	init();
	state = 0;
	over = false;
	lastFrameTicks = 0.0f;
	keys = SDL_GetKeyboardState(nullptr);

	initGame();

	increment = 0.0f;

	red = 0.0f;
	green = 0.0f;
	blue = 0.0f;

	titleFontSheetTextureId = LoadTexture(titleFontSheetPath);
	gameFontSheetTextureId = LoadTexture(gameFontSheetPath);

	
	redTitle = 1.0f;
	greenTitle = 1.0f;
	blueTitle = 0.0f;
	redEnter = 1.0f;
	greenEnter = 1.0f;
	blueEnter = 1.0f;
	sizeEnter = 0.1f;
}

void SpaceInvader::initGame(){
	score = 0;
	scoreDamper = 0.0f;
	totalEnemy = MAX_ENEMIES;
	slow = true;
	const char* player_sheet = "assets/player.png";
	playerSpriteSheet = LoadTexture(player_sheet);
	player.textureId = playerSpriteSheet;
	player.animation = 1.5f;
	player.x = 0.0f;
	player.y = -0.9f;
	player.speed = 0.5f;
	player.spriteIndex = 10;

	float xEnemy = -0.6f;
	float yEnemy = 0.8f;

	const char* enemy_sheet = "assets/pokemon.png";
	enemySpriteSheet = LoadTexture(enemy_sheet);
	int enemyStartSpriteIndeces[4] = { 24,48,72,144 };
	for (int i = 0; i < MAX_ENEMIES; i++){
		enemies[i] = Entity();
		int switchX = rand() % 2;
		enemies[i].visible = true;
		enemies[i].textureId = enemySpriteSheet;
		float randomIndex = enemyStartSpriteIndeces[rand() % 4];
		enemies[i].startSprite = randomIndex;
		enemies[i].maxSprite = randomIndex + 2;
		enemies[i].spriteIndex = enemies[i].startSprite;
		enemies[i].animation = 0.1f;
		enemies[i].lives = 2.0f;
		enemies[i].yDir = -0.05f;
		enemies[i].xDir = switchX ? float(rand() % 2 + 15) / 10 : -float(rand() % 2 + 15) / 10;
		enemies[i].speed = 0.005f;
		enemies[i].x = xEnemy;
		enemies[i].y = yEnemy;
		
		if ((i + 1) % 6){
			xEnemy += 0.5f;
		}
		else{
			xEnemy = -0.6f;
			yEnemy -= 0.2f;
		}
	}

	const char* pokeball = "assets/pokeball.png";
	pokeballTexture = LoadTexture(pokeball);
	for (int i = 0; i < MAX_PLAYER_SHOTS; i++){
		playerShots[i].visible = false;
		playerShots[i].textureId = pokeballTexture;
	}

	const char* backgroundSheet = "assets/grass_tile.png";
	backgroundTexture = LoadTexture(backgroundSheet);
	gameBackground.textureId = backgroundTexture;

}

SpaceInvader::~SpaceInvader(){
	SDL_Quit();
}

void SpaceInvader::init(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pokenvaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void SpaceInvader::render(){
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (state == 0){renderTitle();}
	else if (state == 1){renderGame();}
	else if (state == 2){renderGameOver(); }

	SDL_GL_SwapWindow(displayWindow);
}

void SpaceInvader::renderTitle(){
	drawText(titleFontSheetTextureId, "Pokenvaders", 0.1f, -0.01f, redTitle, greenTitle, blueTitle, 1.0f, -1.0f, 0.3f);
	drawText(titleFontSheetTextureId, "Press Enter", sizeEnter, -0.02f, redEnter, greenEnter, blueEnter, 1.0f, -0.35f, -0.3f);
}

void SpaceInvader::renderGame(){
	gameBackground.setSize(1, 1.5);
	gameBackground.drawSprite();
	drawText(gameFontSheetTextureId, "SCORE:", 0.1f, -0.02f, 1.0f, 1.0f, 1.0f, 1.0f, -1.25f, 0.9f);
	drawText(gameFontSheetTextureId, to_string(score), scoreSize, -0.02, 1.0f, 1.0f, 1.0f, 1.0f, -0.75, 0.9f);
	//drawText(gameFontSheetTextureId, to_string(player.spriteIndex), 0.1f, -0.02f, 1.0f, 1.0f, 1.0f, 1.0f, -0.0, 0.9f);

	if (totalEnemy == 0){
		drawText(gameFontSheetTextureId, "YOU WIN!", 0.2f, -0.01f, redTitle, greenTitle, blueTitle, 1.0f, -0.6f, 0.3f);
	}

	player.drawFromSprite(3, 4);

	for (int i = 0; i < MAX_ENEMIES; i++){
		enemies[i].drawFromSprite(24,8);
	}
	
	for (int i = 0; i < MAX_PLAYER_SHOTS; i++){
		playerShots[i].setSize(0.03, 0.03);
		playerShots[i].drawSprite();
	}

	
}

void SpaceInvader::renderGameOver(){
	drawText(gameFontSheetTextureId, "GAME OVER", 0.2f, -0.01f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.3f);
	drawText(gameFontSheetTextureId, "SCORE: " + to_string(score), 0.1f, -0.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.1f);
	drawText(gameFontSheetTextureId, "Press Enter", 0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -0.4f);
}

void SpaceInvader::update(float elapsed){
	increment += 100;
	if (state == 0){ updateTitle(elapsed); }
	else if (state == 1){ updateGame(elapsed); }
	else if (state == 2){ updateGameOver(elapsed);}
}

void SpaceInvader::updateTitle(float elapsed){
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
			over = true;
		}
		else if (event.type = SDL_KEYUP){
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && !event.key.repeat){
				state = 1;
			}
		}
	}

	if (keys[SDL_SCANCODE_RETURN]){
		sizeEnter = 0.1f;
	}

	redTitle = cos(increment / 10 * PI / 180);
	greenTitle = cos(increment / 2 * PI / 180);
	blueTitle = cos(increment * PI / 180);
}

void SpaceInvader::updateGame(float elapsed){
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
			over = true;
		}
		else if (event.type = SDL_KEYDOWN){
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && !event.key.repeat){
				playerShoot();
			}
			else if (totalEnemy == 0 && event.key.keysym.scancode == SDL_SCANCODE_RETURN){
				state = 2;
			}
		}
	}

	const int playerLeft[] = { 3, 4, 5 };
	const int playerRight[] = { 6, 7, 8 };

	if (keys[SDL_SCANCODE_LEFT]){
		player.xDir = -1.0f;
		if (player.x < -1.33f) { player.x = 1.33f; }
	}
	


	else if (keys[SDL_SCANCODE_RIGHT]){
		player.xDir = 1.0f;
		if (player.x > 1.33f){ player.x = -1.33f; }
	}
	else player.xDir = 0.0f;
	
	player.move(elapsed);
	
	if (player.spriteIndex < 2){
		player.spriteIndex += 0.2f;
	}

	for (int i = 0; i < MAX_PLAYER_SHOTS; i++) playerShots[i].move(elapsed); 

	for (int i = 0; i < MAX_ENEMIES; i++){
		if (enemies[i].lives < 2 && slow){
			for (int j = 0; j < MAX_ENEMIES; j++) enemies[j].speed = 0.5f;
			slow = false;
		}

		enemies[i].move(elapsed);
		enemies[i].move(elapsed);
		enemies[i].spriteIndex += enemies[i].animation;
		if (enemies[i].spriteIndex > enemies[i].maxSprite) enemies[i].spriteIndex = enemies[i].startSprite;

		if (enemies[i].x >= 1.33f){
			enemies[i].x = 1.33f;
			enemies[i].xDir = -enemies[i].xDir;
		}
		if (enemies[i].x <= -1.33f){
			enemies[i].x = -1.33f;
			enemies[i].xDir = -enemies[i].xDir;
		}

		if (enemies[i].alive()){
			if (enemies[i].lives == 0){
				enemies[i].die();
				totalEnemy--;
			}
			else if (enemies[i].lives == 1){
				enemies[i].speed = 0.3;
				//enemies[i].animation = 0.2f;
				enemies[i].startSprite = 120;
				enemies[i].maxSprite = 122;
			}
		}
		
		if (totalEnemy == 1) enemies[i].speed = 0.8f;

		scoreDamper += 0.001f;

		for (int j = 0; j < MAX_PLAYER_SHOTS; j++){
			if (playerShots[j].visible && enemies[i].alive() && collision(playerShots[j], enemies[i])){
				enemies[i].lives -= 1.0f;
				playerShots[j].visible = false;
				scoreSize = 0.2f;
				if (enemies[i].lives == 1)score += 100 / scoreDamper;
				else if (totalEnemy == 1)score += 300 / scoreDamper;
				else score += 150 / scoreDamper;
			}
		}

		if ((collision(enemies[i],player) || (enemies[i].y == player.y)) && (enemies[i].alive()))state = 2;

	}

	if (scoreSize > 0.1f) scoreSize -= 0.01f;
	else scoreSize = 0.1f;


}

void SpaceInvader::updateGameOver(float elapsed){
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			over = true;
		}
		else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && !event.key.repeat) {
			state = 0;
			initGame();
		}
	}
}

bool SpaceInvader::updateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	update(elapsed);
	render();
	return over;
}

void SpaceInvader::playerShoot(){
	player.spriteIndex = 2;
	playerShots[playerShotIndex].visible = true;
	playerShots[playerShotIndex].x = player.x;
	playerShots[playerShotIndex].y = player.y;
	playerShots[playerShotIndex].yDir = 1.0f;
	playerShots[playerShotIndex].speed = 2.0f;
	playerShotIndex++;
	if (playerShotIndex > MAX_PLAYER_SHOTS - 1){
		playerShotIndex = 0;
	}
}

bool SpaceInvader::collision(Entity point, Entity object){
	return point.x <= (object.x + (object.width*0.5)) &&
		point.x >=(object.x - (object.width * 0.5)) &&
		point.y <= (object.y + (object.height * 0.5)) &&
		point.y >=(object.y - (object.height * 0.5));
}

GLuint SpaceInvader::LoadTexture(const char* image_path, GLenum image_format, GLint texParam) {
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

void SpaceInvader::drawText(GLuint texture, string text, float size, float spacing, float r,
	float g, float b, float a, float x, float y){
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);

	float texture_size = 1.0 / 16.0f;

	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		//colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		// a, a, a, a, a, a, a, a, a, a, a, a, a, a, a, a
		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) + (-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f * size });

		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x + texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	for (int i = 0; i < text.size(); i++){
		colorData.insert(colorData.end(), { r,g, b, a, r,g, b, a});
	}

	glColorPointer(3, GL_FLOAT, 8, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4.0f);
	glDisableClientState(GL_COLOR_ARRAY);
}
