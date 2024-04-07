#include <time.h> // Required for: localtime(), asctime()
#include "raylib.h"

#ifndef SHADER_AR_H
#define SHADER_AR_H


typedef struct ShaderAutoReloadState {
  const char *fragShaderFileName;
  time_t fragShaderFileModTime;
  Shader shader;
  int resolutionLoc;
  int mouseLoc;
  int timeLoc;
  float resolution[2];
  float totalTime;
  bool shaderAutoReloading;
} ShaderAutoReloadState;

void shader_ar_init(ShaderAutoReloadState *state, const char *filename);
void shader_ar_step(ShaderAutoReloadState *state);

#endif
