#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;           // Texture coordinates (sampler2D)
varying vec4 fragColor;              // Tint color

// Uniform inputs
uniform sampler2D texture0;
uniform sampler2D texture_mix;
uniform sampler2D texture_mix1;
uniform sampler2D tex_noise0;
uniform sampler2D tex_noise1;
uniform vec2 resolution;        // Viewport resolution (in pixels)
uniform vec2 mouse;             // Mouse pixel xy coordinates
uniform float time;             // Total run time (in secods)
uniform float tilesize;
uniform float zoom;
uniform float erosion;

const float darken_second_mix_tex = 0.8;
const float outline_size_second_mix_tex = 0.2;

void grayscale( out vec4 out_fragColor, in vec2 fragCoord ) {
    // Texel color fetching from texture sampler
    vec4 texelColor = texture2D(texture0, fragCoord)*fragColor;
    // Convert texel color to grayscale using NTSC conversion weights
    float gray = dot(texelColor.rgb, vec3(0.299, 0.587, 0.114));
    // Calculate final fragment color
    out_fragColor = vec4(gray, gray, gray, texelColor.a);
}

void draw_texture( out vec4 out_fragColor, in vec2 fragCoord ) {
	vec2 uv = fragCoord * zoom;
	vec2 uv_tile = uv * zoom * tilesize;

	vec4 noise1_color = texture2D(tex_noise1, fract(uv));
	vec4 noise0_color = texture2D(tex_noise0, fract(uv * noise1_color.yz));

	// Texel color fetching from texture sampler
	vec4 main_color = texture2D(texture0, fract(uv_tile))*fragColor;
	vec4 mix_color = texture2D(texture_mix, fract(uv_tile));
	vec4 mix_color2 = texture2D(texture_mix1, fract(uv_tile)) * darken_second_mix_tex;

	// masking mix texture with noise
	float mask1 = step(erosion, noise0_color.x * noise1_color.x);
	float mask1_outline = smoothstep(erosion - 0.1, erosion + outline_size_second_mix_tex, noise0_color.x * noise1_color.x);

	vec4 color1 = mix(mix_color, mix_color2, mask1);
	vec4 color2 = mix(color1, main_color, mask1_outline);

	out_fragColor = color2;
}

void main()
{
	draw_texture(gl_FragColor, fragTexCoord);
}
