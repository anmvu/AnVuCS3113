#include "SpriteSheet.h"
#include "Matrix.h"
#include "ShaderProgram.h"


SheetSprite::SheetSprite() {}

SheetSprite::SheetSprite(GLuint textureId, float index, int countX, int countY) : textureId(textureId), index(index), countX(countX), countY(countY) {
	u = (float)(((int)index) % countX) / (float)countX;
	v = (float)(((int)index) / countX) / (float)countY;
	width = 1.0 / (float)countX;
	height = 1.0 / (float)countY;
}

SheetSprite::SheetSprite(GLuint textureId, float u, float v, float width, float height) : textureId(textureId), u(u), v(v), width(width), height(height) {}

void SheetSprite::draw(ShaderProgram& program, float objWidth, float objHeight) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_TEXTURE_2D);

	GLfloat vertices[] = {
		-TILE_SIZE * xScale, TILE_SIZE * yScale,
		-TILE_SIZE * xScale, -TILE_SIZE * yScale,
		TILE_SIZE * xScale, -TILE_SIZE * yScale,
		TILE_SIZE * xScale, TILE_SIZE * yScale
	};


	u = (float)(((int)index) % countX) / (float)countX;
	v = (float)(((int)index) / countX) / (float)countY;
	width = 1.0 / (float)countX;
	height = 1.0 / (float)countY;

	GLfloat texCoords[] = {
		u, v,
		u, v + height,
		u + width, v + height,
		u, v,
		u + width, v + height,
		u + width, v
	};

	glUseProgram(program.programID);

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);

	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, LEVEL_HEIGHT * LEVEL_WIDTH * 6);
	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);
}


void SheetSprite::scale(float factor) {
	xScale *= factor;
	yScale *= factor;
}