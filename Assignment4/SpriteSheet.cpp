#include "SpriteSheet.h"

SheetSprite::SheetSprite() {}

SheetSprite::SheetSprite(GLuint textureId, float spriteIndex, int spriteCountX, int spriteCountY) : textureId(textureId) {
	u = (float)(((int)spriteIndex) % spriteCountX) / (float)spriteCountX;
	v = (float)(((int)spriteIndex) / spriteCountY) / (float)spriteCountY;
	width = 1.0 / (float)spriteCountX;
	height = 1.0 / (float)spriteCountY;
}

SheetSprite::SheetSprite(GLuint textureId, float u, float v, float width, float height) : textureId(textureId), u(u), v(v), width(width), height(height) {}

void SheetSprite::Draw(float objWidth, float objHeight, float x, float y, float scale) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);

	GLfloat quad[] = {
		-objWidth * scale, objHeight * scale,
		-objWidth * scale, -objHeight * scale,
		objWidth * scale, -objHeight * scale,
		objWidth * scale, objHeight * scale
	};

	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

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