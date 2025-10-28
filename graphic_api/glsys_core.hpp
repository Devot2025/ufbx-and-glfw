#ifndef _GL_SYS_CORE_HPP_
#define _GL_SYS_CORE_HPP_
#include "glsys.hint"
#include "shader_loader.hpp"
#include <iostream>

class Basic_GLFW_Manager{
    protected:
    GLFWwindow * glwin = nullptr;
    public:
    inline bool gl_sys_push_key(int key){return glfwGetKey(this->glwin, key) == GLFW_PRESS;}
};

class GLFW_Manager : public Basic_GLFW_Manager{
    public:
    explicit GLFW_Manager(int width, int height, const char * name);

    static inline void ini_gl_core(){

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    inline const GLFWwindow * get_glwin(){return glwin;}
    bool glfw_run();
    void glfw_require_process();
    ~GLFW_Manager();
};
class GL_Sys_Sub_Manager{
    public:
    inline void render_base_color(GLclampf red, GLclampf green, GLclampf blue){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(red, green, blue, 1.0f);
    }
    inline void set_glfw_manager(GLFW_Manager & src_glfw_manager){ glfw_manager = &src_glfw_manager;}
    Basic_GLFW_Manager * glfw_manager;
};
class GL_Loop_Manager{
    public:
    virtual int init_loop() = 0;
    virtual int main_loop() = 0;
    virtual int end_loop() = 0;
    GL_Sys_Sub_Manager glfw_sub_manager;
};
class Gl_Sys_Main_Manager{
    public:
    GLFW_Manager glfw_manage;
	void ini_gl_core();
    explicit Gl_Sys_Main_Manager(int width, int height, const char * name);
    ~Gl_Sys_Main_Manager();
    int run_gl_loop(GL_Loop_Manager & loop_manager);
};
#endif