@echo off

echo # Creating folders for CMake
mkdir _CMAKE
cd _CMAKE
mkdir _GLFW
cd ..

echo # Building GLFW
cd _CMAKE/_GLFW
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_INSTALL=OFF ../../examples/glfw3
mingw32-make
cd ../..

echo # Building examples
gcc -O2 -Isrc/glad_gles20/include -L_CMAKE/_GLFW/src -o _build/triangle.exe examples/triangle.c src/glad_gles20/src/glad.c src/*.c -lglfw3 -lgdi32 -limm32 -mwindows
gcc -O2 -Isrc/glad_gles20/include -L_CMAKE/_GLFW/src -o _build/texture.exe examples/texture.c src/glad_gles20/src/glad.c src/*.c -lglfw3 -lgdi32 -limm32 -mwindows
g++ -O2 -fno-exceptions -fno-rtti -Isrc/glad_gles20/include -L_CMAKE/_GLFW/src -o _build/objloader.exe examples/objloader.cpp src/glad_gles20/src/glad.c src/*.c -lglfw3 -lgdi32 -limm32 -mwindows
g++ -O2 -fno-exceptions -fno-rtti -Isrc/glad_gles20/include -DIMGUI_IMPL_OPENGL_ES2 -L_CMAKE/_GLFW/src -o _build/dir_light.exe examples/dir_light.cpp examples/imgui/*.cpp src/glad_gles20/src/glad.c src/*.c -lglfw3 -lgdi32 -limm32 -mwindows

echo # Done
pause
