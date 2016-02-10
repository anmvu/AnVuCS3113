#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"

#define PI 3.14159265

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

//create 2d scene using textured polygons
//one element must be animated
//3 different textures

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

void draw(ShaderProgram& program, Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix, GLuint texture){
	//Blend
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat vertices[] = { 1, 1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1 };
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);

	GLfloat coords[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0 };
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, coords);
	glEnableVertexAttribArray(program.texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);


}

void draw_background(ShaderProgram program, Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix, GLuint texture){
	//Blend
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	//screen is -3.55f, 3.55f, -2.0f, 2.0f
	glUseProgram(program.programID);
	program.setModelMatrix(modelMatrix);
	program.setProjectionMatrix(projectionMatrix);
	program.setViewMatrix(viewMatrix);
	float vertices[] = { 3.55, 1.3,-3.55, 1.3, -3.55, -2.0 , 3.55, 1.3, -3.55, -2.0, 3.55, -2.0 };
	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	//screen is -3.55f, 3.55f, -2.0f, 2.0f
	float coords[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0,0.0, 0.0, 1.0, 1.0, 0.0, 1.0 };
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, coords);
	glEnableVertexAttribArray(program.texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);

}

void move_it(ShaderProgram program, Matrix& modelMatrix, Matrix& projectionMatrix, Matrix& viewMatrix,float x, float y){
	program.setModelMatrix(modelMatrix);
	program.setProjectionMatrix(projectionMatrix);
	program.setViewMatrix(viewMatrix);
	modelMatrix.identity();
	modelMatrix.Translate(x, y, 0.0);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Assignment 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	SDL_Event event;
	bool done = false;
	
	//size and pixel offset
	glViewport(0, 0, 640, 360);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	GLuint street = LoadTexture("street.png", GL_RGBA);
	GLuint car = LoadTexture("car.png",GL_RGBA);
	GLuint moon = LoadTexture("moon.png",GL_RGBA);

	float car_height = 4.0;
	float car_width = 4.0;
	float y = -2.0;

	Matrix projectionMatrix, projectionMatrix2, projectionMatrix3;
	Matrix modelMatrix, modelMatrix2, modelMatrix3;
	Matrix viewMatrix, viewMatrix2, viewMatrix3;

	//projection matrix
	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	projectionMatrix2.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	projectionMatrix3.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	
	float lastFrameTicks = 0.0f;

	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		//tick tick tick
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		glClear(GL_COLOR_BUFFER_BIT);

		//drawing
		draw_background(program,modelMatrix,projectionMatrix,viewMatrix,street);
		move_it(program, modelMatrix2, projectionMatrix2, viewMatrix2, 0, y);
		modelMatrix2.Scale(car_width,car_height,0);
		draw(program,modelMatrix2,projectionMatrix2,viewMatrix2,car);
		move_it(program, modelMatrix3, projectionMatrix3, viewMatrix3, -3.0, 2.0);
		draw(program,modelMatrix3,projectionMatrix3,viewMatrix3,moon);
		
		y += 0.05;
		car_width = 1/car_width;
		car_height = 1/car_height;

		if (y <= 2.0){
			move_it(program, modelMatrix2, projectionMatrix2, viewMatrix2, 0, y);
			modelMatrix2.Scale(car_width, car_height, 0);
			y += 0.5;
			car_width = 1 / car_width;
			car_height = 1 / car_height;
		}

		move_it(program, modelMatrix2, projectionMatrix2, viewMatrix2, 0, -2.0);
		car_width = 1 / car_width;
		car_height = 1 / car_height;
		modelMatrix2.Scale(car_width, car_height, 0);

		glDisableClientState(GL_VERTEX_ARRAY);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
