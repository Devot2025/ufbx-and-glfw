#ifndef _GLSYS_CAMERA_HPP_
#define _GLSYS_CAMERA_HPP_
#include "ggmath/sgg_vec.h"
#include "glsys_core.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

template <GLuint view_pos_id, GLuint perspective_id>
class Basic_Camera3D_Manager{
    
    public:
    
    static void up_load_look_at(Program_Shader_Operator & pso, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
    
    static void up_load_perspective(Program_Shader_Operator & pso, float vertical_rad, float aspect,float near, float far);

};
template <GLuint view_pos_id, GLuint perspective_id>
void Basic_Camera3D_Manager<view_pos_id, perspective_id>::up_load_look_at(Program_Shader_Operator & pso, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ){
    SGG_Mat4 lookAt_mat;
    sgg_lookAt(&lookAt_mat, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
    pso.upload_shader_uniform_mat4_data(view_pos_id, lookAt_mat.m);
}
template <GLuint view_pos_id, GLuint perspective_id>
void Basic_Camera3D_Manager<view_pos_id, perspective_id>::up_load_perspective(Program_Shader_Operator & pso, float vertical_rad, float aspect,float near, float far){
    SGG_Mat4 perspective_mat;
    sgg_perspective(&perspective_mat, vertical_rad, aspect, near, far);
    pso.upload_shader_uniform_mat4_data(perspective_id, perspective_mat.m);
}
class Basic_Camera3D_Control{
    public:
    SGG_Vector3 c_pos;
    SGG_Vector3 c_front;
    SGG_Vector3 c_up;
    float sens;
    float yaw;
    float pitch;
    float speed;
    bool first_pos_check;
    double last_x;
    double last_y;
    Basic_Camera3D_Control();
    SGG_Vector3 get_camera_center();
    void move_camera(Basic_GLFW_Manager & glfw_manager, float correction_value = 1 / 60.0f * 5.0f);
    void mouse_camera_program(double mx, double my);
    inline void add_yaw(float src_yaw){
        yaw += src_yaw;
        contain_yaw();
    }

    inline void add_pitch(float src_pitch){
        pitch += src_pitch;
        contain_pitch();
    }

    inline float get_yaw_radius(){return calc_radius(yaw);}
    inline float get_pitch_radius(){return calc_radius(pitch);}
    inline void contain_yaw(){
        if (std::abs(yaw) > 720.0f) yaw = std::fmod(yaw, 360.0f);
        
    }
    inline void contain_pitch(){
        pitch = std::clamp(pitch, -89.0f, 89.0f);
    }

};

class Cursor_Manager final : public Basic_Cursor_Manager{
    private:
    bool is_valid_cursor = false;

    void cursor_program_main(double cx, double cy) override{
        //std::cout << "now now" << is_valid_cursor << std::endl;

        if(!is_valid_cursor)return;
        camera_control.mouse_camera_program(cx, cy);
    }
    public:
    Basic_Camera3D_Control camera_control;
    void cursor_is_valid_program(Basic_GLFW_Manager & basic_glfw_manager){
        /**
         * push mouse button left
        */
        if(basic_glfw_manager.gl_sys_push_mouse(GLFW_MOUSE_BUTTON_LEFT)){
            is_valid_cursor = true;

        }
        else {
            is_valid_cursor = false;
            basic_glfw_manager.get_render_cursor_pos(camera_control.last_x, camera_control.last_y);
        }
    }
};
#endif