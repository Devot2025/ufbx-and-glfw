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

class Basic_Gl_Texture{
	public:
	GLuint tex_id;
	int tex_size;
	Basic_Gl_Texture();
	~Basic_Gl_Texture();
    void gen_gl_texture();
	virtual inline void start_texture(){}
    
};
template <GLuint u2d_id, int u2d_data_width = 4>
class Emulate_Uniform_2D_Data : Basic_Gl_Texture{
    public:
    Emulate_Uniform_2D_Data(int ini_size);
    void gen_emulated_u2d_data(int data_size);
    bool start_emulated_u2d_data(void * emulated_u2d, int emulated_u2d_size, Program_Shader_Operator & prog_shader_ope);
    bool start_emulated_u2d_data(void * emulated_u2d, Program_Shader_Operator & prog_shader_ope);
    
    private:
    void booked_uniform_2_data(int data_size);
    void standard_emulated_u2d_data_opt();
};
template <GLuint u2d_id, int u2d_data_width>
void Emulate_Uniform_2D_Data<u2d_id, u2d_data_width>::gen_emulated_u2d_data(int data_size){
    if(data_size <= 0)return;
    if(!tex_id)glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    standard_emulated_u2d_data_opt();
    booked_uniform_2_data(data_size);
}

template <GLuint u2d_id, int u2d_data_width>
bool Emulate_Uniform_2D_Data<u2d_id, u2d_data_width>::start_emulated_u2d_data(void * emulated_u2d, int emulated_u2d_size, Program_Shader_Operator & prog_shader_ope){
    if(!emulated_u2d)return false;
    glBindTexture(GL_TEXTURE_2D, tex_id);
    booked_uniform_2_data(emulated_u2d_size);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, u2d_data_width, tex_size, GL_RGBA, GL_FLOAT, emulated_u2d);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    constexpr int dummy_upload_data = 0;
    prog_shader_ope.upload_shader_uniform_v1_data(u2d_id, dummy_upload_data);
    return true;
}

template <GLuint u2d_id, int u2d_data_width>
bool Emulate_Uniform_2D_Data<u2d_id, u2d_data_width>::start_emulated_u2d_data(void * emulated_u2d, Program_Shader_Operator & prog_shader_ope){
    if(!emulated_u2d)return false;
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, u2d_data_width, tex_size, GL_RGBA, GL_FLOAT, emulated_u2d);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    constexpr int dummy_upload_data = 0;
    prog_shader_ope.upload_shader_uniform_v1_data(u2d_id, dummy_upload_data);
    return true;
}
template <GLuint u2d_id, int u2d_data_width>
Emulate_Uniform_2D_Data<u2d_id, u2d_data_width>::Emulate_Uniform_2D_Data(int ini_size){
    if(ini_size <= 0)ini_size = 64;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    standard_emulated_u2d_data_opt();
    booked_uniform_2_data(ini_size);
}
template <GLuint u2d_id, int u2d_data_width>
void Emulate_Uniform_2D_Data<u2d_id, u2d_data_width>::booked_uniform_2_data(int data_size){
    if(tex_size == data_size) return;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, u2d_data_width, data_size, 0, GL_RGBA, GL_FLOAT, nullptr);
    tex_size = data_size;   
}
template <GLuint u2d_id, int u2d_data_width>
void Emulate_Uniform_2D_Data<u2d_id, u2d_data_width>::standard_emulated_u2d_data_opt(){
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
}
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