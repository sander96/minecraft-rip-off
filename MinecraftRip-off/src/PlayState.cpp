#include "PlayState.h"
#include "utilities/Shader_util.h"
#include "utilities/Texture_util.h"
#include <iostream>
#include "utilities/Settings.h"


int textureIndex = 0;
std::vector<GLuint> textureHandles = std::vector<GLuint>();

shader_prog shaderCube("texture.vert.glsl", "texture.frag.glsl");
shader_prog shader("default.vert.glsl", "default.frag.glsl");
GLuint createCube();

void loadTextures() {
	//PILDI LUGEMINE KORRAS;
	//0-muld,1-liiv,2-muru,3-kivi,4-puu,5-lehed
	textureHandles.push_back(load_texture(GL_TEXTURE_2D, GL_RGB, "textures/blocks/muld.png"));
	textureHandles.push_back(load_texture(GL_TEXTURE_2D, GL_RGB, "textures/blocks/liiv.png"));
	textureHandles.push_back(load_texture(GL_TEXTURE_2D, GL_RGB, "textures/blocks/muru.png"));
	textureHandles.push_back(load_texture(GL_TEXTURE_2D, GL_RGB, "textures/blocks/kivi.png"));
	textureHandles.push_back(load_texture(GL_TEXTURE_2D, GL_RGB, "textures/blocks/puu.png"));
	textureHandles.push_back(load_texture(GL_TEXTURE_2D, GL_RGB, "textures/blocks/lehed.png"));
}

GLuint createCube(shader_prog* shader) {
	GLuint vertexArrayHandle;
	GLuint arrayBufferHandle;

	glGenVertexArrays(1, &vertexArrayHandle);
	glBindVertexArray(vertexArrayHandle);
	glGenBuffers(1, &arrayBufferHandle);

	GLfloat vertexData[] = {
		//X		Y		Z		U	  V			NORMAL
		// bottom
		-1.0f, -1.0f, -1.0f,    0.0f,0.0f,		0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		1.0f,0.0f,		0.0f,-1.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,    0.0f,(float)1/3,    0.0f, -1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,     1.0f,0.0f,		1.0f, -1.0f, 0.0f,
		1.0f, -1.0f,  1.0f,     1.0f,(float)1/3,    0.0f, -1.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,    0.0f,(float)1/3,    0.0f, -1.0f, 0.0f,

		// top
		-1.0f, 1.0f, -1.0f,   0.0f,1.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f,  1.0f,   0.0f,(float)2/3,		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -1.0f,    1.0f,1.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -1.0f,    1.0f,1.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f,  1.0f,   0.0f,(float)2/3,      0.0f, 1.0f, 0.0f,
		1.0f, 1.0f,  1.0f,    1.0f,(float)2/3,	    0.0f, 1.0f, 0.0f,

		// front
		-1.0f, -1.0f, 1.0f,   0.0f,(float)1/3,		0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f,    1.0f,(float)1/3,		0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,   0.0f,(float)2/3,	    0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f,    1.0f,(float)1/3,		0.0f, 0.0f, 1.0f,
		1.0f,  1.0f, 1.0f,    1.0f,(float)2/3,		0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,   0.0f,(float)2/3,	    0.0f, 0.0f, 1.0f,

		// back
		-1.0f, -1.0f, -1.0f,   1.0f,(float)1/3,		0.0f, 0.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,   1.0f,(float)2/3,		0.0f, 0.0f, -1.0f,
		1.0f, -1.0f, -1.0f,    0.0f,(float)1/3,		0.0f, 0.0f, -1.0f,
		1.0f, -1.0f, -1.0f,    0.0f,(float)1/3,		0.0f, 0.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,   1.0f,(float)2/3,		0.0f, 0.0f, -1.0f,
		1.0f,  1.0f, -1.0f,    0.0f,(float)2/3,		0.0f, 0.0f, -1.0f,

		// left
		-1.0f, -1.0f,  1.0f,   1.0f,(float)1/3,   -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,   0.0f,(float)2/3,   -1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,   0.0f,(float)1/3,   -1.0f, 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,   1.0f,(float)1/3,   -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,   1.0f,(float)2/3,   -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,   0.0f,(float)2/3,   -1.0f, 0.0f, 0.0f,

		// right
		1.0f, -1.0f,  1.0f,    0.0f,(float)1/3,    1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,    1.0f,(float)1/3,    1.0f, 0.0f, 0.0f,
		1.0f,  1.0f, -1.0f,    1.0f,(float)2/3,    1.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  1.0f,    0.0f,(float)1/3,    1.0f, 0.0f, 0.0f,
		1.0f,  1.0f, -1.0f,    1.0f,(float)2/3,    1.0f, 0.0f, 0.0f,
		1.0f,  1.0f,  1.0f,    0.0f,(float)2/3,    1.0f, 0.0f, 0.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	GLuint loc = glGetAttribLocation(shaderCube.getProg(), "position");
	if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "position"));
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (const GLvoid*)(0 * sizeof(float)));

	loc = glGetAttribLocation(shaderCube.getProg(), "uv");
	if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "color"));
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));

	loc = glGetAttribLocation(shaderCube.getProg(), "normal");
	if (loc < 0) throw (std::runtime_error(std::string("Location not found in shader program for variable ") + "normal"));
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 8* sizeof(float), (const GLvoid*)(5 * sizeof(float)));

	glBindVertexArray(0);
	return vertexArrayHandle;
}

PlayState::PlayState(GLFWwindow* window)
	:
	shader{ "default.vert.glsl", "default.frag.glsl" }
{
	
	loadTextures();//Loads all the textures in once.

	shader.use();

	GLfloat vertices[] = {
		0.0, 1.0, 0.0,
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
	};

	GLubyte indices[] = {
		0, 1, 2
	};

	GLfloat colors[] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};

	GLuint vertexArrayHandle;
	glGenVertexArrays(1, &vertexArrayHandle);
	glBindVertexArray(vertexArrayHandle);
	
	glfwGetCursorPos(window, &mousePosX, &mousePosY); // init global mouse positions

	shader.attribute3fv("position", vertices, 9);
	shader.attribute3fv("color", colors, 9);

	GLuint vboHandle;
	glGenBuffers(1, &vboHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * 9, indices, GL_STATIC_DRAW);

	testTriangleVAO = vertexArrayHandle;
	glfwSetCursorPos(window, settings.getWidth() / 2.0, settings.getHeight() / 2.0);
}



void PlayState::processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		camera.move(glm::vec3(0.0, 0.0, 0.1));
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		camera.move(glm::vec3(0.1, 0.0, 0.0));
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camera.move(glm::vec3(0.0, 0.0, -0.1));
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		camera.move(glm::vec3(-0.1, 0.0, 0.0));
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	double dX = (xpos - settings.getWidth() / 2.0) * settings.getSensitivity();
	double dY = (ypos - settings.getHeight() / 2.0)  * settings.getSensitivity();

	camera.rotate(glm::vec2(-dX, -dY));
	glfwSetCursorPos(window, settings.getWidth() / 2.0, settings.getHeight() / 2.0);
}

void PlayState::update(GLFWwindow* window)
{

}
//assigns a texture to a cube and displays it
void makeBlock(int number){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandles.at(number));
	//call out for making a cube.
	glBindVertexArray(createCube(&shaderCube));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
void PlayState::render(GLFWwindow* window)
{

	shaderCube.use();
	//Compile and send uniforms
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE);

	glm::mat4 matrix{ 1.0 };
	matrix = glm::scale(matrix, glm::vec3(1, 1, 1));
	matrix = glm::rotate(matrix, 2.0f*(float)fmod(glfwGetTime(), 360.0), glm::vec3(0.0f, 1.0f, 0.0f));
	//matrix = glm::rotate(matrix, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));

	shaderCube.uniformMatrix4fv("projectionMatrix", camera.getPerspective());
	shaderCube.uniformMatrix4fv("viewMatrix", camera.getView());
	shaderCube.uniformMatrix4fv("modelMatrix", matrix);

	shaderCube.activate();
	//0-muld,1-liiv,2-muru,3-kivi,4-puu,5-lehed
	//vb mingi parem texPack?Võib lingi panna kuhugi,et saaks noid kasutada siis..
	makeBlock(2);

	glfwSwapBuffers(window);
}
