#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;           // Texture coordinates (sampler2D)
varying vec4 fragColor;              // Tint color

// Uniform inputs
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

	vec2 uv_mod1 = uv;
	uv_mod1 -= 0.5;
	float dist = distance(uv_mod1, vec2(0.0, 0.0)) * 0.1;
	uv_mod1 *= rotate(elapsed);
	uv_mod1 += 0.5;
	vec4 rand = noisetex0(mix(uv_mod1, uv, dist));
	vec4 color = rand * fragColor;
	color.a = 1.0;

	gl_FragColor = color;
}
