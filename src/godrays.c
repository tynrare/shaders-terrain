#include "godrays.h"
#include "root.h"

GodraysState *godrays_init() {
  GodraysState *state = MemAlloc(sizeof(GodraysState));

  shader_ar_init(&state->ar_shader, RES_PATH "godrays.fs");

  state->texture = LoadTexture(RES_PATH "tex1.png");
  state->render_texture =
      LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

  return state;
}

void godrays_step(GodraysState *state) {
  // Update
  //----------------------------------------------------------------------------------
  shader_ar_step(&state->ar_shader);
  // Draw
  //----------------------------------------------------------------------------------

  // draw godrays into texture
  BeginTextureMode(state->render_texture);
		BeginShaderMode(state->ar_shader.shader);
		// DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
			DrawTexture(state->texture, GetScreenWidth() / 2 - state->texture.width / 2,
								0, WHITE);
		EndShaderMode();
  EndTextureMode();

  // draw original texture
  DrawTexture(state->texture, GetScreenWidth() / 2 - state->texture.width / 2,
              -30, WHITE);
  // blend godrays texture
  BeginBlendMode(BLEND_ADDITIVE);
		DrawTextureRec(state->render_texture.texture,
									 (Rectangle){0, 0, state->render_texture.texture.width,
															 -state->render_texture.texture.height},
									 (Vector2){0, 0}, Fade(WHITE, 0.5));
  EndBlendMode();

#ifdef _DEBUG
  DrawText(TextFormat("PRESS [A] to TOGGLE SHADER AUTOLOADING: %s",
                      state->ar_shader.shaderAutoReloading ? "AUTO" : "MANUAL"),
           10, 10, 10, state->ar_shader.shaderAutoReloading ? RED : BLACK);
  if (!state->ar_shader.shaderAutoReloading)
    DrawText("PRES [R] to SHADER RE-LOADING", 10, 30, 10, BLACK);

  DrawText(
      TextFormat("Shader last modification: %s",
                 asctime(localtime(&state->ar_shader.fragShaderFileModTime))),
      10, 430, 10, BLACK);
#endif

  //----------------------------------------------------------------------------------
}

void godrays_dispose(GodraysState *state) {
	if (!state) {
		return;
	}

  UnloadShader(state->ar_shader.shader);
  UnloadTexture(state->texture);
  UnloadRenderTexture(state->render_texture);
}
