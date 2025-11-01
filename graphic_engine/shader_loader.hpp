#ifndef _SHADER_LOADER_HPP_
#define _SHADER_LOADER_HPP_
#include <iostream>
#include <fstream>
#include "glsys.hint"

class Basic_Shader_Loader{
    public:
    GLuint build_shader_program(GLuint *shader_ids, GLuint src_num);
    GLuint gen_shader_file_to_id(const char * shader_source_path, GLenum shader_type);
    GLuint shader_program_gpu_complier(const char * shader_source_code, GLenum shader_type);
    GLenum change_shader_type(GLenum shader_type);
    void confirm_shader_build(GLuint shader_id, GLenum basic_shader_type);
};
class GL_Shader_Loader : private Basic_Shader_Loader{
    public:
	GLuint gen_program_shader_id(const char * vert_file_path,const char * frag_file_path);
	void create_vert_frag_shader_id(GLuint & shader_vert_id, GLuint & shader_frag_id, const char * vert_file_path,const char *frag_file_path);
};
class Basic_GL_Program_Shader_Manager{
    public:

	Basic_GL_Program_Shader_Manager(const char * vert_file_path,const char * frag_file_path);
	
	~Basic_GL_Program_Shader_Manager();
	GLuint prog_shader;
};
#endif