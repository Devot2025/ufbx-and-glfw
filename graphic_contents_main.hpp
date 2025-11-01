#ifndef _GRAPHIC_CONTENTS_MAIN_HPP_
#define _GRAPHIC_CONTENTS_MAIN_HPP
#include "graphic_engine/glsys_core.hpp"
#include "graphic_engine/glsys_camera.hpp"
#include "graphic_engine/glsys_standard_obj_ele.hpp"
#include "ufbx_gl_loader.h"
typedef enum Main_Shader_Var_Id{
    E_Main_Shader_Projection,
    E_Main_Shader_Model,
    E_Main_Shader_View_Pos,
    E_Main_Shader_Mesh_Pos,
    E_Main_Shader_Mesh_Color,
    E_Main_Shader_Light_Pos,
    E_Main_Shader_Light_Color,
    E_Main_Shader_Skin_Bones,
    E_Main_Shader_Static_Check,

}Main_Shader_Var_Id;
class Test_Main_Loop : public GL_Loop_Manager{
    public:
    Emulate_Uniform_2D_Data<E_Main_Shader_Skin_Bones> emulated_bone;
    Cursor_Manager test_cursor_manager;
    Program_Shader_Operator main_shader_manager;
    Object_Color_Manager<E_Main_Shader_Mesh_Color> obj_color;
    Object_Light_3D<E_Main_Shader_Light_Color, E_Main_Shader_Light_Pos> test_light;
    Test_Main_Loop();
    int init_loop()override;
    int main_loop()override;
    int end_loop()override;
};
#endif