cmake_minimum_required(VERSION 3.21)
project(UFBX_TEST)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)


include_directories("base_lib/glad/include")
include_directories("base_lib/")
include_directories("graphic_engine/")
add_library(glad STATIC "base_lib/glad/src/glad.c")
add_executable("main.out"
    "main.cpp"
    "base_lib/ufbx/ufbx.c"
    "graphic_engine/glsys_core.cpp"
    "graphic_engine/shader_loader.cpp"
    "graphic_engine/glsys_camera.cpp"
    "graphic_contents_main.cpp"
    "graphic_engine/ggmath/sgg_vec.c"
    "ufbx_gl_loader.c"
)
target_link_libraries("main.out" PRIVATE glad glfw OpenGL::GL OpenGL::GLU m)
