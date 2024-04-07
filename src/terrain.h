#include "shader_autoreload.h"

typedef struct TerrainState {
  Texture2D texture;
  RenderTexture render_texture;
	ShaderAutoReloadState ar_shader;
} TerrainState;

TerrainState *terrain_init();
void terrain_step(TerrainState *state);
void terrain_dispose(TerrainState *state);
