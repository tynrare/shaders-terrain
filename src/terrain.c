#include "terrain.h"
#include "root.h"

TerrainState *terrain_init() {
	TerrainState *state = MemAlloc(sizeof(TerrainState));

	shader_ar_init(&state->ar_shader, RES_PATH "terrain.fs");

  state->texture = LoadTexture(RES_PATH "tex2.png");
  state->render_texture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

	return state;
}
void terrain_step(TerrainState *state) {
	shader_ar_step(&state->ar_shader);

	BeginDrawing();
		ClearBackground(BLACK);

		// draw shader into texture
		BeginTextureMode(state->render_texture);
			BeginShaderMode(state->ar_shader.shader);
				//DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
				DrawTexture(state->texture, GetScreenWidth() / 2 - state->texture.width / 2, 0, WHITE);
			EndShaderMode();
		EndTextureMode();

		DrawTextureRec(state->render_texture.texture,
									 (Rectangle){0, 0, state->render_texture.texture.width,
															 -state->render_texture.texture.height},
									 (Vector2){0, 0}, Fade(WHITE, 0.5));
	EndDrawing();
}
void terrain_dispose(TerrainState *state) {
  UnloadShader(state->ar_shader.shader); 
	UnloadTexture(state->texture);
	UnloadRenderTexture(state->render_texture);
}
