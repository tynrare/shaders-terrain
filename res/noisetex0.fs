#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;           // Texture coordinates (sampler2D)
varying vec4 fragColor;              // Tint color

// Uniform inputs
uniform sampler2D texture0;
uniform float elapsed;

vec4 noisetex0(vec2 uv) {
	return texture2D(texture0, uv);
}

void main() {
	vec2 uv = fragTexCoord;
	vec4 rand = noisetex0(uv);
	float v = elapsed * 0.001 + 255.0;
	float v1 = rand.x;
	vec4 color = noisetex0(v * v1 *  uv) * fragColor;
	color.a = 1.0;
	gl_FragColor = color;
}
