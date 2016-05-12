#include "SpriteSheet.h"
#include "Matrix.h"
#include "ShaderProgram.h"


SheetSprite::SheetSprite() {}

SheetSprite::SheetSprite(GLuint textureId, float index, int countX, int countY, float x_scale, float y_scale) : textureId(textureId), index(index), countX(countX), countY(countY), x_scale(x_scale), y_scale(y_scale) {
	u = (float)(((int)index) % countX) / (float)countX;
	v = (float)(((int)index) / countX) / (float)countY;
	width = 1.0 / (float)countX;
	height = 1.0 / (float)countY;
}

SheetSprite::SheetSprite(GLuint textureId, float u, float v, float width, float height) : textureId(textureId), u(u), v(v), width(width), height(height) {}

void SheetSprite::Draw(float objWidth, float objHeight) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);

	GLfloat quad[] = {
		-objWidth * x_scale, objHeight * y_scale,
		-objWidth * x_scale, -objHeight * y_scale,
		objWidth * x_scale, -objHeight * y_scale,
		objWidth * x_scale, objHeight * y_scale
	};

	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	u = (float)(((int)index) % countX) / (float)countX;
	v = (float)(((int)index) / countX) / (float)countY;
	width = 1.0 / (float)countX;
	height = 1.0 / (float)countY;

	GLfloat quadUVs[] = {
		u, v,
		u, v + height,
		u + width, v + height,
		u + width, v
	};

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void SheetSprite::FlipX() {
	x_scale = -x_scale;
}