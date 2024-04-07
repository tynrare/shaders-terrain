#include "shader_autoreload.h"

typedef struct TerrainState {
  Texture2D texture;
  Texture2D texture1;
  RenderTexture render_texture;
	ShaderAutoReloadState ar_shader;
	Texture2D tex_noise0;
	Texture2D tex_noise1;
	float tilesize;
	float zoom;
	float erosion;
} TerrainState;

TerrainState *terrain_init();
void terrain_step(TerrainState *state);
void terrain_dispose(TerrainState *state);
