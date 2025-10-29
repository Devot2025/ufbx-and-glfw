#ifndef _GLSYS_STANDARD_OBJ_ELE_HPP_
#define _GLSYS_STANDARD_OBJ_ELE_HPP_
#include "ggmath/sgg_vec.h"
#include "glsys_core.hpp"
template <GLuint static_check_id>
static inline void use_static_check(Program_Shader_Operator & prog_shader_ope, int static_check){
    prog_shader_ope.upload_shader_uniform_v1_data(static_check_id, static_check);
}
template <GLuint color_id>
class Object_Color_Manager{
    public:
    SGG_Vector3 color;
    Object_Color_Manager();
    SGG_Vector3 & operator () (float vec_a);
    void use_obj_color(Program_Shader_Operator & prog_shader_ope);
};

template <GLuint color_id>
SGG_Vector3 & Object_Color_Manager<color_id>::operator()(float vec_a){
    assignment_vector3(&color, vec_a);
    return color;
}
template <GLuint color_id>
Object_Color_Manager<color_id>::Object_Color_Manager(){
    assignment_vector3(&color, 0.0f);
}
template <GLuint color_id>
void Object_Color_Manager<color_id>::use_obj_color(Program_Shader_Operator & prog_shader_ope){
    prog_shader_ope.upload_shader_uniform_v3_data(color_id, color.x, color.y, color.z);
}

template <GLuint pos_id>
class Object_Pos_Manager{
    public:
    SGG_Vector3 pos;
    SGG_Vector3 & operator () (float vec_a);
    SGG_Vector3 & operator () (float vec_x, float vec_y, float vec_z);
    Object_Pos_Manager();
    void use_obj_pos(Program_Shader_Operator & prog_shader_ope);
};

template <GLuint pos_id>
SGG_Vector3 & Object_Pos_Manager<pos_id>::operator()(float vec_a){
    assignment_vector3(&pos, vec_a);
    return pos;
}

template <GLuint pos_id>
SGG_Vector3 & Object_Pos_Manager<pos_id>::operator()(float vec_x, float vec_y, float vec_z){
    pos.x = vec_x;
    pos.y = vec_y;
    pos.z = vec_z;
    return pos;
}
template <GLuint pos_id>
Object_Pos_Manager<pos_id>::Object_Pos_Manager(){
    assignment_vector3(&pos, 0.0f);
}
template <GLuint pos_id>
void Object_Pos_Manager<pos_id>::use_obj_pos(Program_Shader_Operator & prog_shader_ope){
    prog_shader_ope.upload_shader_uniform_v3_data(pos_id, pos.x, pos.y, pos.z);
}

template <GLuint color_id, GLuint pos_id>
class Object_Light_3D{
    public:
    Object_Pos_Manager<pos_id> pos_manager;
    Object_Color_Manager<color_id> color_manager;
};

#endif