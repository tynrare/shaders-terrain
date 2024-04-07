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

typedef enum SHOWCASE_MODES {
	SHOWCASE_MODE_GODRAYS = 0,
	SHOWCASE_MODE_TERRAIN,
	__SHOWCASE_MODES_COUNT
} SHOWCASE_MODES; 

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
	const int screenWidth = 800;
	const int screenHeight = 450;
	SHOWCASE_MODES mode = SHOWCASE_MODE_GODRAYS;

  // Initialization
  //--------------------------------------------------------------------------------------
  InitWindow(screenWidth, screenHeight,
             "tynroar shaders terrain");

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------
	
	GodraysState *godrays_state = godrays_init();
	TerrainState *terrain_state = terrain_init();

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			mode = (mode + 1) % __SHOWCASE_MODES_COUNT;
		}

		switch(mode) {
			default:
			case SHOWCASE_MODE_GODRAYS:
				godrays_step(godrays_state);
				break;
			case SHOWCASE_MODE_TERRAIN:
				terrain_step(terrain_state);
				break;
		}

  }

	godrays_dispose(godrays_state);
	terrain_dispose(terrain_state);

  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
