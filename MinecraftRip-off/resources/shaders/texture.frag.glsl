#version 400

uniform vec3 lightPosition;
uniform sampler2D tex;

in vec3 interpolatedNormal;
in vec3 interpolatedPosition;
in vec2 interpolatedUv;

out vec4 fragColor;


void main(void) {
	vec3 viewerPosition = vec3(0.0);
	fragColor = texture2D(tex, interpolatedUv);
}
