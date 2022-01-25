#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#include <stdio.h>
#include <string.h>
#include "../include/picogfx.h"
#include "glfw3/include/GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "obj_load.h"
#include "util.h"

void update();
void normalize(float x, float y, float z, float* out);

GLFWwindow* window = NULL;
float projection[16];
float model_view[16];
float view[16];
float model[16];
renderdata_t* renderdata = NULL;
std::vector<geom_t*> geoms;
double last_time = 0.0;
float delta_time = 0.0f;
float angle = 0.0f;
float light_dir[] = {1, 1, -1, 0};
float view_light_dir[3];
float diffuse[] = {1, 1, 0};
float ambient[] = {0.01f, 0.01f, 0.01f};
float emissive[] = {0, 0, 0};
float specular[] = {1, 1, 1};
int specular_power = 64;
float eye_pos[] = {0, 4, -5};
float half_vector[] = {0, 0, 0, 0};
float view_half_vector[3];

int main(int argc, char* argv[]) {
  // init glfw
  if (!glfwInit()) {
    puts("Could not init GLFW.\n");
    return -1;
  }

  // create window
  glfwWindowHint(GLFW_SAMPLES, 8);
  glfwWindowHint(GLFW_RESIZABLE, true);
  window = glfwCreateWindow(800, 600, "", NULL, NULL);
  if (!window) {
    puts("could not open window.\n");
    glfwTerminate();
    return -1;
  }
  glfwSetWindowTitle(window, "picogfx directional lighting");
  glfwMakeContextCurrent(window);

  // init picogfx
  if (!core_init((load_proc_t)glfwGetProcAddress)) {
    puts("could not init picogfx.\n");
    glfwTerminate();
    return -1;
  }

  // setup dear imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGuiIO &io = ImGui::GetIO();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __APPLE__
  ImGui_ImplOpenGL3_Init("#version 120");
#else
  ImGui_ImplOpenGL3_Init("#version 100");
#endif

  // create shader
  char shader_error[256];
  char* vertex = str_load("data/dir_light.vs.glsl");
  char* fragment = str_load("data/dir_light.fs.glsl");
#ifndef __APPLE__
  fragment = str_prefix(fragment, "precision mediump float;", true);
#endif
  shader_t* shader = shader_create(vertex, fragment, shader_error, sizeof(shader_error));
  free(vertex);
  free(fragment);
  if (!shader) {
    strcat(shader_error, "\n");
    puts(shader_error);
    return -1;
  }

  // load obj
  geoms = obj_load("data/teapot.obj");
  if (geoms.size() == 0) return -1;

  // create render data
  renderdata = renderdata_create(shader);
  renderdata_bindmat4(renderdata, "projection", projection);
  renderdata_bindmat4(renderdata, "model_view", model_view);
  renderdata_bindvec3(renderdata, "light_dir", view_light_dir);
  renderdata_bindvec3(renderdata, "diffuse", diffuse);
  renderdata_bindvec3(renderdata, "ambient", ambient);
  renderdata_bindvec3(renderdata, "emissive", emissive);
  renderdata_bindvec3(renderdata, "specular", specular);
  renderdata_bindint(renderdata, "spec_power", &specular_power);
  renderdata_bindvec3(renderdata, "half_vector", view_half_vector);

  // main loop
  last_time = glfwGetTime();
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(update, 0, true);
#else
  while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
    update();
  }
  glfwTerminate();
#endif
  return 0;
}

void update() {
  // update delta time
  delta_time = float(glfwGetTime() - last_time);
  last_time = glfwGetTime();

  // update angle
  angle += 90 * delta_time;

  // update half vector
  normalize(eye_pos[0], eye_pos[1], eye_pos[2], half_vector);
  normalize(light_dir[0] - half_vector[0], light_dir[1] - half_vector[1], light_dir[2] - half_vector[2], half_vector);

  // prepare for drawing
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  math_setperspective(60, float(width) / height, 1, 1000, projection);
  math_setview(eye_pos[0], eye_pos[1], eye_pos[2], 30, 0, 0, view);
  math_settransform(0, 0, 0, 0, angle, 0, 1, 1, 1, model);
  math_mulmatrices(view, model, model_view);
  math_mulvecbymatrix(light_dir, view, view_light_dir);
  math_mulvecbymatrix(half_vector, view, view_half_vector);
  core_prepare(0, 0, width, height, 0xff000044);

  // render geom
  for (std::vector<geom_t*>::iterator it = geoms.begin(); it != geoms.end(); ++it) {
    geom_render(*it, renderdata, bm_solid, true, true, true);
  }

  // start new imgui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // render gui
  ImGui::Begin("Light", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::ColorEdit3("Diffuse", diffuse);
  ImGui::ColorEdit3("Ambient", ambient);
  ImGui::ColorEdit3("Emissive", emissive);
  ImGui::ColorEdit3("Specular", specular);
  ImGui::SliderInt("Specular Power", &specular_power, 1, 128);
  ImGui::SliderFloat("X", &light_dir[0], -1, 1);
  ImGui::SliderFloat("Y", &light_dir[1], -1, 1);
  ImGui::SliderFloat("Z", &light_dir[2], -1, 1);
  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // swap screen buffers
  glfwSwapBuffers(window);

  // poll events
  glfwPollEvents();
}

void normalize(float x, float y, float z, float* out) {
  const float sqlen = x*x + y*y + z*z;
  const float inv_len = (sqlen != 0) ? (1.0 / sqrt(sqlen)) : 1;
  out[0] = x * inv_len;
  out[1] = y * inv_len;
  out[2] = z * inv_len;
}
