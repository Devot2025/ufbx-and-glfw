#include "graphic_contents_main.hpp"
unsigned int gVAO = 0, gVBO = 0, gEBO = 0;
void create_simple_cube_mesh() {
    float vertices[] = {
        // pos             // normal          // uv
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f
    };

    unsigned int indices[] = {
        0,1,2, 2,3,0,  // back
        4,5,6, 6,7,4,  // front
        0,1,5, 5,4,0,  // bottom
        2,3,7, 7,6,2,  // top
        0,3,7, 7,4,0,  // left
        1,2,6, 6,5,1   // right
    };

    glGenVertexArrays(1, &gVAO);
    glGenBuffers(1, &gVBO);
    glGenBuffers(1, &gEBO);

    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void draw_simple_cube_mesh() {
    glBindVertexArray(gVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
Test_Main_Loop::Test_Main_Loop() : main_shader_manager("shader/gl_shader.vert", "shader/gl_shader.frag",8){
    
}
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
    create_simple_cube_mesh();
    
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
    test_light.color_manager.use_obj_color(main_shader_manager);
    test_light.pos_manager.use_obj_pos(main_shader_manager);
    use_static_check<E_Main_Shader_Static_Check>(main_shader_manager, 1);

    if(glfw_sub_manager.glfw_manager->gl_sys_push_key(GLFW_KEY_A)) std::cout << "hello world!" << std::endl;
    draw_simple_cube_mesh();
    return 1;
}