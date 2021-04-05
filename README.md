# ![PicoGFX](logo.png) PicoGFX

A small OpenGL based graphics library for C++.

## Building with Emscripten

```bash
em++ examples/<example_name>.cpp src/glad_gles20/src/glad.c src/*.cpp -o _build/<example_name>.html --preload-file _build/data@/data -Isrc/glad_gles20/include -s USE_GLFW=3 -fno-exceptions -fno-rtti
cd _build
python3 -m http.server
```
