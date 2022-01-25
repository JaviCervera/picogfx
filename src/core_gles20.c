#include <string.h>
#include "../include/geom.h"
#include "core_gles20.h"
#include "glad_gles20/include/glad/glad.h"
#define STB_DS_IMPLEMENTATION
#include "stb/stb_ds.h"

void _core_enablestates();
void _core_setviewport(int viewport_x, int viewport_y, int viewport_width, int viewport_height, int color);

bool_t core_init(load_proc_t proc) {
  return gladLoadGLES2Loader((GLADloadproc)proc);
}

void core_prepare(int viewport_x, int viewport_y, int viewport_width, int viewport_height, int color) {
  _core_enablestates();
  _core_setviewport(viewport_x, viewport_y, viewport_width, viewport_height, color);
}

void _core_enablestates() {
  glEnable(GL_BLEND);
  glEnable(GL_SCISSOR_TEST);
  glDepthFunc(GL_LEQUAL);
  glFrontFace(GL_CW);
  _core_setblendmode(bm_solid);
  _core_setculling(TRUE);
  _core_setdepthtest(TRUE);
  _core_setdepthwrite(TRUE);
}

void _core_setviewport(int viewport_x, int viewport_y, int viewport_width, int viewport_height, int color) {
  glViewport(viewport_x, viewport_y, viewport_width, viewport_height);
  glScissor(viewport_x, viewport_y, viewport_width, viewport_height);
  if (color != 0) {
    glClearColor(((color >> 16) & 0xff) / 255.0f, ((color >> 8) & 0xff) / 255.0f, (color & 0xff) / 255.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}

void _core_setblendmode(enum blendmode_t blend) {
  switch (blend) {
  case bm_solid:
    glBlendFunc(GL_ONE, GL_ZERO);
    break;
  case bm_alpha:
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    break;
  case bm_add:
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    break;
  case bm_multiply:
    glBlendFunc(GL_DST_COLOR, GL_ZERO);
    break;
  }
}

void _core_setculling(bool_t enable) {
  if (enable) glEnable(GL_CULL_FACE);
  else glDisable(GL_CULL_FACE);
}

void _core_setdepthtest(bool_t enable) {
  if (enable) glEnable(GL_DEPTH_TEST);
  else glDisable(GL_DEPTH_TEST);
}

void _core_setdepthwrite(bool_t enable) {
  glDepthMask(enable);
}
