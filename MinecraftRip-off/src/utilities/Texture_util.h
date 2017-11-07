/**
* MTAT.03.015 Computer Graphics.
* Texture loading utility routines.
*/
#ifndef TEXTURE_UTIL_H
#define TEXTURE_UTIL_H
#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <iostream>

GLuint load_texture(GLenum target, GLint internalFormat, const char* filename);

#endif

