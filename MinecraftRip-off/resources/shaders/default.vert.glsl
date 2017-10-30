#version 400

uniform mat4 modelMatrix;

in vec3 position;
in vec3 color;
out vec3 interpolatedColor;

void main(void) {
    interpolatedColor = color;
    gl_Position = modelMatrix * vec4(position, 1.0);
}
