#!/bin/sh
cd `dirname $0`

echo "# Creating folders for CMake"
mkdir _CMAKE
mkdir _CMAKE/_GLFW

echo "# Building GLFW"
cd _CMAKE/_GLFW
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=MinSizeRel -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_INSTALL=OFF ../../examples/glfw3
make
cd ../..

echo "# Building examples"
g++ -Os -fno-exceptions -fno-rtti -Isrc/glad_gles20/include -L_CMAKE/_GLFW/src -o _build/triangle examples/triangle.cpp src/glad_gles20/src/glad.c src/*.cpp -lglfw3 -ldl -lpthread -lX11
g++ -Os -fno-exceptions -fno-rtti -Isrc/glad_gles20/include -L_CMAKE/_GLFW/src -o _build/texture examples/texture.cpp src/glad_gles20/src/glad.c src/*.cpp -lglfw3 -ldl -lpthread -lX11
g++ -Os -fno-exceptions -fno-rtti -Isrc/glad_gles20/include -L_CMAKE/_GLFW/src -o _build/objloader examples/objloader.cpp src/glad_gles20/src/glad.c src/*.cpp -lglfw3 -ldl -lpthread -lX11
g++ -Os -fno-exceptions -fno-rtti -Isrc/glad_gles20/include -DIMGUI_IMPL_OPENGL_ES2 -L_CMAKE/_GLFW/src -o _build/dir_light examples/dir_light.cpp examples/imgui/*.cpp src/glad_gles20/src/glad.c src/*.cpp -lglfw3 -ldl -lpthread -lX11

echo "# Done"
