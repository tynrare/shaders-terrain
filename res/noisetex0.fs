#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;           // Texture coordinates (sampler2D)
varying vec4 fragColor;              // Tint color

// Uniform inputs
uniform vec2 resolution;
uniform vec2 pointer;
uniform sampler2D texture0;
uniform float elapsed;

mat2 rotate(float angle) {
	return mat2(
		cos(angle), -sin(angle),
		sin(angle), cos(angle)
	);
}

vec4 noisetex0(vec2 uv) {
	return texture2D(texture0, uv);
}

void main() {
	vec2 uv = fragTexCoord;
	vec4 rand0 = noisetex0(uv);
	vec4 rand = noisetex0(rand0.xy);
	vec4 color = rand * fragColor;
	color.a = 1.0;

	gl_FragColor = color;
}
