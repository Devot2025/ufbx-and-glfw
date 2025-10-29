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
include_directories("graphic_api/")
add_library(glad STATIC "base_lib/glad/src/glad.c")
add_executable("main.out"
    "main.cpp"
    "base_lib/ufbx/ufbx.c"
    "graphic_api/glsys_core.cpp"
    "graphic_api/shader_loader.cpp"
    "graphic_api/glsys_camera.cpp"
    "graphic_contents_main.cpp"
    "graphic_api/ggmath/sgg_vec.c"
)
target_link_libraries("main.out" PRIVATE glad glfw OpenGL::GL OpenGL::GLU m)
