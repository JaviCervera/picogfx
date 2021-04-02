#include <stdio.h>
#include <string.h>
#include "../include/picogfx.h"
#include "glfw3/include/GLFW/glfw3.h"
#include <util.h>

using namespace picogfx;

int main(int argc, char* argv[]) {
    // Init GLFW
    if (!glfwInit()) {
        puts("Could not init GLFW.\n");
        return -1;
    }

    // Create window
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_RESIZABLE, true);
    GLFWwindow* window = glfwCreateWindow(800, 600, "", NULL, NULL);
    if (!window) {
        puts("Could not open window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Init PicoGfx
    if (!Core::Get().Init((Core::LoadProc)glfwGetProcAddress)) {
        puts("Could not init PicoGFX.\n");
        glfwTerminate();
        return -1;
    }

    // Define matrices
    float projection[16];
    float modelView[16];
    float view[16];
    float model[16];

    // Create shader
    char shaderError[256];
    Shader* shader = Shader::Create(LoadString("data/color.vs.glsl").c_str(), LoadString("color.fs.glsl").c_str(), shaderError, sizeof(shaderError));
    if (!shader) {
        strcat(shaderError, "\n");
        puts(shaderError);
        return -1;
    }

    // Create shader vars
    ShaderVars* vars = ShaderVars::Create(*shader);
    vars->SetMat4("Projection", projection);
    vars->SetMat4("ModelView", modelView);

    // Create triangle
    const Vertex vertices[] = {
        Vertex(-1, -1, 0, 0xffff0000),
        Vertex(0, 1, 0, 0xff00ff00),
        Vertex(1, -1, 0, 0xff0000ff)
    };
    const unsigned short indices[] = {0, 1, 2};
    Geom* geom = Geom::Create(vertices, 3, indices, 3);

    // Main loop
    double lastTime = glfwGetTime();
    float deltaTime = 0.0f;
    while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        // Update delta time
        deltaTime = float(glfwGetTime() - lastTime);
        lastTime = glfwGetTime();

        // Prepare for drawing
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        Core::Get().SetPerspective(60, float(width) / height, 1, 1000, projection);
        Core::Get().SetView(0, 0, -4, 0, 0, 0, view);
        Core::Get().SetTransform(0, 0, 0, 0, 0, 0, 1, 1, 1, model);
        Core::Get().MulMatrices(view, model, modelView);
        Core::Get().Prepare(0, 0, width, height, 0xff000088);

        // Draw geom
        geom->Draw(*vars);

        // Swap screen buffers
        glfwSwapBuffers(window);

        // Poll events
        glfwPollEvents();
    }

    // Shutdown
    glfwTerminate();
    return 0;
}
