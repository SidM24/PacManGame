#include<png.h>
#ifndef COURSEWORK_LOAD_AND_BIND_TEXTURE_H
#define COURSEWORK_LOAD_AND_BIND_TEXTURE_H

#include <GL/glut.h>
#include "png_load.h"

unsigned int load_and_bind_texture(const char* filename) {
	char* image_buffer = NULL;
	int width = 0;
	int height = 0;

	//Reading the image png data to image Buffer using a external function png_load
	if (png_load(filename, &height, &width, &image_buffer) == 0) {
		fprintf(stderr, "Failed to read image texture from %s\n", filename);
		exit(1);
	}

	//else we will handle the texturing of the image onto the screen
	unsigned int texture_handle = 0;
	glGenTextures(1, &texture_handle);

	// create a new texture object and bind it to tex_handle
	glBindTexture(GL_TEXTURE_2D, texture_handle);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image_buffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	free(image_buffer);
	return texture_handle;
}

#endif
