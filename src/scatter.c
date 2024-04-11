#include "scatter.h"
#include "external/raygui.h"
#include "gui.h"
#include "root.h"
#include <raylib.h>

static void scatter_shader_init_uniforms(ScatterState *state) {
  Shader shader = state->ar_shader.shader;
  int noise_location = GetShaderLocation(shader, "tex_noise0");
  SetShaderValueTexture(shader, noise_location, state->tex_noise0);
  int gridscale = GetShaderLocation(shader, "gridscale");
  SetShaderValue(shader, gridscale, &state->gridscale, SHADER_UNIFORM_FLOAT);
  int tex_sheet = GetShaderLocation(shader, "tex_sheet");
  SetShaderValueTexture(shader, tex_sheet, state->sheet);
  int sheet_w = GetShaderLocation(shader, "sheet_w");
  SetShaderValue(shader, sheet_w, &state->sheet_w, SHADER_UNIFORM_FLOAT);
  int sheet_h = GetShaderLocation(shader, "sheet_h");
  SetShaderValue(shader, sheet_h, &state->sheet_h, SHADER_UNIFORM_FLOAT);
  int scatter_amount = GetShaderLocation(shader, "scatter_amount");
  SetShaderValue(shader, scatter_amount, &state->scatter_amount, SHADER_UNIFORM_FLOAT);
  int scatter_scale = GetShaderLocation(shader, "scatter_scale");
  SetShaderValue(shader, scatter_scale, &state->scatter_scale, SHADER_UNIFORM_FLOAT);
}

ScatterState *scatter_init() {
  ScatterState *state = MemAlloc(sizeof(ScatterState));

  shader_ar_init(&state->ar_shader, RES_PATH "scatter.fs");

  state->render_texture =
      LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

  // fracCoords won't work properly without fullsize texture
	// using simple square texture
  int w = GetScreenHeight();
  int h = GetScreenHeight();
  Color *pixels = (Color *)MemAlloc(w * h * sizeof(Color));
  Image image = {.data = pixels, // We can assign pixels directly to data
                 .width = w,
                 .height = h,
                 .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
                 .mipmaps = 1};
  Texture2D texture = LoadTextureFromImage(image);
  UnloadImage(image); // Unload CPU (RAM) image data (pixels)

  state->sheet = LoadTexture(RES_PATH "tex5.png");
  state->tex_noise0 = LoadTexture(RES_PATH "tex_noise2.png");
  state->texture = texture;
  state->gridscale = 10;
  state->sheet_w = 2;
  state->sheet_h = 3;
  state->scatter_amount = 0.5;
  state->scatter_scale = 1;

  scatter_shader_init_uniforms(state);

  return state;
}

void scatter_step(ScatterState *state) {
  GuiSlider((Rectangle){GUI_PADDING, GUI_PADDING, 128, 16}, NULL, "gridscale",
            &state->gridscale, 1, 10);
  GuiSlider(
      (Rectangle){GUI_PADDING, GUI_PADDING + 16 * 1 + GUI_GAP * 1, 128, 16},
      NULL, "amount", &state->scatter_amount, 0, 1);
  GuiSlider(
      (Rectangle){GUI_PADDING, GUI_PADDING + 16 * 2 + GUI_GAP * 2, 128, 16},
      NULL, "scale", &state->scatter_scale, 1, 1.5);

  scatter_shader_init_uniforms(state);

  ShaderAutoReloadStatus ar_status = shader_ar_step(&state->ar_shader);
  if (ar_status == SHADER_AR_STATUS_RELOADED) {
    // .. on reload
  }

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

void scatter_dispose(ScatterState *state) {
  UnloadShader(state->ar_shader.shader);
  UnloadTexture(state->texture);
  UnloadRenderTexture(state->render_texture);
}