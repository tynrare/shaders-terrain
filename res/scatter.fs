#version 100

precision mediump float;

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
uniform float scatter_amount;
uniform float scatter_scale;

mat2 rotate(float angle) {
	return mat2(
		cos(angle), -sin(angle),
		sin(angle), cos(angle)
	);
}

void draw_scatter( out vec4 out_fragColor, in vec2 fragCoord ) {
		vec2 uv = fragCoord * gridscale;
		vec2 uv_fract = fract(uv);
		vec2 uv_ceil = ceil(uv) / gridscale;
		vec4 rand = texture2D(texture0, uv_ceil);

		float spawn_chance = 1.0 - step(scatter_amount, rand.x);

		float scale = (1.0 / min(1.0 + rand.z, scatter_scale));
		vec2 uv_scaled = uv_fract * scale;

		mat2 rot = rotate(rand.y * 360.0);
		vec2 uv_rot = (uv_scaled - 0.5) * rot + 0.5;

		vec2 uv_sheet = vec2(
			ceil(rand.x * sheet_w) / sheet_w + (uv_rot.x / sheet_w), 
			ceil(rand.y * sheet_h) / sheet_h + (uv_rot.y / sheet_h)
		);


		vec4 color = texture2D(tex_sheet, uv_sheet) * spawn_chance;

    out_fragColor = vec4(color.rgb, 1.0);
    //out_fragColor = vec4(uv_rot.x, uv_rot.y, 0.0, 1.0);

}

void main()
{
	draw_scatter(gl_FragColor, fragTexCoord);
}
