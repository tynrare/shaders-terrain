#include "terrain.h"
#include "external/raygui.h"
#include "gui.h"
#include "root.h"
#include <raylib.h>

static void terrain_shader_init_uniforms(TerrainState *state) {
  Shader shader = state->ar_shader.shader;
  int noise_location = GetShaderLocation(shader, "tex_noise0");
  SetShaderValueTexture(shader, noise_location, state->tex_noise0);
  int noise1_location = GetShaderLocation(shader, "tex_noise1");
  SetShaderValueTexture(shader, noise1_location, state->tex_noise1);
  int tile_location = GetShaderLocation(shader, "tilesize");
  SetShaderValue(shader, tile_location, &state->tilesize, SHADER_UNIFORM_FLOAT);
  int zoom_location = GetShaderLocation(shader, "zoom");
  SetShaderValue(shader, zoom_location, &state->zoom, SHADER_UNIFORM_FLOAT);
  int tex1_location = GetShaderLocation(shader, "texture_mix");
  SetShaderValueTexture(shader, tex1_location, state->texture1);
  int tex2_location = GetShaderLocation(shader, "texture_mix1");
  SetShaderValueTexture(shader, tex2_location, state->texture2);
  int erosion_location = GetShaderLocation(shader, "erosion");
  SetShaderValue(shader, erosion_location, &state->erosion, SHADER_UNIFORM_FLOAT);
}

TerrainState *terrain_init() {
  TerrainState *state = MemAlloc(sizeof(TerrainState));

  shader_ar_init(&state->ar_shader, RES_PATH "terrain.fs");

  state->texture = LoadTexture(RES_PATH "tex2.png");
  state->texture1 = LoadTexture(RES_PATH "tex3.png");
  state->texture2 = LoadTexture(RES_PATH "tex4.png");
  state->tex_noise0 = LoadTexture(RES_PATH "tex_noise0.png");
  state->tex_noise1 = LoadTexture(RES_PATH "tex_noise1.png");
  state->render_texture =
      LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
  state->tilesize = 1;
  state->zoom = 1;
  state->erosion = 0.5;

  terrain_shader_init_uniforms(state);

  return state;
}

void terrain_step(TerrainState *state) {
  // if (GuiButton((Rectangle){0, 0, 16, 16}, "#76#Next demo")) { /* ACTION */ }
  GuiSlider((Rectangle){GUI_PADDING, GUI_PADDING, 128, 16}, NULL, "tilesize",
            &state->tilesize, 1, 3);
  GuiSlider(
      (Rectangle){GUI_PADDING, GUI_PADDING + 16 * 1 + GUI_GAP * 1, 128, 16},
      NULL, "zoom", &state->zoom, 1, 3);
  GuiSlider(
      (Rectangle){GUI_PADDING, GUI_PADDING + 16 * 2 + GUI_GAP * 2, 128, 16},
      NULL, "erosion", &state->erosion, 0, 1);

  terrain_shader_init_uniforms(state);

  ShaderAutoReloadStatus ar_status = shader_ar_step(&state->ar_shader);
  if (ar_status == SHADER_AR_STATUS_RELOADED) {
    // .. on reload
  }

  // draw shader into texture
  BeginTextureMode(state->render_texture);
		BeginShaderMode(state->ar_shader.shader);
		// DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
			DrawTexture(state->texture, GetScreenWidth() / 2 - state->texture.width / 2,
								0, WHITE);
		EndShaderMode();
  EndTextureMode();

  DrawTextureRec(state->render_texture.texture,
                 (Rectangle){0, 0, state->render_texture.texture.width,
                             -state->render_texture.texture.height},
                 (Vector2){0, 0}, WHITE);
}
void terrain_dispose(TerrainState *state) {
	if (!state) {
		return;
	}

  UnloadShader(state->ar_shader.shader);
  UnloadTexture(state->texture);
  UnloadTexture(state->texture1);
  UnloadTexture(state->texture2);
  UnloadTexture(state->tex_noise0);
  UnloadTexture(state->tex_noise1);
  UnloadRenderTexture(state->render_texture);
}
