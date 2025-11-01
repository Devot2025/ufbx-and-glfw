#include "graphic_contents_main.hpp"

Test_Main_Loop::Test_Main_Loop() : main_shader_manager("shader/gl_shader.vert", "shader/gl_shader.frag", 8), emulated_bone(64){
    
}
GL_Mesh_Data_Root * a_mesh;
int Test_Main_Loop::init_loop(){
    std::cout << "init loop call" << std::endl;
    test_light.color_manager(1.0f);
    test_light.pos_manager(0.0f, -20.0f, 40.0f);
    glfw_sub_manager.glfw_manager->set_cursor_basic_cursor(test_cursor_manager);

    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Projection, "projection");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_View_Pos, "view");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Skin_Bones, "skin_bones");
    
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Light_Pos, "lightPos");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Light_Color, "lightColor");
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Mesh_Color, "objectColor");
    
    main_shader_manager.register_shader_var_name_id(E_Main_Shader_Static_Check, "static_check");
    a_mesh = change_fbx_to_gl_mesh_style("a.fbx", 1.0f);
    
    
    return 1;
}
int Test_Main_Loop::end_loop(){
    return 1;
}
int Test_Main_Loop::main_loop(){
    glEnable(GL_DEPTH_TEST);
    glfw_sub_manager.render_base_color(0.0f, 1.0f, 1.0f);
    test_cursor_manager.camera_control.move_camera(* glfw_sub_manager.glfw_manager);
    test_cursor_manager.cursor_is_valid_program(* glfw_sub_manager.glfw_manager);
    SGG_Vector3 center = test_cursor_manager.camera_control.get_camera_center();
    main_shader_manager.begin_program_shader();

    Basic_Camera3D_Manager<E_Main_Shader_View_Pos, E_Main_Shader_Projection>::up_load_look_at(
        main_shader_manager, test_cursor_manager.camera_control.c_pos.x, test_cursor_manager.camera_control.c_pos.y, test_cursor_manager.camera_control.c_pos.z,
        center.x , center.y, center.z, test_cursor_manager.camera_control.c_up.x, test_cursor_manager.camera_control.c_up.y, test_cursor_manager.camera_control.c_up.z
    );

    Basic_Camera3D_Manager<E_Main_Shader_View_Pos, E_Main_Shader_Projection>::up_load_perspective(main_shader_manager, 45.0f, 1600.0f / 900.0f, 0.05f, 200.0f);
    obj_color.use_obj_color(main_shader_manager);
    test_light.use_light(main_shader_manager);

    use_static_check<E_Main_Shader_Static_Check>(main_shader_manager, 0);




    /*render a_mesh*/
    for(unsigned i = 0; i < a_mesh->meshes_size; ++i){
        auto &ms = a_mesh->meshes_table[i];
        for(unsigned j = 0; j < ms.parts_size; ++j){
            std::vector<SGG_Mat4> gm;
            /*upload skin bones*/
            for(unsigned int u = 0; u < ms.bone_maps[j].bone_size; ++u){
                GL_Node_Table * node = &a_mesh->nodes_table[ms.bone_maps[j].bone_indices[u]];
                SGG_Mat4 new_mat;
                mul_mat4_mat4(&new_mat, &node->node_to_world_deform, &ms.bone_maps[j].bone_mat4[u]);
                gm.push_back(new_mat);
            }
            emulated_bone.start_emulated_u2d_data((void *)gm.data(), ms.bone_maps[j].bone_size, main_shader_manager);
            
            /*render mesh*/
            glBindVertexArray(ms.mesh_parts[j].vao);
            glDrawElements(GL_TRIANGLES, ms.mesh_parts[j].vtn, GL_UNSIGNED_INT, 0);
        }
    }
    return 1;
}