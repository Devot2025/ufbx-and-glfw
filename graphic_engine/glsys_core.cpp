#include "glsys_core.hpp"


Program_Shader_Operator::Program_Shader_Operator(const char * vert_file_path,const char * frag_file_path,  GLuint shader_var_num) : program_shader_manager(vert_file_path, frag_file_path){
    prog_shader_var_ids.resize(shader_var_num);
}
void Program_Shader_Operator::register_shader_var_name_id(GLuint shader_var_name_id, const char * shader_var_name){
    prog_shader_var_ids[shader_var_name_id] = glGetUniformLocation(program_shader_manager.prog_shader, shader_var_name);
}
Basic_Gl_Texture::Basic_Gl_Texture() : tex_id{}, tex_size{} {

}
void Basic_Gl_Texture::gen_gl_texture(){
    glGenTextures(1, &tex_id);
}
Basic_Gl_Texture::~Basic_Gl_Texture(){glDeleteTextures(1, &tex_id);}
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



GL_Sys_Main_Manager::GL_Sys_Main_Manager(int width, int height, const char * name) : glfw_manage(width, height, name){
    glfw_manage.ini_gl_core();
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::runtime_error("error : failed to load glad");
    
}
GL_Sys_Main_Manager::~GL_Sys_Main_Manager(){
    glfwTerminate();
}



int GL_Sys_Main_Manager::run_gl_loop(GL_Loop_Manager & loop_manager){
    loop_manager.glfw_sub_manager.set_glfw_manager(glfw_manage);
    
    if(!loop_manager.init_loop())return -1;

    while(glfw_manage.glfw_run()){
        if(!loop_manager.main_loop()) break;
        glfw_manage.glfw_require_process();
    }
    if(!loop_manager.end_loop()) return -1;
    return 0;
}