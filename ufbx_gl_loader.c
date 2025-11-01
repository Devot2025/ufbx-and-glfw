#include "ufbx_gl_loader.h"

ufbx_scene * loading_fbx_file(const char * file_path, float mesh_unit_meter){
    ufbx_load_opts load_opts = {0};
    ufbx_error load_err;
    load_opts.target_unit_meters = mesh_unit_meter;
    ufbx_scene * dst_scene = ufbx_load_file(file_path, &load_opts, &load_err);
    return dst_scene;
}

void change_fbx_mat_to_gl_mesh_mat(SGG_Mat4 * dst_mat4, ufbx_matrix * src_mat){
    dst_mat4->m[0] = src_mat->m00; dst_mat4->m[1] = src_mat->m10; dst_mat4->m[2] = src_mat->m20; dst_mat4->m[3] = 0;
    dst_mat4->m[4] = src_mat->m01; dst_mat4->m[5] = src_mat->m11; dst_mat4->m[6] = src_mat->m21; dst_mat4->m[7] = 0;
    dst_mat4->m[8] = src_mat->m02; dst_mat4->m[9] = src_mat->m12; dst_mat4->m[10] = src_mat->m22; dst_mat4->m[11] = 0;
    dst_mat4->m[12] = src_mat->m03; dst_mat4->m[13] = src_mat->m13; dst_mat4->m[14] = src_mat->m23; dst_mat4->m[15] = 1;
}
GL_Mesh_Data_Root * change_fbx_to_gl_mesh_style(const char * fbx_file_path, float mesh_unit_meter){
    ufbx_scene * tmp_scene = loading_fbx_file(fbx_file_path, mesh_unit_meter);
    if(!tmp_scene) return NULL;
    GL_Mesh_Data_Root * mesh_root = smart_calloc(GL_Mesh_Data_Root, 1);
    if(!mesh_root)return NULL;
    
    mesh_root->meshes_size = tmp_scene->meshes.count;
    mesh_root->nodes_size = tmp_scene->nodes.count;
    mesh_root->meshes_table = smart_calloc(GL_Meshes_Table, mesh_root->meshes_size);
    if(!mesh_root->meshes_table)return NULL;
    mesh_root->nodes_table = smart_calloc(GL_Node_Table, mesh_root->nodes_size);
    if(!mesh_root->nodes_table)return NULL;
    for(GLuint m_idx = 0; m_idx < mesh_root->meshes_size; ++m_idx){
        change_fbx_mesh_to_gl_mesh_style(&mesh_root->meshes_table[m_idx], tmp_scene->meshes.data[m_idx]);
    }

    for(GLuint n_idx = 0; n_idx < mesh_root->nodes_size; ++n_idx){
        change_fbx_to_gl_node_style(&mesh_root->nodes_table[n_idx], tmp_scene->nodes.data[n_idx]);
    }
    return mesh_root;
}
void change_fbx_to_gl_node_style(GL_Node_Table * gl_node, ufbx_node * fbx_node){
    gl_node->parent_id = fbx_node->parent ? fbx_node->parent->typed_id : -1;
    change_fbx_mat_to_gl_mesh_mat(&gl_node->node_to_parent_deform, &fbx_node->node_to_parent);
    change_fbx_mat_to_gl_mesh_mat(&gl_node->node_to_world_deform, &fbx_node->node_to_world);
}
void change_fbx_mesh_to_gl_mesh_style(GL_Meshes_Table * gl_mesh, ufbx_mesh * fbx_mesh){
    GLuint fbx_triangle_num = 0;
    GLuint fbx_parts_num = 0;
    count_fbx_mesh_parts(fbx_mesh, &fbx_triangle_num, &fbx_parts_num);
    GL_Skin_Attribute * skin_attribute = smart_calloc(GL_Skin_Attribute, fbx_triangle_num * DIMENSION_XYZ);
    GL_Skin_Attribute * mesh_skin_attribute = smart_calloc(GL_Skin_Attribute, fbx_mesh->num_vertices * DIMENSION_XYZ);
    ufbx_skin_deformer *skin_deformer = NULL;
    size_t bone_num = 0;
    if(fbx_mesh->skin_deformers.count > 0){
        assert(fbx_mesh->skin_deformers.count <= 1);
        skin_deformer = fbx_mesh->skin_deformers.data[0];

        for(size_t v_idx = 0; v_idx < fbx_mesh->num_vertices; ++v_idx){
            size_t weight_size = 0;
            float total_weight = 0.0f;
            float weights[4] = {0.0f};
            GLushort clusters[4] = {0};
            ufbx_skin_vertex mesh_skin_vertex = skin_deformer->vertices.data[v_idx];
            for(size_t w_idx = 0; w_idx < mesh_skin_vertex.num_weights; ++w_idx){
                if(weight_size >= 4){
                    break;
                }

                ufbx_skin_weight skin_weight = skin_deformer->weights.data[mesh_skin_vertex.weight_begin + w_idx];
                total_weight += (float)skin_weight.weight;
                clusters[weight_size] = (GLshort)skin_weight.cluster_index;
                weights[weight_size] = (float)skin_weight.weight;
                weight_size++;
            }
            if(total_weight > 0.0f){

                GL_Skin_Attribute * now_skin_attribute = &mesh_skin_attribute[v_idx];

                GLuint quantized_sum = 0;
                for(GLuint i = 0; i < 4; ++i){
                    GLubyte quantized_weight = (GLubyte)((float)weights[i] / total_weight * 255.0f);
                    quantized_sum += quantized_weight;

                    now_skin_attribute->bone_indices[i] = clusters[i];
                    now_skin_attribute->bone_weight[i] = quantized_weight;
                }

                now_skin_attribute->bone_weight[0] +=  255 - quantized_sum;
            }
        }
    }

    change_fbx_attribute_to_gl_attribute(gl_mesh, fbx_mesh, skin_deformer, fbx_triangle_num, skin_attribute, mesh_skin_attribute);

    free(mesh_skin_attribute);
    free(skin_attribute);
}

void change_fbx_attribute_to_gl_attribute(GL_Meshes_Table * gl_mesh, ufbx_mesh * fbx_mesh, ufbx_skin_deformer * fbx_skin, GLuint fbx_triangle_num, GL_Skin_Attribute * skin_attribute, GL_Skin_Attribute  * mesh_skin_attribute){
    float * mesh_vertex = smart_calloc(float, fbx_triangle_num * DIMENSION_XYZ * 8);
    if(!mesh_vertex)return;
    GLuint * mesh_indices = smart_calloc(GLuint, fbx_mesh->max_face_triangles * DIMENSION_XYZ);
    GLuint * mesh_triangle_indices = smart_calloc(GLuint, fbx_triangle_num * DIMENSION_XYZ);
    gl_mesh->mesh_parts = smart_calloc(GL_Meshes_Parts, fbx_mesh->material_parts.count);
    if(!gl_mesh->mesh_parts)return;
    gl_mesh->bone_maps = smart_calloc(GL_Meshes_Bone_Maps, fbx_mesh->material_parts.count);
    if(!gl_mesh->bone_maps)return;
    gl_mesh->parts_size = fbx_mesh->material_parts.count;
    for(size_t mp_idx = 0; mp_idx < fbx_mesh->material_parts.count; ++mp_idx){
        ufbx_mesh_part * mesh_part = &fbx_mesh->material_parts.data[mp_idx];
        if(mesh_part->num_triangles == 0)continue;
        GLuint atr_idx = 0;
        for(size_t f_idx = 0; f_idx < mesh_part->num_faces; ++f_idx){
            
            ufbx_face mesh_face = fbx_mesh->faces.data[mesh_part->face_indices.data[f_idx]];
            GLuint triangle_face_num = ufbx_triangulate_face(mesh_indices, fbx_mesh->max_face_triangles * DIMENSION_XYZ, fbx_mesh, mesh_face) * DIMENSION_XYZ;
            
            ufbx_vec2 deform_tri_uv = {0.0f};
            
            for(GLuint ta_idx = 0; ta_idx < triangle_face_num; ++ta_idx){

                GLuint tmp_indices_num = mesh_indices[ta_idx];
                ufbx_vec3 tri_vert = ufbx_get_vertex_vec3(&fbx_mesh->vertex_position, tmp_indices_num);
                ufbx_vec3 tri_norm = ufbx_get_vertex_vec3(&fbx_mesh->vertex_normal, tmp_indices_num);
                ufbx_vec2 tri_uv = fbx_mesh->vertex_uv.exists ? ufbx_get_vertex_vec2(&fbx_mesh->vertex_uv, tmp_indices_num) : deform_tri_uv;
                
                mesh_vertex[atr_idx * 8] = tri_vert.x;

                mesh_vertex[atr_idx * 8 + 1] = tri_vert.y;
                mesh_vertex[atr_idx * 8 + 2] = tri_vert.z;
                mesh_vertex[atr_idx * 8 + 3] = tri_norm.x;
                mesh_vertex[atr_idx * 8 + 4] = tri_norm.y;
                mesh_vertex[atr_idx * 8 + 5] = tri_norm.z;
                mesh_vertex[atr_idx * 8 + 6] = tri_uv.x;
                mesh_vertex[atr_idx * 8 + 7] = tri_uv.y;

                if(fbx_skin) skin_attribute[atr_idx] = mesh_skin_attribute[fbx_mesh->vertex_indices.data[tmp_indices_num]];

                atr_idx++;
                
            }   
        }

        ufbx_vertex_stream v_stream[2];
        
        size_t v_stream_num = 1;
        
        v_stream[0].data = mesh_vertex;
        v_stream[0].vertex_count = atr_idx;
        v_stream[0].vertex_size = sizeof(float) * 8;

        if(fbx_skin){ 

            const GLuint num_clusters = (GLuint)fbx_skin->clusters.count;

            bool *weight_have_map = smart_calloc(bool, num_clusters);
            if(!weight_have_map)continue;
            for (GLuint i = 0; i < atr_idx; ++i) {
                const GL_Skin_Attribute *sv = &skin_attribute[i];
                for (int j = 0; j < 4; ++j) {
                    if (!sv->bone_weight[j]) continue;
                    GLushort g = sv->bone_indices[j];                 
                    if (g < num_clusters) weight_have_map[g] = true;
                }
            }
            GLushort *g2l = smart_calloc(GLushort, num_clusters);
            if(!g2l){
                free(weight_have_map);
                continue;
            }
            /*init max uint16_t*/
            for (GLuint g = 0; g < num_clusters; ++g) g2l[g] = 0xFFFF;
            gl_mesh->bone_maps[mp_idx].bone_indices = smart_calloc(GLint ,num_clusters);
            gl_mesh->bone_maps[mp_idx].bone_mat4 = smart_calloc(SGG_Mat4 ,num_clusters);
            if(!gl_mesh->bone_maps[mp_idx].bone_indices || !gl_mesh->bone_maps[mp_idx].bone_mat4){
                free(weight_have_map);
                free(g2l);
                continue;
            }
            /*ps is fix map size and index*/
            GLshort ps = 0;
            /*marking is_valid weight cluster*/
            for (GLuint g = 0; g < num_clusters; ++g) {
                if (!weight_have_map[g]) continue;
                g2l[g] = ps;
                ufbx_skin_cluster *cl = fbx_skin->clusters.data[g];
                gl_mesh->bone_maps[mp_idx].bone_indices[ps] = (GLuint)cl->bone_node->typed_id;
                change_fbx_mat_to_gl_mesh_mat(&gl_mesh->bone_maps[mp_idx].bone_mat4[ps], &cl->geometry_to_bone);
                ++ps;
            }
            
            gl_mesh->bone_maps[mp_idx].bone_size = ps;
            GLint * tmp_map_indices = smart_realloc(GLint, gl_mesh->bone_maps[mp_idx].bone_indices, ps);
            if(tmp_map_indices) gl_mesh->bone_maps[mp_idx].bone_indices = tmp_map_indices;
            SGG_Mat4 * tmp_map_mat4 = smart_realloc(SGG_Mat4, gl_mesh->bone_maps[mp_idx].bone_mat4, ps);
            if(tmp_map_mat4) gl_mesh->bone_maps[mp_idx].bone_mat4 = tmp_map_mat4;

            for (GLuint i = 0; i < atr_idx; ++i) {
                GL_Skin_Attribute * sv = &skin_attribute[i];

                float    wf[4] = {0.0f};
                GLuint li[4] = {0};
                int      n = 0;
                float    sum = 0.0f;

                for (int j = 0; j < 4; ++j) {
                    if (!sv->bone_weight[j]) continue;
                    GLshort g = sv->bone_indices[j];                 
                    if (g >= num_clusters) continue;
                    GLshort l = g2l[g];                       
                    if (l == 0xFFFF) continue;                 
                    li[n] = l;
                    wf[n] = sv->bone_weight[j] / 255.0f;
                    sum  += wf[n];
                    ++n; 
                }
                if (sum > 0.0f) {
                    for (int t = 0; t < n; ++t) wf[t] /= sum;
                }
                GLuint qsum = 0;
                for (int t = 0; t < 4; ++t) {
                    GLubyte q = (t < n) ? (GLubyte)(wf[t] * 255.0f) : 0;
                    sv->bone_indices[t]  = (t < n) ? li[t] : 0;
                    sv->bone_weight[t] = q;
                    qsum += q;
                }
                if(n > 0)
                sv->bone_weight[0] += (GLubyte)(255 - (qsum & 0xFF));
            }

            free(weight_have_map);
            free(g2l);
            
                v_stream_num = 2;
                v_stream[1].data = skin_attribute;
                v_stream[1].vertex_count = atr_idx;
                v_stream[1].vertex_size = sizeof(GL_Skin_Attribute);
        }
        ufbx_error fbx_error;
        GL_Meshes_Parts * gl_mesh_parts = & gl_mesh->mesh_parts[mp_idx];

        size_t attr_num = ufbx_generate_indices(v_stream, v_stream_num, mesh_triangle_indices, atr_idx, NULL, &fbx_error);
        if(fbx_error.type != UFBX_ERROR_NONE){
            continue;
        }
        glGenVertexArrays(1, &gl_mesh_parts->vao);
        glBindVertexArray(gl_mesh_parts->vao);

        glGenBuffers(1, &gl_mesh_parts->vbo[0]);
        glBindBuffer(GL_ARRAY_BUFFER, gl_mesh_parts->vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, attr_num * sizeof(float) * 8, mesh_vertex, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(sizeof(float) * 6));

        if (fbx_skin && skin_attribute) {
            glGenBuffers(1, &gl_mesh_parts->vbo[1]);
            glBindBuffer(GL_ARRAY_BUFFER, gl_mesh_parts->vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, attr_num * sizeof(GL_Skin_Attribute), skin_attribute, GL_STATIC_DRAW);

            glEnableVertexAttribArray(4);
            glVertexAttribIPointer(
                4, 4, GL_UNSIGNED_SHORT,
                (GLsizei)sizeof(GL_Skin_Attribute),
                (const void*)(uintptr_t)offsetof(GL_Skin_Attribute, bone_indices)
            );

            glEnableVertexAttribArray(5);
            glVertexAttribPointer(
                5, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                (GLsizei)sizeof(GL_Skin_Attribute),
                (const void*)(uintptr_t)offsetof(GL_Skin_Attribute, bone_weight)
            );
        } else {
            gl_mesh_parts->vbo[1] = 0;
        }

        glGenBuffers(1, &gl_mesh_parts->vbo[2]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_mesh_parts->vbo[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, atr_idx * sizeof(GLuint), mesh_triangle_indices, GL_STATIC_DRAW);
        gl_mesh_parts->vtn = atr_idx;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
    }
    free(mesh_vertex);
    free(mesh_indices);
    free(mesh_triangle_indices);
}