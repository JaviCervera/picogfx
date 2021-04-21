#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include "../include/picogfx.h"
#include "glfw3/include/GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "util.h"

using namespace picogfx;

void Update();
Geom* LoadObj(const char* filename);

GLFWwindow* window = NULL;
float projection[16];
float modelView[16];
float view[16];
float model[16];
RenderData* renderData = NULL;
Geom* geom = NULL;
Texture* tex = NULL;
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
    glfwSetWindowTitle(window, "PicoGFX OBJ Loading");
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
    Shader* shader = Shader::Create(vertex, fragment, shaderError, sizeof(shaderError));
    free(vertex);
    free(fragment);
    if (!shader) {
        strcat(shaderError, "\n");
        puts(shaderError);
        return -1;
    }

    // Load OBJ
    geom = LoadObj("data/skull.obj");
    if (geom == NULL) return -1;

    // Load texture
    int w, h;
    unsigned char *data = stbi_load("data/skull.jpg", &w, &h, NULL, 4);
    tex = Texture::Create(w, h, TextureFilter::TRILINEAR);
    tex->SetPixels(data);
    stbi_image_free(data);

    // Create render data
    renderData = RenderData::Create(*shader);
    renderData->BindMat4("Projection", projection);
    renderData->BindMat4("ModelView", modelView);
    renderData->BindTexture("Texture", (const Texture**)&tex);

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
    glfwGetWindowSize(window, &width, &height);
    Core::Get().SetPerspective(60, float(width) / height, 1, 1000, projection);
    Core::Get().SetView(0, 0, -4, 0, 0, 0, view);
    Core::Get().SetTransform(0, 0, 0, 0, angle, 0, 1, 1, 1, model);
    Core::Get().MulMatrices(view, model, modelView);
    Core::Get().Prepare(0, 0, width, height, 0xff000044);

    // Render geom
    geom->Render(*renderData, false);

    // Swap screen buffers
    glfwSwapBuffers(window);

    // Poll events
    glfwPollEvents();
}

Geom* LoadObj(const char* filename) {
    // Load obj
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename);
    if (!ret) {
        puts(err.c_str());
        return NULL;
    }

    // Parse vertices and indices from first shape
    std::vector<Vertex> vertices(attrib.vertices.size());
    std::vector<unsigned short> indices;
    const tinyobj::shape_t& shape = shapes[0];
    size_t indexOffset = 0;
    for (int f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
        const int numIndices = shape.mesh.num_face_vertices[f];
        for (int i = 0; i < numIndices; ++i) {
            const tinyobj::index_t idx = shape.mesh.indices[indexOffset + i];
            const tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
            const tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
            const tinyobj::real_t vz = -attrib.vertices[3*idx.vertex_index+2];
            const tinyobj::real_t nx = idx.normal_index > -1 ? attrib.normals[3*idx.normal_index+0] : 0;
            const tinyobj::real_t ny = idx.normal_index > -1 ? attrib.normals[3*idx.normal_index+1] : 0;
            const tinyobj::real_t nz = idx.normal_index > -1 ? -attrib.normals[3*idx.normal_index+2] : -1;
            const tinyobj::real_t tx = 1;
            const tinyobj::real_t ty = 0;
            const tinyobj::real_t tz = 0;
            const tinyobj::real_t u = idx.texcoord_index > -1 ? attrib.texcoords[2*idx.texcoord_index+0] : 0;
            const tinyobj::real_t v = idx.texcoord_index > -1 ? -attrib.texcoords[2*idx.texcoord_index+1] : 0;
            vertices[idx.vertex_index] = Vertex(vx, vy, vz, nx, ny, nz, tx, ty, tz, 0xffffffff, u, v);
        }
        indices.push_back(shape.mesh.indices[indexOffset + 0].vertex_index);
        indices.push_back(shape.mesh.indices[indexOffset + 2].vertex_index);
        indices.push_back(shape.mesh.indices[indexOffset + 1].vertex_index);
        indexOffset += numIndices;
    }
    return Geom::Create(&vertices[0], vertices.size(), &indices[0], indices.size());
}
