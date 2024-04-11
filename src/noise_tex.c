#include <raylib.h>
#include "root.h"
#include "noise_tex.h"

NoiseTexState *noisetex_init() {
  NoiseTexState *state = MemAlloc(sizeof(NoiseTexState));

  shader_ar_init(&state->ar_shader, RES_PATH "noisetex0.fs");

  state->render_texture =
      LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

  // fracCoords won't work properly without fullsize texture
	// using simple square texture
  int w = GetScreenHeight();
  int h = GetScreenHeight();
  Color *pixels = (Color *)MemAlloc(w * h * sizeof(Color));

	for( int x = 0 ; x < w * h ; x += 1 ) {
		Color *p = &pixels[x];
		p->r = GetRandomValue(0, 0xff);
		p->g = GetRandomValue(0, 0xff);
		p->b = GetRandomValue(0, 0xff);
		p->a = GetRandomValue(0, 0xff);
	}

  Image image = {.data = pixels, // We can assign pixels directly to data
                 .width = w,
                 .height = h,
                 .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
                 .mipmaps = 1};
  Texture2D texture = LoadTextureFromImage(image);
  state->texture = texture;

  UnloadImage(image); // Unload CPU (RAM) image data (pixels)

	return state;
}

void noisetex_step(NoiseTexState *state) {
  BeginTextureMode(state->render_texture);
		BeginShaderMode(state->ar_shader.shader);
			Rectangle rec = {0, 0, GetScreenHeight(), GetScreenHeight()};
			Vector2 pos = {(float)(GetScreenWidth() - GetScreenHeight()) / 2, 0};
			DrawTextureRec(state->texture, rec, pos, WHITE);
		EndShaderMode();
  EndTextureMode();

  DrawTextureRec(state->render_texture.texture,
                 (Rectangle){0, 0, state->render_texture.texture.width,
                             -state->render_texture.texture.height},
                 (Vector2){0, 0}, WHITE);
}

void noisetex_dispose(NoiseTexState *state) {
	if (!state) {
		return;
	}
	UnloadShader(state->ar_shader.shader);
	UnloadTexture(state->texture);
	UnloadRenderTexture(state->render_texture);
}
