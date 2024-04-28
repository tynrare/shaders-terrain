#include "scatter.h"
#include "external/raygui.h"
#include "gui.h"
#include "noise_tex.h"
#include "root.h"
#include <raylib.h>

static void scatter_shader_init_uniforms(ScatterState *state) {
  Shader shader = state->ar_shader.shader;
  int gridscale = GetShaderLocation(shader, "gridscale");
  SetShaderValue(shader, gridscale, &state->gridscale, SHADER_UNIFORM_FLOAT);
  int tex_sheet = GetShaderLocation(shader, "tex_sheet");
  SetShaderValueTexture(shader, tex_sheet, state->sheet);
  int sheet_w = GetShaderLocation(shader, "sheet_w");
  SetShaderValue(shader, sheet_w, &state->sheet_w, SHADER_UNIFORM_FLOAT);
  int sheet_h = GetShaderLocation(shader, "sheet_h");
  SetShaderValue(shader, sheet_h, &state->sheet_h, SHADER_UNIFORM_FLOAT);
  int scatter_amount = GetShaderLocation(shader, "scatter_amount");
  SetShaderValue(shader, scatter_amount, &state->scatter_amount,
                 SHADER_UNIFORM_FLOAT);
  int scatter_scale = GetShaderLocation(shader, "scatter_scale");
  SetShaderValue(shader, scatter_scale, &state->scatter_scale,
                 SHADER_UNIFORM_FLOAT);
  int spritemask_scale = GetShaderLocation(shader, "spritemask_scale");
  SetShaderValue(shader, spritemask_scale, &state->spritemask_scale,
                 SHADER_UNIFORM_FLOAT);
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

  state->sheet = LoadTexture(RES_PATH "tex5.png");
  state->texture = NoiseTexGenerate(w, h);
  state->gridscale = 32;
  state->sheet_w = 2;
  state->sheet_h = 3;
  state->scatter_amount = 0.5;
  state->scatter_scale = 0.5;
  state->spritemask_scale = 1.5;
  state->shadermode = 1;

  scatter_shader_init_uniforms(state);

  return state;
}

static void draw_sliders(ScatterState *state) {
  GuiSlider((Rectangle){GUI_PADDING, GUI_PADDING, 128, 16}, NULL,
            TextFormat("gridscale (%0.2f)", state->gridscale),
            &state->gridscale, 1, 32);
  GuiSlider(
      (Rectangle){GUI_PADDING, GUI_PADDING + 16 * 1 + GUI_GAP * 1, 128, 16},
      NULL, TextFormat("amount (%0.2f)", state->scatter_amount),
      &state->scatter_amount, 0, 1);
  GuiSlider(
      (Rectangle){GUI_PADDING, GUI_PADDING + 16 * 2 + GUI_GAP * 2, 128, 16},
      NULL, TextFormat("scale (%0.2f)", state->scatter_scale), &state->scatter_scale, 0.1, 1.0);
  GuiSlider(
      (Rectangle){GUI_PADDING, GUI_PADDING + 16 * 3 + GUI_GAP * 3, 128, 16},
      NULL, TextFormat("mask size (%0.2f)", state->spritemask_scale), &state->spritemask_scale, 0.5, 2.0);
  GuiSlider((Rectangle){GUI_PADDING, GUI_PADDING + (16 + GUI_GAP) * 4, 128, 16},
            NULL, "mode", &state->shadermode, 0, 3.0);
}

static void draw_shader(ScatterState *state) {
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

void scatter_step(ScatterState *state) {
  draw_shader(state);
  draw_sliders(state);
  scatter_shader_init_uniforms(state);

  ShaderAutoReloadStatus ar_status = shader_ar_step(&state->ar_shader);
  if (ar_status == SHADER_AR_STATUS_RELOADED) {
    // .. on reload
  }
}

void scatter_dispose(ScatterState *state) {
  if (!state) {
    return;
  }

  UnloadShader(state->ar_shader.shader);
  UnloadTexture(state->texture);
  UnloadRenderTexture(state->render_texture);
}
