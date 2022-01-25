#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#include <stdio.h>
#include <string.h>
#include "../include/picogfx.h"
#include "glfw3/include/GLFW/glfw3.h"
#include "util.h"

void update();

GLFWwindow* window = NULL;
float projection[16];
float model_view[16];
float view[16];
float model[16];
struct renderdata_t* renderdata = NULL;
struct geom_t* geom = NULL;
double last_time = 0.0;
float delta_time = 0.0f;
float angle = 0.0f;

int main(int argc, char* argv[]) {
  /* init glfw */
  if (!glfwInit()) {
    puts("could not init glfw.\n");
    return -1;
  }

  /* create window */
  glfwWindowHint(GLFW_SAMPLES, 8);
  glfwWindowHint(GLFW_RESIZABLE, TRUE);
  window = glfwCreateWindow(800, 600, "", NULL, NULL);
  if (!window) {
    puts("could not open window.\n");
    glfwTerminate();
    return -1;
  }
  glfwSetWindowTitle(window, "picogfx triangle");
  glfwMakeContextCurrent(window);

  /* init picogfx */
  if (!core_init((load_proc_t)glfwGetProcAddress)) {
      puts("could not init picogfx.\n");
      glfwTerminate();
      return -1;
  }

  /* create shader */
  char shader_error[256];
  char* vertex = str_load("data/vertexcolor.vs.glsl");
  char* fragment = str_load("data/vertexcolor.fs.glsl");
#ifndef __APPLE__
  fragment = str_prefix(fragment, "precision mediump float;", TRUE);
#endif
  struct shader_t* shader = shader_create(vertex, fragment, shader_error, sizeof(shader_error));
  free(vertex);
  free(fragment);
  if (!shader) {
    strcat(shader_error, "\n");
    puts(shader_error);
    return -1;
  }

  /* create render data */
  renderdata = renderdata_create(shader);
  renderdata_bindmat4(renderdata, "projection", projection);
  renderdata_bindmat4(renderdata, "model_view", model_view);

  /* create triangle */
  struct vertex_t vertices[3];
  vertex_init(&vertices[0], -1, -1, 0, 0xffff0000, 0, 0);
  vertex_init(&vertices[1], 0, 1, 0, 0xff00ff00, 0, 0);
  vertex_init(&vertices[2], 1, -1, 0, 0xff0000ff, 0, 0);
  const unsigned short indices[] = {0, 1, 2};
  geom = geom_create(rm_triangles, vertices, 3, indices, 3);

  /* main loop */
  last_time = glfwGetTime();
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(update, 0, TRUE);
#else
  while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    update();
  }
  glfwTerminate();
#endif
  return 0;
}

void update() {
  /* update delta time */
  delta_time = (float)(glfwGetTime() - last_time);
  last_time = glfwGetTime();

  /* update angle */
  angle += 90 * delta_time;

  /* prepare for drawing */
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  math_setperspective(60, width * 1.0f / height, 1, 1000, projection);
  math_setview(0, 0, -4, 0, 0, 0, view);
  math_settransform(0, 0, 0, 0, angle, 0, 1, 1, 1, model);
  math_mulmatrices(view, model, model_view);
  core_prepare(0, 0, width, height, 0xff000044);

  /* render geom */
  geom_render(geom, renderdata, bm_solid, FALSE, TRUE, TRUE);

  /* swap screen buffers */
  glfwSwapBuffers(window);

  /* poll events */
  glfwPollEvents();
}
