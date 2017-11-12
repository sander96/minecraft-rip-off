#version 400

uniform vec3 lightPosition;

/**
 * --Task--
 * Receive the texture uniform.
 * - Find out what type is it
 * - You can use any name for it, because there is only one texture at a time
 */

 uniform sampler2D tex;

in vec3 interpolatedNormal;
in vec3 interpolatedPosition;
in vec2 interpolatedUv;
//klassiarvutis, et töötaks
out vec4 fragColor;


void main(void) {
    vec3 viewerPosition = vec3(0.0);

    /**
     * --Task--
     * Copy your Phong or Blinn lighting model here.
     * Use the diffuse and ambient color from the texture - sample it from the correct coordinates.
     */
	/**
    vec3 n = interpolatedNormal;
	vec3 l = normalize(interpolatedPosition);
	vec3 v = normalize(interpolatedPosition.xyz);
	vec3 r = reflect(-l, n);
	float shininess= 10.0;
	**/
    // vec3 color =  texture2D(tex,interpolatedUv).rgb * (0.1 + max(0.0,dot(n,l))) + pow(max(0.0,dot(r,v)), shininess); //Assign your calculation here instead
	vec3 color =  texture2D(tex,interpolatedUv).rgb;
    //gl_FragColor = vec4(color, 1.0) + 0.01 * lightPosition;
    fragColor = vec4(color,1.0);

}
