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
	float rf = resolution.y / resolution.x;
	vec2 uv = fragTexCoord;
	uv.x /= rf;
	vec2 center = vec2(0.5, 0.5);
	float dist = min(1.0, distance(center, uv));
	uv -= center;
	uv *= rotate(elapsed * cos(elapsed * 0.01) * 0.01);
	uv *= 0.5;
	vec4 rand0 = noisetex0(uv);
	vec4 rand = noisetex0(rand0.xy + elapsed * 0.001);
	vec4 color = rand * fragColor;
	color.a = 1.0;
	color.r *= 1.0;
	color.g *= 1.0;
	color.b *= 1.0;

	gl_FragColor = color;
	//float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
	//gl_FragColor = vec4(gray, gray, gray, 1.0);
}
