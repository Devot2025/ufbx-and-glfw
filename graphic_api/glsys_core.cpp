#include "glsys_core.hpp"
GLFW_Manager::GLFW_Manager(int width, int height, const char * name){
    if(!name) name = "exapmle gl";
    if(!glfwInit()){
        throw std::runtime_error("error : failed to glfw init");
    }
    glwin = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if(!glwin){
        throw std::runtime_error("error : failed to create glfw window");
    }
	glfwMakeContextCurrent(glwin);

}

GLFW_Manager::~GLFW_Manager(){
    glfwDestroyWindow(glwin);
}
bool GLFW_Manager::glfw_run(){
    return !glfwWindowShouldClose(glwin);
}

void GLFW_Manager::glfw_require_process(){
    glfwPollEvents();
    glfwSwapBuffers(glwin);
}



Gl_Sys_Main_Manager::Gl_Sys_Main_Manager(int width, int height, const char * name) : glfw_manage(width, height, name){
    glfw_manage.ini_gl_core();
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::runtime_error("error : failed to load glad");
    
}
Gl_Sys_Main_Manager::~Gl_Sys_Main_Manager(){
    glfwTerminate();
}



int Gl_Sys_Main_Manager::run_gl_loop(GL_Loop_Manager & loop_manager){
    loop_manager.glfw_sub_manager.set_glfw_manager(glfw_manage);
    
    if(!loop_manager.init_loop())return -1;

    while(glfw_manage.glfw_run()){
        if(!loop_manager.main_loop()) break;
        glfw_manage.glfw_require_process();
    }
    if(!loop_manager.end_loop()) return -1;
    return 0;
}