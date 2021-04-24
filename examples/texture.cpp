#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#include <stdio.h>
#include <string.h>
#include "../include/picogfx.h"
#include "glfw3/include/GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "util.h"

using namespace picogfx;

void Update();

GLFWwindow* window = NULL;
float projection[16];
float modelView[16];
float view[16];
float model[16];
Texture* tex = NULL;
RenderData* renderData = NULL;
Geom* geom = NULL;
double lastTime = 0.0;
float deltaTime = 0.0f;
float angle = 0.0f;

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
    glfwSetWindowTitle(window, "PicoGFX Texture");
    glfwMakeContextCurrent(window);

    // Init PicoGfx
    if (!Core::Get().Init((Core::LoadProc)glfwGetProcAddress)) {
        puts("Could not init PicoGFX.\n");
        glfwTerminate();
        return -1;
    }

    // Create shader
    char shaderError[256];
    char* vertex = LoadString("data/textured.vs.glsl");
    char* fragment = LoadString("data/textured.fs.glsl");
#ifndef __APPLE__
    fragment = PrefixString(fragment, "precision mediump float;");
#endif
    Shader* shader = Shader::Create(vertex, fragment, shaderError, sizeof(shaderError));
    free(vertex);
    free(fragment);
    if (!shader) {
        strcat(shaderError, "\n");
        puts(shaderError);
        return -1;
    }

    // Load texture
    int w, h;
    unsigned char *data = stbi_load("data/texture.jpg", &w, &h, NULL, 4);
    tex = Texture::Create(w, h, TextureFilter::TRILINEAR);
    tex->SetPixels(data);
    stbi_image_free(data);

    // Create render data
    renderData = RenderData::Create(*shader);
    renderData->BindMat4("Projection", projection);
    renderData->BindMat4("ModelView", modelView);
    renderData->BindTexture("Texture", (const Texture**)&tex);

    // Create quad
    const Vertex vertices[] = {
        Vertex(-1, -1, 0, 0xffffffff, 0, 1),
        Vertex(-1, 1, 0, 0xffffffff, 0, 0),
        Vertex(1, 1, 0, 0xffffffff, 1, 0),
        Vertex(1, -1, 0, 0xffffffff, 1, 1)
    };
    const unsigned short indices[] = {0, 1, 2, 3};
    geom = Geom::Create(TRIANGLE_FAN, vertices, 4, indices, 4);

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

    // Prepare for drawing
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    Core::Get().SetPerspective(60, float(width) / height, 1, 1000, projection);
    Core::Get().SetView(0, 0, -4, 0, 0, 0, view);
    Core::Get().SetTransform(0, 0, 0, 0, angle, 0, 1, 1, 1, model);
    Core::Get().MulMatrices(view, model, modelView);
    Core::Get().Prepare(0, 0, width, height, 0xff000044);

    // Render geom
    geom->Render(*renderData, SOLID, false);

    // Swap screen buffers
    glfwSwapBuffers(window);

    // Poll events
    glfwPollEvents();
}
