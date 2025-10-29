#include "glsys_camera.hpp"
Basic_Camera3D_Control::Basic_Camera3D_Control():first_pos_check{true}, yaw{-90.0f}, pitch{0.0f}, speed{10.0f}, sens{0.1f}, last_x{0.0f}, last_y{0.0f} {
    assignment_vector3_xyz(&c_pos, 0.0f, 0.0f, 20.0f);
    assignment_vector3_xyz(&c_front, 0.0f, 0.0f, -1.0f);
    assignment_vector3_xyz(&c_up, 0.0f, 1.0f, 0.0f);    
}
SGG_Vector3 Basic_Camera3D_Control::get_camera_center(){
    SGG_Vector3 center;
    assignment_vector3_vector3(&center, &c_pos);
    add_vector3_vector3(&center, &c_front);
    return center;
}

void Basic_Camera3D_Control::mouse_camera_program(double mx, double my){

    if (first_pos_check) {
        last_x = mx;
        last_y = my;
        first_pos_check = false;
        return;
    }
    SGG_Vector2 offset_pos = {(float)(mx - last_x), (float)(last_y - my)}; 
    last_x = mx; last_y = my;
    offset_pos.x *= sens;
    offset_pos.y *= sens;
    add_yaw(offset_pos.x);
    add_pitch(offset_pos.y);
    const float yaw_r   = get_yaw_radius();
    const float pitch_r = get_pitch_radius();
    assignment_vector3_xyz(&c_front,std::cos(yaw_r) * std::cos(pitch_r), std::sin(pitch_r), std::sin(yaw_r) * std::cos(pitch_r));
}

void Basic_Camera3D_Control::move_camera(Basic_GLFW_Manager & glfw_manager, float correction_value){
    constexpr float dash_scalar = 2.5f;
    float result_speed = speed;
    if(glfw_manager.gl_sys_push_key(GLFW_KEY_LEFT_CONTROL)) result_speed *= dash_scalar;

    const float sv = result_speed * correction_value;
    SGG_Vector3 right_vector3;
    cross_sgg_vector3(&right_vector3, &c_front, &c_up);
    if(glfw_manager.gl_sys_push_key(GLFW_KEY_W)){
        auto tmp_gv = c_front;
        mul_vector3(&tmp_gv, sv);
        add_vector3_vector3(&c_pos, &tmp_gv);
    }
    else if(glfw_manager.gl_sys_push_key(GLFW_KEY_S)){
        auto tmp_gv = c_front;
        mul_vector3(&tmp_gv, sv);
        sub_vector3_vector3(&c_pos, &tmp_gv);
    }
    else if(glfw_manager.gl_sys_push_key(GLFW_KEY_D)){
        auto tmp_gv = right_vector3;
        mul_vector3(&tmp_gv, sv);
        add_vector3_vector3(&c_pos, &tmp_gv);
    }
    else if(glfw_manager.gl_sys_push_key(GLFW_KEY_A)){
        auto tmp_gv = right_vector3;
        mul_vector3(&tmp_gv, sv);
        sub_vector3_vector3(&c_pos, &tmp_gv);
    }
    else if(glfw_manager.gl_sys_push_key(GLFW_KEY_Q)){
        auto tmp_gv = c_up;
        mul_vector3(&tmp_gv, sv);
        add_vector3_vector3(&c_pos, &tmp_gv);
    }
    else if(glfw_manager.gl_sys_push_key(GLFW_KEY_Z)){
        auto tmp_gv = c_up;
        mul_vector3(&tmp_gv, sv);
        sub_vector3_vector3(&c_pos, &tmp_gv);
    }
}