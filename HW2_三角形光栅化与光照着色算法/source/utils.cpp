#include "utils.h"
FragmentAttr getLinearInterpolation(const FragmentAttr& a, FragmentAttr& b, int x_position) {
	FragmentAttr result;
	result.x = x_position;
	float t = (x_position - a.x) / float(b.x - a.x);

	result.z = a.z + t * (b.z - a.z);

	result.color.r = a.color.r + t * (b.color.r - a.color.r);
	result.color.g = a.color.g + t * (b.color.g - a.color.g);
	result.color.b = a.color.b + t * (b.color.b - a.color.b);

	result.normal.x = a.normal.x + t * (b.normal.x - a.normal.x);
	result.normal.y = a.normal.y + t * (b.normal.y - a.normal.y);
	result.normal.z = a.normal.z + t * (b.normal.z - a.normal.z);

	result.pos_mv.x = a.pos_mv.x + t * (b.pos_mv.x - a.pos_mv.x);
	result.pos_mv.y = a.pos_mv.y + t * (b.pos_mv.y - a.pos_mv.y);
	result.pos_mv.z = a.pos_mv.z + t * (b.pos_mv.z - a.pos_mv.z);

	return result;
}


void renderWithTexture(vec3* render_buffer, int h, int w) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, render_buffer);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, -1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}