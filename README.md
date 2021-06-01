# ![PicoGFX](logo.png) PicoGFX

A small OpenGL based graphics library for C++.

Project documentation is available [here](https://javicervera.github.io/picogfx/).

## Building with Emscripten

```bash
em++ -o _build/<example_name>.html examples/<example_name>.cpp examples/imgui/*.cpp src/glad_gles20/src/glad.c src/*.cpp --preload-file _build/data@/data -Isrc/glad_gles20/include -s USE_GLFW=3 -fno-exceptions -fno-rtti
cd _build
python3 -m http.server
```
