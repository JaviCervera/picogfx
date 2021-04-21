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
#include "load_obj.h"
#include "util.h"

using namespace picogfx;

void Update();
void Normalize(float x, float y, float z, float* out);

GLFWwindow* window = NULL;
float projection[16];
float modelView[16];
float view[16];
float model[16];
RenderData* renderData = NULL;
std::vector<Geom*> geoms;
double lastTime = 0.0;
float deltaTime = 0.0f;
float angle = 0.0f;
float lightDir[] = {1, 1, -1, 0};
float viewLightDir[3];
float diffuse[] = {1, 1, 0};
float ambient[] = {0.01f, 0.01f, 0.01f};
float emissive[] = {0, 0, 0};
float specular[] = {1, 1, 1};
int specularPower = 64;
float eyePos[] = {0, 4, -5};
float halfVector[] = {0, 0, 0, 0};
float viewHalfVector[3];

int main(int argc, char* argv[]) {
    // Init GLFW
    if (!glfwInit()) {
        puts("Could not init GLFW.\n");
        return -1;
    }

    // Create window
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_RESIZABLE, true);
    window = glfwCreateWindow(800, 600, "", NULL, NULL);
    if (!window) {
        puts("Could not open window.\n");
        glfwTerminate();
        return -1;
    }
    glfwSetWindowTitle(window, "PicoGFX Directional lighting");
    glfwMakeContextCurrent(window);

    // Init PicoGfx
    if (!Core::Get().Init((Core::LoadProc)glfwGetProcAddress)) {
        puts("Could not init PicoGFX.\n");
        glfwTerminate();
        return -1;
    }

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 100");

    // Create shader
    char shaderError[256];
    char* vertex = LoadString("data/dir_light.vs.glsl");
    char* fragment = LoadString("data/dir_light.fs.glsl");
    Shader* shader = Shader::Create(vertex, fragment, shaderError, sizeof(shaderError));
    free(vertex);
    free(fragment);
    if (!shader) {
        strcat(shaderError, "\n");
        puts(shaderError);
        return -1;
    }

    // Load OBJ
    geoms = LoadObj("data/teapot.obj");
    if (geoms.size() == 0) return -1;

    // Create render data
    renderData = RenderData::Create(*shader);
    renderData->BindMat4("Projection", projection);
    renderData->BindMat4("ModelView", modelView);
    renderData->BindVec3("LightDir", viewLightDir);
    renderData->BindVec3("Diffuse", diffuse);
    renderData->BindVec3("Ambient", ambient);
    renderData->BindVec3("Emissive", emissive);
    renderData->BindVec3("Specular", specular);
    renderData->BindInt("SpecPower", &specularPower);
    renderData->BindVec3("HalfVector", viewHalfVector);

    // Main loop
    lastTime = glfwGetTime();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(Update, 0, true);
#else
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        Update();
    }
    glfwTerminate();
#endif
    return 0;
}

void Update() {
    // Update delta time
    deltaTime = float(glfwGetTime() - lastTime);
    lastTime = glfwGetTime();

    // Update angle
    angle += 90 * deltaTime;

    // Update half vector
    Normalize(eyePos[0], eyePos[1], eyePos[2], halfVector);
    Normalize(lightDir[0] - halfVector[0], lightDir[1] - halfVector[1], lightDir[2] - halfVector[2], halfVector);

    // Prepare for drawing
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    Core::Get().SetPerspective(60, float(width) / height, 1, 1000, projection);
    Core::Get().SetView(eyePos[0], eyePos[1], eyePos[2], 30, 0, 0, view);
    Core::Get().SetTransform(0, 0, 0, 0, angle, 0, 1, 1, 1, model);
    Core::Get().MulMatrices(view, model, modelView);
    Core::Get().MulVecByMatrix(lightDir, view, viewLightDir);
    Core::Get().MulVecByMatrix(halfVector, view, viewHalfVector);
    Core::Get().Prepare(0, 0, width, height, 0xff000044);

    // Render geom
    for (std::vector<Geom*>::iterator it = geoms.begin(); it != geoms.end(); ++it) {
        (*it)->Render(*renderData, false);
    }

    // Start new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Render gui
    ImGui::Begin("Light", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::ColorEdit3("Diffuse", diffuse);
    ImGui::ColorEdit3("Ambient", ambient);
    ImGui::ColorEdit3("Emissive", emissive);
    ImGui::ColorEdit3("Specular", specular);
    ImGui::SliderInt("Specular Power", &specularPower, 1, 128);
    ImGui::SliderFloat("X", &lightDir[0], -1, 1);
    ImGui::SliderFloat("Y", &lightDir[1], -1, 1);
    ImGui::SliderFloat("Z", &lightDir[2], -1, 1);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap screen buffers
    glfwSwapBuffers(window);

    // Poll events
    glfwPollEvents();
}

void Normalize(float x, float y, float z, float* out) {
    const float sqlen = x*x + y*y + z*z;
    const float invLen = (sqlen != 0) ? (1.0 / sqrt(sqlen)) : 1;
    out[0] = x * invLen;
    out[1] = y * invLen;
    out[2] = z * invLen;
}
