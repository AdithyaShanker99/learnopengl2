cmake_minimum_required(VERSION 3.0.0)
project(learn_open_gl VERSION 0.0.1)

add_executable(learn_open_gl
    Headers/EBO.h 
    Headers/VBO.h 
    Headers/VAO.h 
    Headers/shaderClass.h 
    Headers/Textures.h
    Headers/Camera.h
    Headers/Mesh.h
    Headers/HeightMap.h
    src/glad.c
    src/main.cpp
    src/EBO.cpp
    src/VBO.cpp 
    src/VAO.cpp
    src/shaderClass.cpp  
    src/stb.cpp
    src/textures.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/HeightMap.cpp
)

target_link_libraries(learn_open_gl glfw)