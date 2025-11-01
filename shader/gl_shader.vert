
#version 330 core

/*static bone array start*/
#define BONE_NUM 128

layout(std140) uniform skinVert {
    mat4 bones[BONE_NUM];
};
/*static bone array end*/

uniform sampler2D skin_bones;

#define TEX_APPER_LINER_DATA_WIDTH 4

vec4 get_tex_apper_unique_data(int index){
    return texelFetch(skin_bones, ivec2(index & (TEX_APPER_LINER_DATA_WIDTH - 1), index / TEX_APPER_LINER_DATA_WIDTH), 0);
}

mat4 get_tex_apper_data_mat4(int index, int size, int base){
    int buf_index = base + index * size;
    return mat4(
        get_tex_apper_unique_data(buf_index + 0),
        get_tex_apper_unique_data(buf_index + 1),
        get_tex_apper_unique_data(buf_index + 2),
        get_tex_apper_unique_data(buf_index + 3)
    );
}
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 t_normal;
layout (location = 2) in vec2 t_uv;
layout (location = 3) in float t_indices;
layout (location = 4) in uvec4 boneInd;
layout (location = 5) in vec4 boneWeight;
uniform mat4 model;

uniform mat4 view;
uniform mat4 projection;
uniform int static_check;
out vec3 f_normal;
out vec4 f_worldPos;

mat4 get_skin_main(){
    int check = 0;
    if(check == 0){
        mat4 m0 = get_tex_apper_data_mat4(int(boneInd.x), 4, 0) * boneWeight.x;
        mat4 m1 = get_tex_apper_data_mat4(int(boneInd.y), 4, 0) * boneWeight.y;
        mat4 m2 = get_tex_apper_data_mat4(int(boneInd.z), 4, 0) * boneWeight.z;
        mat4 m3 = get_tex_apper_data_mat4(int(boneInd.w), 4, 0) * boneWeight.w;
        return m0 + m1 + m2 + m3;
    }
    else{
        return bones[boneInd.x] * boneWeight.x
        + bones[boneInd.y] * boneWeight.y
        + bones[boneInd.z] * boneWeight.z
        + bones[boneInd.w] * boneWeight.w;
    }
}
void dynamic_mesh_program(){

    mat4 boneDef = get_skin_main();
    
    f_worldPos = (boneDef * vec4(a_pos, 1.0));
    
    mat3 nmat = mat3(transpose(inverse(boneDef)));
    
    f_normal = normalize(nmat * t_normal);
    
    gl_Position = projection * view * f_worldPos;
}
void static_mesh_program(){
    f_worldPos = vec4(a_pos, 1.0);
    f_normal   = normalize(t_normal);
    //f_uv       = t_uv;
    gl_Position = projection * view * f_worldPos;
}
void main() {
    if(static_check == 0)dynamic_mesh_program();
    else static_mesh_program();
}