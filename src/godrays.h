#include "shader_autoreload.h"

typedef struct GodraysState {
  Texture2D texture;
  RenderTexture render_texture;
	ShaderAutoReloadState ar_shader;
} GodraysState;

GodraysState *godrays_init();
void godrays_step(GodraysState *state);
void godrays_dispose(GodraysState *state);
