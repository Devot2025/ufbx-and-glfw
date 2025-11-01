#include "sgg_vec.h"
#include <math.h>
float length_vector3(const SGG_Vector3 * src_vec3){
    return sqrt(
        src_vec3->x * src_vec3->x + 
        src_vec3->y * src_vec3->y + 
        src_vec3->z * src_vec3->z
    );
}
void normalize_vector3(SGG_Vector3 * dst_vec3){
    float v_len = length_vector3(dst_vec3);
    if(v_len >= FLT_MIN){
        v_len = 1.0f / v_len;
        mul_vector3(dst_vec3, v_len);
    }
    else assignment_vector3(dst_vec3, 0.0f);
}

float calc_radius(float degree){

    return degree * M_PI / 180.0f;
}
void assignment_vector3_vector3(SGG_Vector3 * vec3, SGG_Vector3 * a_vec3){
    vec3->x = a_vec3->x;
    vec3->y = a_vec3->y;
    vec3->z = a_vec3->z;
}
void assignment_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az){
    vec3->x = ax;
    vec3->y = ay;
    vec3->z = az;
}
void assignment_vector3(SGG_Vector3 * vec3, const float ass_vec){
    vec3->x = vec3->y = vec3->z = ass_vec;
}

void add_vector3_vector3(SGG_Vector3 * vec3, SGG_Vector3 * a_vec3){
    vec3->x += a_vec3->x;
    vec3->y += a_vec3->y;
    vec3->z += a_vec3->z;
}
void add_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az){
    vec3->x += ax;
    vec3->y += ay;
    vec3->z += az;
}
void add_vector3(SGG_Vector3 * vec3, const float add_vec){
    vec3->x += add_vec;
    vec3->y += add_vec;
    vec3->z += add_vec;
}
void sub_vector3_vector3(SGG_Vector3 * vec3, SGG_Vector3 * a_vec3){
    vec3->x -= a_vec3->x;
    vec3->y -= a_vec3->y;
    vec3->z -= a_vec3->z;
}

void sub_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az){
    vec3->x -= ax;
    vec3->y -= ay;
    vec3->z -= az;
}

void sub_vector3(SGG_Vector3 * vec3, const float sub_vec){
    vec3->x -= sub_vec;
    vec3->y -= sub_vec;
    vec3->z -= sub_vec;
}

void mul_vector3_vector3(SGG_Vector3 * vec3, SGG_Vector3 * a_vec3){
    vec3->x *= a_vec3->x;
    vec3->y *= a_vec3->y;
    vec3->z *= a_vec3->z;
}

void mul_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az){
    vec3->x *= ax;
    vec3->y *= ay;
    vec3->z *= az;
}

void mul_vector3(SGG_Vector3 * vec3, const float mul_vec){
    vec3->x *= mul_vec;
    vec3->y *= mul_vec;
    vec3->z *= mul_vec;
}

void div_vector3_vector3(SGG_Vector3 * vec3, SGG_Vector3 * a_vec3){
    vec3->x /= a_vec3->x;
    vec3->y /= a_vec3->y;
    vec3->z /= a_vec3->z;
}

void div_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az){
    vec3->x /= ax;
    vec3->y /= ay;
    vec3->z /= az;
}

void div_vector3(SGG_Vector3 * vec3, const float div_vec){
    vec3->x /= div_vec;
    vec3->y /= div_vec;
    vec3->z /= div_vec;
}


void cross_sgg_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * src_a, const SGG_Vector3 * src_b){
    vec3->x = src_a->y * src_b->z - src_a->z * src_b->y;
    vec3->y = src_a->z * src_b->x - src_a->x * src_b->z;
    vec3->z = src_a->x * src_b->y - src_a->y * src_b->x;
}
void sgg_perspective(SGG_Mat4 * dst_per, float vertical_rad, float aspect, float near, float far){

    //verticalRad = verticalRad * M_PI / 180.0f;
    float scale_coeff = 1.0f / tanf(vertical_rad / 2.0f);
    dst_per->m[0] = scale_coeff / aspect;
    for(unsigned int i = 1;i<10;++i)dst_per->m[i] = 0.0f;
    dst_per->m[5] = scale_coeff;
    dst_per->m[10] = (far + near)/(near - far);
    dst_per->m[11] = -1.0f;
    dst_per->m[12] = dst_per->m[13] = 0.0f;
    dst_per->m[14] = (2.0f * far * near) / (near - far);
    dst_per->m[15] = 0.0f;
}
void sgg_lookAt(SGG_Mat4 * dst_mat4, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ){
    SGG_Vector3 for_word = {centerX - eyeX, centerY - eyeY, centerZ - eyeZ};
    float for_word_normal = sqrtf(for_word.x * for_word.x + for_word.y * for_word.y + for_word.z * for_word.z);
    
    div_vector3(&for_word, for_word_normal);
    SGG_Vector3 side = {for_word.y * upZ - for_word.z * upY,for_word.z * upX - for_word.x * upZ,for_word.x * upY - for_word.y * upX};
    float side_normal = 1.0f/ sqrtf(side.x * side.x + side.y * side.y + side.z * side.z);
    mul_vector3(&side, side_normal);
    SGG_Vector3 up;
    cross_sgg_vector3(&up, &side, &for_word);
    
    /*rotation matrix*/
    dst_mat4->m[0] = side.x;   dst_mat4->m[1] = up.x;   dst_mat4->m[2]  = -for_word.x;   dst_mat4->m[3]  = 0.0f;
    dst_mat4->m[4] = side.y;   dst_mat4->m[5] = up.y;   dst_mat4->m[6]  = -for_word.y;   dst_mat4->m[7]  = 0.0f;
    dst_mat4->m[8] = side.z;   dst_mat4->m[9] = up.z;   dst_mat4->m[10] = -for_word.z;   dst_mat4->m[11] = 0.0f;
    
    /*move Matrix*/
    dst_mat4->m[12] = -(side.x * eyeX + side.y * eyeY + side.z * eyeZ);
    dst_mat4->m[13] = -(up.x * eyeX + up.y * eyeY + up.z * eyeZ);
    dst_mat4->m[14] = (for_word.x * eyeX + for_word.y * eyeY + for_word.z * eyeZ);
    dst_mat4->m[15] = 1.0f;
}