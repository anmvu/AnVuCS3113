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

}

void left_won(bool left = true){
	ball.x = 0.0f;
	ball.y = 0.0f;
	ball.direction_x = left ? -1.0f : 1.0f;
	ball.direction_y = 0.0f;
	paddle_left.x = -1.0f;
	paddle_left.y = 0.0f;
	paddle_right.x = 1.0f;
	paddle_right.y = 0.0f;
	top_bumper.x = 0.0f;
	top_bumper.y = 1.0f;
	bottom_bumper.x = 0.0f;
	bottom_bumper.y = -1.0f;
}

bool Play(){

}

void Update(){
	paddle_left.y += paddle_left.direction_y*elapsed;
	paddle_right.y += paddle_right.direction_y*elapsed;
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

	SDL_Event event;
	bool done = false;

	glViewport(0, 0, 640, 360);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	float lastFrameTicks = 0.0f;



	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_KEYDOWN){
				
			}
		}

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;


	}

	SDL_Quit();
	return 0;
}
