#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include "Platformer.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Entity.h"
using namespace std;


PlatformerGame::PlatformerGame(){
	init();
	done = false;
	lastFrameTicks = 0.0f;
	keys = SDL_GetKeyboardState(nullptr);
	buildLevel();
}

PlatformerGame::~PlatformerGame(){
	SDL_Quit();
}
 
GLuint PlatformerGame::loadTexture(const char* image_path, GLenum image_format, GLint texParam) {
	SDL_Surface *surface = IMG_Load("assets/alien_sheet.png");

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, image_format, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texParam);

	SDL_FreeSurface(surface);

	return textureID;
}

void PlatformerGame::drawText(GLuint texture, string text, float size, float spacing, float r,
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
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a });
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

void PlatformerGame::handleCollisionX(Entity* entity, vector<Entity*> entities){
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i] != entity && entities[i]->isStatic && entity->collides(entities[i])) {

			float x_penetration = fabs(entity->x - entities[i]->x) - (entity->width * 0.5) - (entities[i]->width * 0.5) - COLLISION_OFFSET; 

			//left
			if (entity->x < entities[i]->x) {
				entity->x += x_penetration;
				entity->velocity_x = 0.0f;
				entity->collidedLeft = true;
			}
			//right
			if (entity->x > entities[i]->x) {
				entity->x -= x_penetration;
				entity->velocity_x = 0.0f;
				entity->collidedRight = true;
			}
		}
	}
}

void PlatformerGame::handleCollisionY(Entity* entity, vector<Entity*> entities){
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i] != entity && entities[i]->isStatic && entity->collides(entities[i])) {

			float y_penetration = fabs(entity->y - entities[i]->y) - (entity->height * 0.5) - (entities[i]->height * 0.5) - COLLISION_OFFSET;

			//top
			if (entity->y < entities[i]->y) {
				entity->y += y_penetration;
				entity->velocity_y = 0.0f;
				entity->collidedTop = true;
			}
			//bottom
			if (entity->y > entities[i]->y) {
				entity->y -= y_penetration;
				entity->velocity_y = 0.0f;
				entity->collidedBottom = true;
			}
		}
	}
}


void PlatformerGame::init(){
	SDL_Init(SDL_INIT_VIDEO);
	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	displayWindow = SDL_CreateWindow("Assignment 4 Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	#ifdef _WINDOWS
		glewInit();
	#endif

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

void PlatformerGame::buildLevel(){

	const char* sheet = "assets\alien_sheet.png";
	GLuint appSpritesTexture = loadTexture("assets/alien_sheet.png");
	int spriteCountX = 30;
	int spriteCountY = 30;

	player.spriteIndex = 79;
	player.sprite = SheetSprite(appSpritesTexture, player.spriteIndex, spriteCountX, spriteCountY);
	player.x = 0.0f; 
	player.y = 0.0f;
	player.width = 0.1f;
	player.height = 0.1f;
	player.isStatic = false;
	player.enableCollisions = true;
	player.velocity_x = 0.0f;
	player.velocity_y = 0.0f;
	player.acceleration_x = 0.0f;
	player.acceleration_y = 0.0f;
	player.friction_x = 3.0f;
	player.friction_y = 3.0f;

	entities.push_back(&player);

	vector<pair<float, float>> tileCoordinates;
	float startFromLeft = -1.0f;
	float endAtRight = 1.0f;
	while (startFromLeft < endAtRight + 0.1) {
		tileCoordinates.push_back({ startFromLeft, -0.8f });
		tileCoordinates.push_back({ startFromLeft, 0.8f });
		if (startFromLeft < -0.3f || startFromLeft > 0.3f) {
			tileCoordinates.push_back({ startFromLeft, 0.0f });
		}
		if (startFromLeft > -0.4f && startFromLeft < 0.4f) {
			tileCoordinates.push_back({ startFromLeft, 0.4f });
			tileCoordinates.push_back({ startFromLeft, -0.4f });
		}
		startFromLeft += 0.1f;
		startFromLeft = roundf(startFromLeft * 100) / 100;
	}
	float startFromTop = 0.8f;
	float endAtBottom = -0.8f;
	while (startFromTop > endAtBottom) {
		tileCoordinates.push_back({ -1.0f, startFromTop });
		tileCoordinates.push_back({ 1.0f, startFromTop });
		startFromTop -= 0.1f;
		startFromTop = roundf(startFromTop * 100) / 100;
	}

	for (int i = 0; i < tileCoordinates.size(); i++) {
		tiles[i].spriteIndex = 1;
		tiles[i].sprite = SheetSprite(appSpritesTexture, tiles[i].spriteIndex, spriteCountX, spriteCountY);
		tiles[i].x = tileCoordinates[i].first;
		tiles[i].y = tileCoordinates[i].second;
		tiles[i].width = 0.1f; tiles[i].height = 0.1f;
		tiles[i].isStatic = true;

		entities.push_back(&tiles[i]);
	}
}

bool PlatformerGame::updateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		fixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	update(elapsed);
	render();
	return done;
}

void PlatformerGame::render(){
	GLclampf red = 94 / 255.0;
	GLclampf green = 128 / 255.0;
	GLclampf blue = 160 / 255.0;
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->render();
	}

	SDL_GL_SwapWindow(displayWindow);
}

void PlatformerGame::update(float elapsed){

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && !event.key.repeat) { //mid-air jumping
				player.velocity_y += 3.0f;
			}
			if ((event.key.keysym.scancode == SDL_SCANCODE_UP || event.key.keysym.scancode == SDL_SCANCODE_W) &&
				!event.key.repeat && player.collidedBottom == true) { //standard jumping
				player.velocity_y += 5.0f;
			}
		}
	}

	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) {
		player.acceleration_x = 5.0f;
	}
	else if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) {
		player.acceleration_x = -5.0f;
	}
	else {
		player.acceleration_x = 0.0f;
	}

}

void PlatformerGame::fixedUpdate() {
	for (size_t i = 0; i < entities.size(); i++) {
		entities[i]->fixedUpdate();

		entities[i]->collidedTop = false;
		entities[i]->collidedBottom = false;
		entities[i]->collidedLeft = false;
		entities[i]->collidedRight = false;

		if (!entities[i]->isStatic) {
			entities[i]->velocity_y += GRAVITY * FIXED_TIMESTEP;
			//            entities[i]->velocity_x += gravity_x * FIXED_TIMESTEP;
			//            entities[i]->velocity_y += gravity_y * FIXED_TIMESTEP;
		}
	}

	// handle Y collisiions
	for (int i = 0; i < entities.size(); i++) {
		if (!entities[i]->isStatic && entities[i]->enableCollisions) {
			entities[i]->y += entities[i]->velocity_y * FIXED_TIMESTEP;
			handleCollisionY(entities[i], entities);
		}
	}

	// handle X collisions
	for (int i = 0; i < entities.size(); i++) {
		if (!entities[i]->isStatic && entities[i]->enableCollisions) {
			entities[i]->x += entities[i]->velocity_x * FIXED_TIMESTEP;
			handleCollisionX(entities[i], entities);
		}
	}
}