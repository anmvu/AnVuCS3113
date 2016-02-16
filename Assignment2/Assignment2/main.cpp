#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "Entity.h"

#define PI 3.14159265

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

/*
Make Pong!
Does not need to keep score
Must detect wins
can use images or untextured polygons
keyboard, mouse or joystick input
*/


SDL_Window* displayWindow;

Entity ball;
Entity left(0.1,0.3), right(0.1,0.3);
Entity top_bumper(2,0.1), bottom_bumper(2,0.1), net(0.02,1.5);
const Uint8* keys = SDL_GetKeyboardState(nullptr);

GLuint LoadTexture(const char *img, int type){
	SDL_Surface *surface = IMG_Load(img);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (type == GL_RGBA){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	}
	else if (type == GL_RGB){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}

void setup(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Assignment 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif
	glViewport(0, 0, 640, 360);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

}

void left_won(bool left_win = true){
	ball.x = 0.0f;
	ball.y = 0.0f;
	ball.direction_x = left_win ? -1.0f : 1.0f;
	ball.direction_y = 0.0f;
	left.x = -1.0f;
	left.y = 0.0f;
	right.x = 1.0f;
	right.y = 0.0f;
	top_bumper.x = 0.0f;
	top_bumper.y = 1.0f;
	bottom_bumper.x = 0.0f;
	bottom_bumper.y = -1.0f;
}

bool Play(){
	SDL_Event event;

	while (SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
			return false;
		}
	}

	//move them paddles
	
	//player left
	if (keys[SDL_SCANCODE_W]) left.direction_y = 1.0f;
	else if (keys[SDL_SCANCODE_S]) left.direction_y = -1.0f;
	else left.direction_y = 0.0f;

	//player right
	if (keys[SDL_SCANCODE_UP]) right.direction_y = 1.0f;
	else if (keys[SDL_SCANCODE_DOWN]) right.direction_y = -1.0f;
	else right.direction_y = 0.0f;

	//crasssh into meee

	//ball to the right
	if (((ball.x + ball.width * 0.5) > (right.x - right.width * 0.5)) &&
		((ball.y + ball.height*0.5) > (right.x - right.height * 0.5)) &&
		((ball.y - ball.height * 0.5) < (right.y + right.height * 0.5)))
		{
			ball.x = right.x - right.width * 0.5 - ball.width*0.5;
			ball.direction_x = -ball.direction_x;
			float rebound = (ball.y - right.y) / (right.height * 0.5);
			ball.direction_y = rebound * ball.max_speed;
		}

	//ball to the left
	if (((ball.x + ball.width * 0.5) > (left.x - left.width * 0.5)) &&
		((ball.y + ball.height*0.5) > (left.x - left.height * 0.5)) &&
		((ball.y - ball.height * 0.5) < ( left.y + left.height * 0.5)))
	{
		ball.x = left.x - left.width * 0.5 - ball.width*0.5;
		ball.direction_x = -ball.direction_x;
		float rebound = (ball.y - left.y) / (left.height * 0.5);
		ball.direction_y = rebound * ball.max_speed;
	}

	//ball up
	if ((ball.y + ball.height * 0.5) > (top_bumper.y - top_bumper.height * 0.5)){
		ball.y = (top_bumper.y - top_bumper.height * 0.05) - (ball.height * 0.5);
		ball.direction_y = -ball.direction_y;
	}

	//ball down
	if ((ball.y + ball.height * 0.5) > (bottom_bumper.y + bottom_bumper.height * 0.5)){
		ball.y = (bottom_bumper.y + bottom_bumper.height * 0.05) - (ball.height * 0.5);
		ball.direction_y = -ball.direction_y;
	}

	//left up 
	if ((left.y + left.height * 0.5) > (top_bumper.y - top_bumper.height * 0.05)){
		left.y = (top_bumper.y - top_bumper.height * 0.5) - (left.height * 0.5);
	}

	//left down
	if ((left.y + left.height * 0.5) > (bottom_bumper.y + top_bumper.height * 0.05)){
		left.y = (bottom_bumper.y - bottom_bumper.height * 0.5) - (left.height * 0.5);
	}

	//right up
	if ((right.y + right.height * 0.5) > (top_bumper.y - top_bumper.height * 0.05)){
		right.y = (top_bumper.y - top_bumper.height * 0.5) - (right.height * 0.5);
	}
	//right down
	if ((right.y + right.height * 0.5) > (bottom_bumper.y + bottom_bumper.height * 0.05)){
		right.y = (bottom_bumper.y + bottom_bumper.height * 0.5) - (right.height * 0.5);
	}
	//right wins
	if (ball.x > (right.x + right.width)){
		left_won(false);
	}

	//left wins
	if (ball.x < (left.x + left.width)){
		left_won(true);
	}

	return true;
}

void Update(float elapsed){
	left.y += left.direction_y*elapsed;
	right.y += right.direction_y*elapsed;
	ball.x += ball.direction_x * elapsed;
	ball.y += ball.direction_y * elapsed;
	ball.max_speed = 2;
}

void Render(){
	glClear(GL_COLOR_BUFFER_BIT);
	ball.draw();
	left.draw();
	right.draw();
	top_bumper.draw();
	bottom_bumper.draw();
	net.draw();
	SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char *argv[])
{
	setup();	

	float lastFrameTicks = 0.0f;

	left_won();

	while (Play()) {

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		Update(elapsed);
		Render();

	}

	SDL_Quit();
	return 0;
}
