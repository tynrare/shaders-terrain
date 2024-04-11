#include "shader_autoreload.h"

typedef struct NoiseTexState {
  Texture2D texture;
  RenderTexture render_texture;
	ShaderAutoReloadState ar_shader;
} NoiseTexState;

NoiseTexState *noisetex_init();
void noisetex_step(NoiseTexState *state);
void noisetex_dispose(NoiseTexState *state);
