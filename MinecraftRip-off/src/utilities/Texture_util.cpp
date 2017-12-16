/**
* DevIL texture loading utility functions.
*/
#include "Texture_util.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <iostream>

using namespace std;

bool is_texture_util_inited = false;

void init_texture_util() {
	if (!is_texture_util_inited) {
		is_texture_util_inited = true;
		ILuint error;

		ilutRenderer(ILUT_OPENGL);
		ilInit();

		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

		error = ilGetError();

		if (error != IL_NO_ERROR) {
			printf("Could not initialize IL library. Error: %d\n", error);
			exit(2);
		}
	}
}

GLuint load_texture(GLenum target, GLint internalFormat, const char* filename) {
	GLuint handle;
	init_texture_util();

	ILuint error;
	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);
	std::string fullFilename = std::string("resources/textures/") + filename;
	ilLoadImage(fullFilename.c_str());  // Loads into the current bound image

	error = ilGetError();

	if (error != IL_NO_ERROR) {
		printf("Could not load image. Error: %d\n", error);
		exit(2);
	}

	glGenTextures(1, &handle);
	glBindTexture(target, handle);

	glTexImage2D(target, 0, internalFormat, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	glGenerateMipmap(target);

	error = ilGetError();
	if (error != IL_NO_ERROR) {
		printf("Could not bind image as texture. Error: %d\n", error);
		exit(2);
	}
	if (handle == 0) {
		printf("Error in texture binding\n");
	}

	glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return handle;
}
