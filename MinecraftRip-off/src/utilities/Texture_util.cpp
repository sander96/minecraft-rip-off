/**
* DevIL texture loading utility functions.
*/
#include "Texture_util.h"

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
	ilLoadImage(filename);  // Loads into the current bound image

	error = ilGetError();

	if (error != IL_NO_ERROR) {
		printf("Could not load image. Error: %d\n", error);
		exit(2);
	}

	//handle = ilutGLBindTexImage(); // This would generate the texture for you

	//But here we generate it ourselves via OpenGL commands.
	glGenTextures(1, &handle);
	glBindTexture(target, handle);

	//See: https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
	//InternalFormat - format of the texture (can be GL_RGB, GL_RGBA among many-many others)
	//Format - format of the image (must be GL_RGB or GL_RGBA or something among few others)
	glTexImage2D(target, 0, internalFormat, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	glGenerateMipmap(target);

	printf("Loaded image with format: %d, and bytes per pixel: %d\n", ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_BPP));

	error = ilGetError();
	if (error != IL_NO_ERROR) {
		printf("Could not bind image as texture. Error: %d\n", error);
		exit(2);
	}
	if (handle == 0) {
		printf("Error in texture binding\n");
	}

	//You can specify different filters here
	//glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



	return handle;
}
