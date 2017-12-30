#version 400

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 position;

void main(void) {
	mat4 modelViewMatrix = viewMatrix * modelMatrix;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}
