#include "shader_autoreload.h"

typedef struct ScatterState {
  Texture2D texture;
  Texture2D sheet;
  RenderTexture render_texture;
	ShaderAutoReloadState ar_shader;
	float gridscale;
	float scatter_amount;
	float scatter_scale;
	float sheet_w;
	float sheet_h;
} ScatterState;

ScatterState *scatter_init();
void scatter_step(ScatterState *state);
void scatter_dispose(ScatterState *state);
