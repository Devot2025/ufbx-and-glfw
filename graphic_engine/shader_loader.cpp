#include "shader_loader.hpp"
#define BASIC_SHADER_TYPE_NONE -1
#define BASIC_SHADER_TYPE_VERTEX 0
#define BASIC_SHADER_TYPE_FRAGMENT 1
#define BASIC_SHADER_TYPE_PROGRAM 2
#define BUFFER_BYTE_9 256

GLuint Basic_Shader_Loader::build_shader_program(GLuint *shader_ids, GLuint src_num){
    GLuint prog_shader = glCreateProgram();
    for(GLuint i = 0u;i < src_num; ++i)glAttachShader(prog_shader,*(shader_ids + i));
    glLinkProgram(prog_shader);
    confirm_shader_build(BASIC_SHADER_TYPE_PROGRAM, prog_shader);
    return prog_shader;
}
GLuint Basic_Shader_Loader::gen_shader_file_to_id(const char * shader_code_path, GLenum shader_type){
    std::string code_buffer;
    std::ifstream f_loader_manager(shader_code_path, std::ios::binary);
    if(!f_loader_manager){
        std::cerr << "error : failed to open " << shader_code_path << " file.";
        return 0x0;
    }
    f_loader_manager.seekg(0, std::ios::end);
    size_t f_all_size = f_loader_manager.tellg();
    if(f_all_size <= 0x0){
        std::cerr << "error : failed to get " << shader_code_path << " file size.";
        return 0x0;
    }
    f_loader_manager.seekg(0, std::ios::beg);
    code_buffer.resize(f_all_size);
    f_loader_manager.read(&code_buffer[0], f_all_size);
    
    return shader_program_gpu_complier(code_buffer.c_str(), shader_type);
}
GLuint Basic_Shader_Loader::shader_program_gpu_complier(const char * shader_source_code, GLenum shader_type){
    GLuint shader_id = glCreateShader(shader_type);
    
    glShaderSource(shader_id, 1, (const GLchar **) & shader_source_code, nullptr);
    glCompileShader(shader_id);
    
    shader_type = change_shader_type(shader_type);

    if(shader_type == 999)return shader_id;
    
    confirm_shader_build(shader_id, shader_type);
    
    return shader_id;
}
GLenum Basic_Shader_Loader::change_shader_type(GLenum shader_type){
    switch(shader_type){
        case GL_VERTEX_SHADER: return BASIC_SHADER_TYPE_VERTEX;
        case GL_FRAGMENT_SHADER : return BASIC_SHADER_TYPE_FRAGMENT;
        std::cerr << "a type not int the list was used" << std::endl;
    }
    return 999;
}

void Basic_Shader_Loader::confirm_shader_build(GLuint shader_id, GLenum basic_shader_type){
    int build_success;
    char err_log_buffer[BUFFER_BYTE_9];
    const char * shader_type_str[3] = {"VERTEX", "FRAGMENT", "PROGRAM"};
    if(basic_shader_type == BASIC_SHADER_TYPE_PROGRAM){
        glGetProgramiv(shader_id, GL_LINK_STATUS, &build_success);
        if(!build_success){
            glGetProgramInfoLog(shader_id, BUFFER_BYTE_9, NULL, err_log_buffer);				
            std::cerr << shader_type_str[basic_shader_type] <<  " shader compile is "<< err_log_buffer << std::endl;
            return;
        }
    }
    else{
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &build_success);
        if(!build_success){
            glGetShaderInfoLog(shader_id, BUFFER_BYTE_9, NULL, err_log_buffer);
            std::cerr << shader_type_str[basic_shader_type] <<  " shader compile is "<< err_log_buffer << std::endl;
            return;
        }
    }
}

GLuint GL_Shader_Loader::gen_program_shader_id(const char * vert_file_path,const char * frag_file_path){
    GLuint shader_vert_id;
    GLuint shader_frag_id;
    create_vert_frag_shader_id(shader_vert_id, shader_frag_id, vert_file_path, frag_file_path);
    constexpr int standard_link_shader_num = 2;
    GLuint link_shaders[standard_link_shader_num] = {shader_vert_id, shader_frag_id};
    GLuint program_shader = this->build_shader_program(&link_shaders[0], standard_link_shader_num);
    glDeleteShader(shader_vert_id);
    glDeleteShader(shader_frag_id);
    return program_shader;
}
void GL_Shader_Loader::create_vert_frag_shader_id(GLuint & shader_vert_id, GLuint & shader_frag_id,const char * vert_file_path,const char *frag_file_path){

    shader_vert_id = this->gen_shader_file_to_id(vert_file_path, GL_VERTEX_SHADER);
    shader_frag_id = this->gen_shader_file_to_id(frag_file_path, GL_FRAGMENT_SHADER);
}
Basic_GL_Program_Shader_Manager::Basic_GL_Program_Shader_Manager(const char * vert_file_path,const char * frag_file_path){

    GL_Shader_Loader shader_manager;
    
    prog_shader = shader_manager.gen_program_shader_id(vert_file_path, frag_file_path);	
	
}
Basic_GL_Program_Shader_Manager::~Basic_GL_Program_Shader_Manager(){
    glDeleteProgram(prog_shader);
}