#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

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
	#define MAX_ENEMIES 44
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
	totalEnemy = MAX_ENEMIES;
	const char* player_sheet = "assets/player.png";
	playerSpriteSheet = LoadTexture(player_sheet);
	player.textureId = playerSpriteSheet;
	player.animation = 0.04f;
	player.x = 0.0f;
	player.y = -0.9f;
	player.speed = 0.5f;
	player.spriteIndex = 10;

	float xEnemy = -0.6f;
	float yEnemy = 0.8f;

	int start = 10;
	int max = 12;

	const char* enemy_sheet = "assets/pokemon.png";
	enemySpriteSheet = LoadTexture(enemy_sheet);
	for (int i = 0; i < MAX_ENEMIES; i++){
		enemies[i] = Entity();
		enemies[i].visible = true;
		enemies[i].textureId = enemySpriteSheet;
		enemies[i].startSprite = start;
		enemies[i].maxSprite = max;
		enemies[i].spriteIndex = enemies[i].startSprite;
		enemies[i].animation = 0.03f;
		enemies[i].lives = 2.0f;
		enemies[i].yDir = -enemies[i].height;
		enemies[i].xDir = 1.5f;
		enemies[i].speed = 0.2f;
		enemies[i].x = xEnemy;
		enemies[i].y = yEnemy;
		
		xEnemy += 0.2f;

		if ((i + 1) % 11 == 0 ){
			yEnemy -= 0.15f;
			xEnemy = -0.6f;
		}
		if ((i + 1) % 5 == 0 && start!= 82){
			start += 24;
			max += 24;
		}
		else if (start == 82){
			start = 10;
			max = 12;
		}
	}

	rareEnemy.textureId = LoadTexture(enemy_sheet);
	rareEnemy.visible = false;
	rareEnemy.lives = 1;
	rareEnemy.y = 0.85;
	rareEnemy.x = -1.33f;
	rareEnemy.xDir = 1.0f;
	rareEnemy.animation = 0.04f;
	rareEnemy.speed = 1.5f;
	rareEnemy.spriteIndex = 168;
	rareEnemy.startSprite = 168;
	rareEnemy.maxSprite = 191;

	const char* pokeball = "assets/pokeball.png";
	pokeballTexture = LoadTexture(pokeball);
	for (int i = 0; i < MAX_PLAYER_SHOTS; i++){
		playerShots[i].visible = false;
		playerShots[i].textureId = pokeballTexture;
	}

	for (int i = 0; i < MAX_ENEMY_SHOTS; i++){
		enemyShots[i].visible = false;
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

	#ifdef _WINDOWS
		glewInit();
	#endif

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void SpaceInvader::render(){
	glClearColor(red,green,blue, 1.0f);
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

	if (totalEnemy == 0){
		drawText(gameFontSheetTextureId, "YOU WIN!", 0.2f, -0.01f, redTitle, greenTitle, blueTitle, 1.0f, -0.6f, 0.3f);
		drawText(titleFontSheetTextureId, "Press Enter", sizeEnter, -0.02f, redEnter, greenEnter, blueEnter, 1.0f, -0.35f, -0.3f);
	}

	player.drawFromSprite(3,4);

	rareEnemy.drawFromSprite(24, 8);

	for (int i = 0; i < MAX_ENEMIES; i++){
		//drawText(gameFontSheetTextureId, to_string(i), scoreSize, -0.02, 1.0f, 1.0f, 1.0f, 1.0f, -0.75f + i*0.1,0.7f);
		enemies[i].drawFromSprite(24,8);
	}
	
	for (int i = 0; i < MAX_PLAYER_SHOTS; i++){
		playerShots[i].setSize(0.03, 0.03);
		playerShots[i].drawSprite();
	}

	for (int i = 0; i < MAX_ENEMY_SHOTS; i++){
		enemyShots[i].setSize(0.01, 0.01);
		enemyShots[i].drawSprite();
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

			//meh I tried
			else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_RIGHT && !event.key.repeat){
				player.spriteIndex += player.animation;
				if (player.spriteIndex > player.maxSprite) player.spriteIndex = player.startSprite;
			}
			else if (totalEnemy == 0 && event.key.keysym.scancode == SDL_SCANCODE_RETURN){
				state = 0;
				initGame();
			}
		}
	}

	const int playerLeft[] = { 3, 4, 5 };
	const int playerRight[] = { 6, 7, 8 };

	if (keys[SDL_SCANCODE_LEFT]){
		player.xDir = -1.0f;
		if (player.x < -1.33f) { player.x = 1.33f; }
		player.startSprite = playerLeft[0];
		player.maxSprite = playerLeft[2];
		player.spriteIndex = player.startSprite;
	}
	
	else if (keys[SDL_SCANCODE_RIGHT]){
		player.xDir = 1.0f;
		if (player.x > 1.33f){ player.x = -1.33f; }
		player.startSprite = playerRight[0];
		player.maxSprite = playerRight[2];
		player.spriteIndex = player.startSprite;
	}
	else{ player.xDir = 0.0f; }
	//wanted to animate but it makes it doesn't really work and doesn't work as cool
	/*player.spriteIndex += player.animation;
	if (player.spriteIndex > player.maxSprite) player.spriteIndex = player.startSprite;*/
	player.move(elapsed);
	

	for (int i = 0; i < MAX_PLAYER_SHOTS; i++) playerShots[i].move(elapsed); 

	for (int i = 0; i < MAX_ENEMY_SHOTS; i++) enemyShots[i].move(elapsed);

	rareEnemy.spriteIndex += rareEnemy.animation;
	if (rareEnemy.spriteIndex > rareEnemy.maxSprite) rareEnemy.spriteIndex = rareEnemy.startSprite;
	
	//merr... how do I make the extra points guy mooove?
	if (!rareEnemy.alive()){
		if (rand() % 419 == 0){
			rareEnemy.visible = true;
			if(rareEnemy.lives == 1){
				rareEnemy.xDir = 1.0f;
				rareEnemy.speed = 1.5f;
				if (rareEnemy.x >= 1.33){
					rareEnemy.x = -1.33;
					rareEnemy.visible = false;
				}
				
			}
			else if (rareEnemy.lives == 0){
				rareEnemy.x = -1.33;
				rareEnemy.visible = false;
				rareEnemy.move(elapsed);
			}
		}

	}

	rareEnemy.move(elapsed);
	

	for (int i = 0; i < MAX_ENEMIES; i++){

		enemies[i].move(elapsed);
		//enemies[i].move(elapsed);
		enemies[i].spriteIndex += enemies[i].animation;
		if (enemies[i].spriteIndex > enemies[i].maxSprite) enemies[i].spriteIndex = enemies[i].startSprite;

		/*if ((i == 10 || (i+1)%11 == 0) && enemies[i].x >= 1.33f){*/
		if (enemies[i].x >= 1.33f){
			enemies[i].x = 1.31;
			enemies[i].startSprite -= 3;
			enemies[i].maxSprite -= 3;
			enemies[i].xDir = -enemies[i].xDir;
			enemies[i].y -= 0.02;
			/*
				tried moving by row. didn't work out
				for (int k = 0; k < 11; k++){
				int index = i - k;
				enemies[index].x = 1.33 - (0.2*k);
				enemies[index].startSprite -= 3;
				enemies[index].maxSprite -= 3;
				enemies[index].xDir = -enemies[index].xDir;
				enemies[index].y = yChange;
			}*/
		}
		//if ((i == 0 || i % 11 == 0) && enemies[i].x <= -1.33f){
		if (enemies[i].x <= -1.33f){
			enemies[i].x = -1.31;
			enemies[i].startSprite += 3;
			enemies[i].maxSprite += 3;
			enemies[i].xDir = -enemies[i].xDir;
			enemies[i].y -= 0.02;
			/*for (int k = 0; k < 11; k++){
				int index = i + k;
				enemies[index].x = -1.33 + (0.2*k);
				enemies[index].startSprite += 3;
				enemies[index].maxSprite += 3;
				enemies[index].xDir = -enemies[index].xDir;
				enemies[index].y = yChange;
			}*/
		}

		if (enemies[i].alive()){
			if (enemies[i].lives == 0){
				enemies[i].die();
				totalEnemy--;
			}
			else if (enemies[i].lives == 1){
				enemies[i].animation = 0.15f;
			}
			int randomInt = rand() + 1;
			int rand2 = rand() + 7;
			if (randomInt % rand2 == 0) {
				pokeShoot(i);
			}
			
		}
		
		if (totalEnemy == 1) enemies[i].speed = 0.8f;

		for (int j = 0; j < MAX_PLAYER_SHOTS; j++){
			if (playerShots[j].visible && enemies[i].alive() && collision(playerShots[j], enemies[i])){
				enemies[i].lives -= 1.0f;

				playerShots[j].visible = false;
				if (enemies[i].lives == 1)score += 100;
				else if (totalEnemy == 1)score += 300;
				else score += 150;
			}
			if (playerShots[j].visible && rareEnemy.alive() && rareEnemy.lives == 1 && collision(playerShots[j], rareEnemy)){
				playerShots[j].visible = false;
				rareEnemy.die();
				rareEnemy.lives--;
				score += 300;
			}
		}

		for (int k = 0; k < MAX_ENEMY_SHOTS; k++){
			if (enemyShots[k].visible && collision(enemyShots[k], player)){
				enemyShots[k].visible = false;
				state = 2;
			}
		}

		if ((collision(enemies[i],player) || (enemies[i].y <= player.y)) && (enemies[i].alive()))state = 2;

	}

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
	player.spriteIndex = 10;
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

void SpaceInvader::pokeShoot(int i){
	enemyShots[enemyShotIndex].visible = true;
	enemyShots[enemyShotIndex].x = enemies[i].x;
	enemyShots[enemyShotIndex].y = enemies[i].y;
	enemyShots[enemyShotIndex].yDir = -1.0f;
	enemyShots[enemyShotIndex].speed = 2.0f;
	enemyShotIndex++;
	if (enemyShotIndex > MAX_ENEMY_SHOTS - 1){
		enemyShotIndex = 0;
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

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

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
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
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
		colorData.insert(colorData.end(), { r,g, b, a,r,g,b,a});
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
