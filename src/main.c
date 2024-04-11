/*******************************************************************************************
 *
 *   raylib [shaders] example - Hot reloading
 *
 *   NOTE: This example requires raylib OpenGL 3.3 for shaders support and only
 *#version 330 is currently supported. OpenGL ES 2.0 platforms are not supported
 *at the moment.
 *
 *   Example originally created with raylib 3.0, last time updated with
 *raylib 3.5
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an
 *OSI-certified, BSD-like license that allows static linking with closed source
 *software
 *
 *   Copyright (c) 2020-2024 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include "godrays.h"
#include "terrain.h"
#include "scatter.h"
#include "noise_tex.h"
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "external/raygui.h"
#undef RAYGUI_IMPLEMENTATION 
#include "gui.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

typedef enum SHOWCASE_MODES {
  SHOWCASE_MODE_GODRAYS = 0,
  SHOWCASE_MODE_TERRAIN,
	SHOWCASE_MODE_SCATTER,
	SHOWCASE_MODE_NOISE_TEX,
  __SHOWCASE_MODES_COUNT
} SHOWCASE_MODES;

SHOWCASE_MODES mode = SHOWCASE_MODE_NOISE_TEX;

GodraysState *godrays_state = NULL;
TerrainState *terrain_state = NULL;
ScatterState *scatter_state = NULL;
NoiseTexState *noisetex_state = NULL;

bool active = false;

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

void step(void) {
  if (!active) {
#if defined(PLATFORM_WEB)
    emscripten_cancel_main_loop();
#endif
    return;
  }

	BeginDrawing();
		draw();
	EndDrawing();
}

void dispose() {
	godrays_dispose(godrays_state);
	terrain_dispose(terrain_state);
	scatter_dispose(scatter_state);
	noisetex_dispose(noisetex_state);
}

void init() {
	dispose();

	// lvl 0
  godrays_state = godrays_init();
  terrain_state = terrain_init();
  scatter_state = scatter_init();

	// lvl 1
  noisetex_state = noisetex_init();
}

int main(void) {
  const int screenWidth = 0x320;
  const int screenHeight = 0x1c2;
	const int seed = 2;
	const int max = 255;

  InitWindow(screenWidth, screenHeight, "noisetex");
  SetTargetFPS(seed); // Set our game to run at 60 frames-per-second
	SetRandomSeed(seed);

	init();

  active = true;

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(step, 0, 1);
#else
  SetTargetFPS(60);

  while (!WindowShouldClose() && active) {
    step();
  } // Detect window close button or ESC key
#endif

	dispose();
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
