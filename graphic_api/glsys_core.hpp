#ifndef _GL_SYS_CORE_HPP_
#define _GL_SYS_CORE_HPP_
#include "glsys.hint"
#include "shader_loader.hpp"
#include <iostream>
#include <vector>
class Basic_Cursor_Manager{
    public:
    virtual void cursor_program_main(double cx, double cy) = 0;
};
class Program_Shader_Operator{
    public:
    std::vector<GLuint> prog_shader_var_ids;
    Program_Shader_Operator(const char * vert_file_path,const char * frag_file_path, GLuint shader_var_num);
    void register_shader_var_name_id(GLuint shader_var_name_id, const char * shader_var_name);
    inline void begin_program_shader(){glUseProgram(program_shader_manager.prog_shader);}
    inline void upload_shader_uniform_v3_data(GLuint shader_var_name_id, float x, float y, float z){glUniform3f(prog_shader_var_ids[shader_var_name_id], x, y, z);}
    inline void upload_shader_uniform_v1_data(GLuint shader_var_name_id, int i){glUniform1i(prog_shader_var_ids[shader_var_name_id], i);}
    inline void upload_shader_uniform_v1_data(GLuint shader_var_name_id, float i){glUniform1f(prog_shader_var_ids[shader_var_name_id], i);}
    inline void upload_shader_uniform_mat4_data(GLuint shader_var_name_id, float * m4){glUniformMatrix4fv(prog_shader_var_ids[shader_var_name_id], 1, GL_FALSE, m4);}
    
    private:
    Basic_GL_Program_Shader_Manager program_shader_manager;
};
class Basic_GLFW_Manager{
    protected:
    GLFWwindow * glwin = nullptr;
    static inline Basic_Cursor_Manager * cursor_manager;
    static inline void glfw_cursor_program_call_back(GLFWwindow * tool_glwin, double x, double y){
        cursor_manager->cursor_program_main(x,y);
    }
    public:
    inline void set_cursor_basic_cursor(Basic_Cursor_Manager & src_cursor_manager){
        cursor_manager = &src_cursor_manager;
        glfw_cursor_program_start();
    }
    inline void glfw_cursor_program_start(){
		glfwSetCursorPosCallback(glwin, glfw_cursor_program_call_back);
    }
    inline bool gl_sys_push_key(int key){return glfwGetKey(glwin, key) == GLFW_PRESS;}
    inline bool gl_sys_push_mouse(int mouse_button){return glfwGetMouseButton(glwin, mouse_button) == GLFW_PRESS;}
	inline void get_render_cursor_pos(double & dst_cursor_x, double& dst_cursor_y)const {glfwGetCursorPos(glwin, &dst_cursor_x, &dst_cursor_y);}

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
class GL_Sys_Main_Manager{
    public:
    GLFW_Manager glfw_manage;
	void ini_gl_core();
    explicit GL_Sys_Main_Manager(int width, int height, const char * name);
    ~GL_Sys_Main_Manager();
    int run_gl_loop(GL_Loop_Manager & loop_manager);
};

#endif