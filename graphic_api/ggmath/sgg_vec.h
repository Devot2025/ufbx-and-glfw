#ifndef _SGG_VEC_H_
#define _SGG_VEC_H_
#include <math.h>
#include <float.h>
#include <stdint.h>
#include "c_header_helper.hint"
typedef struct SGG_Vector2{ float x; float y;}SGG_Vector2;
typedef struct SGG_Vector3{ float x; float y; float z;}SGG_Vector3;

typedef struct SGG_Mat3{float m[3 * 3];}SGG_Mat3;
typedef struct SGG_Mat4{float m[4 * 4];}SGG_Mat4;
_EXTERN_C_BEGIN_

float calc_radius(float degree);
void cross_vector3(SGG_Vector3 * dst_vec, const SGG_Vector3 * src_1, const SGG_Vector3 * src_2);
void normalize_vector3(SGG_Vector3 * dst_vec3);

void assignment_vector3_vector3(SGG_Vector3 * vec3, SGG_Vector3 * a_vec3);
void assignment_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az);
void assignment_vector3(SGG_Vector3 * vec3, const float ass_vec);

void add_vector3_vector3(SGG_Vector3 * vec3, SGG_Vector3 * a_vec3);
void add_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az);
void add_vector3(SGG_Vector3 * vec3, const float add_vec);

void sub_vector3_vector3(SGG_Vector3 * vec3, SGG_Vector3 * a_vec3);
void sub_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az);
void sub_vector3(SGG_Vector3 * vec3, const float sub_vec);

void mul_vector3_vector3(SGG_Vector3 * vec3, SGG_Vector3 * a_vec3);
void mul_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az);
void mul_vector3(SGG_Vector3 * vec3, const float mul_vec);

void div_vector3_vector3(SGG_Vector3 * vec3, SGG_Vector3 * a_vec3);
void div_vector3_xyz(SGG_Vector3 * vec3, float ax, float ay, float az);
void div_vector3(SGG_Vector3 * vec3, const float div_vec);

void cross_sgg_vector3(SGG_Vector3 * vec3, const SGG_Vector3 * src_a, const SGG_Vector3 * src_b);
void sgg_perspective(SGG_Mat4 * dst_per, float vertical_rad, float aspect, float near, float far);
void sgg_lookAt(SGG_Mat4 * dst_mat4, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
_EXTERN_C_END_
#endif