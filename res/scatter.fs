#version 100

precision mediump float;

#define M_PI 3.1415926535897932384626433832795
#define M_PI2 M_PI * 2.0

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;           // Texture coordinates (sampler2D)
varying vec4 fragColor;              // Tint color

// Uniform inputs
uniform vec2 resolution;
uniform sampler2D texture0;
uniform sampler2D tex_sheet;
uniform float gridscale;
uniform float sheet_w;
uniform float sheet_h;
uniform float spritemask_scale;
uniform float scatter_amount;
uniform float scatter_scale;
uniform float mode;

mat2 rotate(float angle) {
	return mat2(
		cos(angle), -sin(angle),
		sin(angle), cos(angle)
	);
}

vec4 scatter( in vec2 fragCoord, float rand_origin, float spawn_mask ) {
		vec2 uv = fragCoord * gridscale;
		vec2 uv_fract = fract(uv);
		vec2 uv_ceil = ceil(uv) / gridscale;
		vec4 rand = texture2D(texture0, fract(uv_ceil + rand_origin));

		float scale = max(rand.z + 0.2, 1.1 - scatter_scale);
		vec2 uv_scaled = uv_fract * scale;
		mat2 rot = rotate(rand.y * M_PI2);
		vec2 uv_rot = (uv_scaled - 0.5 * scale) * rot + 0.5;

		vec2 boxmask = step(0.5 * scale * scale, 0.5 * spritemask_scale * scale - abs(uv_rot - 0.5));

		float spawn_chance = 1.0 - step(scatter_amount * spawn_mask, rand.x);
		float mask = boxmask.x * boxmask.y;
		float cross = step(abs(uv_rot.x - 0.5), 0.01 * scale) + step(abs(uv_rot.y - 0.5), 0.01 * scale);

		vec2 uv_sheet = vec2(
			ceil(rand.x * sheet_w) / sheet_w - (uv_rot.x / sheet_w), 
			ceil(rand.y * sheet_h) / sheet_h - (uv_rot.y / sheet_h)
		);

		vec4 color = texture2D(tex_sheet, uv_sheet) * spawn_chance * mask;
		//vec4 color = vec4(cross, cross, cross, 1.0);
		//vec4 color = rand * mask * spawn_chance;

		return color;
}

vec4 scatter0( in vec2 fragCoord ) {
	return scatter(fragCoord, 0.0, 1.0);
}

vec4 scatter1( in vec2 fragCoord ) {
	vec2 uv = fragCoord;

	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

	float maskgrid = gridscale * 0.2;

	const int loops = 7;
	for (int i = 0; i < loops; i++) {
		float f = float(i) / float(loops);

		vec4 rand = texture2D(texture0, vec2(sin(f), cos(f)));

		float uvx = sin(f * M_PI2) * rand.x * f;
		float uvy = cos(f * M_PI2) * rand.z * f;
		vec2 uv_local = vec2(uvx, uvy);
		vec4 maskrand = texture2D(texture0, ceil(uv_local * maskgrid) / maskgrid);

		float randx = (rand.x - 0.5) * 0.3;
		float randy = (rand.y - 0.5) * 0.3;


		vec2 coord = vec2(uv.x + uvx + randx, uv.y + uvy + randy);
		vec4 c = scatter(coord, rand.x, maskrand.x * scatter_amount * 0.5);
		color = mix(color, c, c.a);
	}

	return color;
}

void main()
{
	vec4 color = scatter1(fragTexCoord);
	color.a = 1.0;
	gl_FragColor = color;
}
