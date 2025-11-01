#ifndef _UFBX_GL_LOADER_H_
#define _UFBX_GL_LOADER_H_
#include "base_lib/ufbx/ufbx.h"
#include "graphic_engine/glsys.hint"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "src/ext_alloc.h"
#include "graphic_engine/ggmath/sgg_vec.h"
#define DIMENSION_XYZ 3

typedef struct GL_Skin_Attribute{
    GLushort bone_indices[4];

    GLubyte bone_weight[4];
}GL_Skin_Attribute;
typedef struct GL_Meshes_Bone_Maps{
    GLint * bone_indices;
    SGG_Mat4 * bone_mat4;
    GLuint bone_size;
}GL_Meshes_Bone_Maps;
typedef struct GL_Meshes_Parts{
    GLuint vao;
    GLuint vbo[3];
    GLuint vtn;
}GL_Meshes_Parts;
typedef struct GL_Meshes_Table{
    GL_Meshes_Parts * mesh_parts;
    GL_Meshes_Bone_Maps * bone_maps;

    GLuint parts_size;
}GL_Meshes_Table;
typedef struct GL_Node_Table{
    GLuint parent_id;
    SGG_Mat4 node_to_parent_deform;
    SGG_Mat4 node_to_world_deform;
}GL_Node_Table;
typedef struct GL_Mesh_Data_Root{
    GL_Meshes_Table * meshes_table;
    GL_Node_Table * nodes_table;
    GLuint nodes_size;
    GLuint meshes_size;
}GL_Mesh_Data_Root;
_EXTERN_C_BEGIN_
ufbx_scene * loading_fbx_file(const char * file_path, float mesh_unit_meter);
GL_Mesh_Data_Root * change_fbx_to_gl_mesh_style(const char * fbx_file_path, float mesh_unit_meter);
void change_fbx_attribute_to_gl_attribute(GL_Meshes_Table * gl_mesh, ufbx_mesh * fbx_mesh, ufbx_skin_deformer * fbx_skin, GLuint fbx_triangle_num, GL_Skin_Attribute * skin_attribute, GL_Skin_Attribute  * mesh_skin_attribute);
void change_fbx_to_gl_node_style(GL_Node_Table * gl_node, ufbx_node * fbx_node);
void change_fbx_mat_to_gl_mesh_mat(SGG_Mat4 * dst_mat4, ufbx_matrix * src_mat);
void change_fbx_mesh_to_gl_mesh_style(GL_Meshes_Table * gl_mesh, ufbx_mesh * fbx_mesh);

static inline void count_fbx_mesh_parts(ufbx_mesh * fbx_mesh, GLuint * dst_triangle_num, GLuint * dst_parts_num){
    for(size_t mp_idx = 0; mp_idx < fbx_mesh->material_parts.count; mp_idx++){
        ufbx_mesh_part * fbx_parts = &fbx_mesh->material_parts.data[mp_idx];
        if(fbx_parts->num_triangles == 0)continue;
        *dst_triangle_num += fbx_parts->num_triangles;
        ++(*dst_parts_num);
    }
}
_EXTERN_C_END_
#endif