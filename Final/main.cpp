#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "ShaderProgram.h"
#include "Platformer.h"

/*
Make A Platformer!
Use tilemap or static/dynamic entities
scroll
procedurally generated/load levels from a file.

*/

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif



/*int main(int argc, char *argv[])
{
	PlatformerGame game;
	while (!game.UpdateAndRender()) {}
	return 0;

}*/


int main(int argc, char *argv[])
{
	PlatformerGame game;
	//state = STATE_MAIN_MENU;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	game.Setup(projectionMatrix);
	float ticks;
	float lastFrameTicks = 0.0f;
	float elapsed;

	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	/*SDL_AudioSpec requestSpec;
	requestSpec.freq = 44100;
	requestSpec.format = AUDIO_F32;
	requestSpec.samples = 512;
	requestSpec.channels = 1;
	requestSpec.userdata = NULL;
	requestSpec.callback = myAudioCallBack;*/

	//deviceSpec.userdata = (void*)this;
	//SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &requestSpec, &deviceSpec, SDL_AUDIO_ALLOW_ANY_CHANGE);

	//SDL_PauseAudioDevice(dev, 0);

	//initializeEntities();

	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	game.readData();
	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 48; j++)
		{
			//cout << levelData[i][j] << endl;
		}
	}

	while (!game.isDone()) {

		ticks = (float)SDL_GetTicks() / 1000.0f;
		elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		game.ProcessEvents();

		glUseProgram(program.programID);
		//p1Entity.Update(elapsed);
		game.setBackgroundColorAndClear();
		program.setModelMatrix(modelMatrix);
		program.setViewMatrix(viewMatrix);
		program.setProjectionMatrix(projectionMatrix);



		float fixedElapsed = elapsed;
		if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
			fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
		}
		while (fixedElapsed >= FIXED_TIMESTEP) {
			fixedElapsed -= FIXED_TIMESTEP;
			game.Update(FIXED_TIMESTEP, modelMatrix, program);
		}
		game.Update(fixedElapsed, modelMatrix, program);

		game.setViewMatrix(program, viewMatrix);

		game.Render(program, modelMatrix);

		SDL_GL_SwapWindow(game.getWindow());
	}

	SDL_Quit();
	return 0;
}