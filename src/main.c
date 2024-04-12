// @tynroar

#include "gui.h"
#include <raylib.h>
#include "godrays.h"
#include "terrain.h"
#include "scatter.h"
#include "noise_tex.h"

bool active = false;

int viewport_w = 0x320;
int viewport_h = 0x1c2;

#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"
#undef RAYGUI_IMPLEMENTATION 

typedef enum SHOWCASE_MODES {
  SHOWCASE_MODE_GODRAYS = 0,
  SHOWCASE_MODE_TERRAIN,
	SHOWCASE_MODE_SCATTER,
	SHOWCASE_MODE_NOISE_TEX,
  __SHOWCASE_MODES_COUNT
} SHOWCASE_MODES;

GodraysState *godrays_state = NULL;
TerrainState *terrain_state = NULL;
ScatterState *scatter_state = NULL;

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

NoiseTexState *noisetex_state = NULL;

SHOWCASE_MODES mode = SHOWCASE_MODE_NOISE_TEX;

void draw() {
	ClearBackground(BLACK);
  switch (mode) {
  default:
  case SHOWCASE_MODE_GODRAYS:
    godrays_step(godrays_state);
    break;
  case SHOWCASE_MODE_TERRAIN:
    terrain_step(terrain_state);
    break;
  case SHOWCASE_MODE_SCATTER:
    scatter_step(scatter_state);
    break;
  case SHOWCASE_MODE_NOISE_TEX:
    noisetex_step(noisetex_state);
    break;
  }

  if (GuiButton((Rectangle) {GUI_ALIGN_RIGHT(100), GUI_ALIGN_BOTTOM(16), 100, 16}, "#76#Next demo (N)") || IsKeyPressed(KEY_N)) {
    mode = (mode + 1) % __SHOWCASE_MODES_COUNT;
  }
}

void dispose() {
	godrays_dispose(godrays_state);
	terrain_dispose(terrain_state);
	scatter_dispose(scatter_state);
	noisetex_dispose(noisetex_state);
}

void init() {
	dispose();

	// lvl 1
  noisetex_state = noisetex_init();

	// lvl 0
  godrays_state = godrays_init();
  terrain_state = terrain_init();
  scatter_state = scatter_init();
}

void equilizer() {
	const int vw = GetScreenWidth();
	const int vh = GetScreenHeight();
	if (viewport_w != vw || viewport_h != vh) {
		viewport_w = vw;
		viewport_h = vh;
		init();
	}
}

void step(void) {
	equilizer();

	BeginDrawing();
		draw();
	EndDrawing();
}

void loop() {
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(step, 0, 1);
#else

  while (!WindowShouldClose()) {
    step();
  } 
#endif
}

int main(void) {
	const int seed = 2;
	const int max = 255;

  InitWindow(viewport_w, viewport_h, "noisetex");
  SetTargetFPS(max); // Set our game to run at 60 frames-per-second
	SetRandomSeed(seed);

	init();

  active = true;
	loop();

	dispose();
  CloseWindow(); 

  return 0;
}
