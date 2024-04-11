#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;           // Texture coordinates (sampler2D)
varying vec4 fragColor;              // Tint color

// Uniform inputs
uniform sampler2D texture0;

vec4 noisetex0(vec2 uv) {
	return texture2D(texture0, uv);
}

void main() {
	gl_FragColor = noisetex0(fragTexCoord) * fragColor;
}
