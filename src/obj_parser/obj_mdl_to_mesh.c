#include "obj_mdl_to_mesh.h"
#include "../util/common_vecs.h"
#include <float.h>

static void push_vertex_normal(vec_float* dest, Vertex vertex, Normal normal);
static int index_to_id(FaceIndex index, vec_float* vertices, const vec_Normal* norm_src);

static float find_lowest_y(const vec_Vertex* vertices) {
  float lowest_y = FLT_MAX;
  
  for (int i = 0; i < vertices->length; i++)
    if (vertices->data[i].y < lowest_y)
      lowest_y = vertices->data[i].y;

  return lowest_y;
}

Mesh obj_model_to_mesh(ObjModel model) {
  float lowest_y = find_lowest_y(&model.vertices);

  vec_float vertices = vec_float_create();

  for (int i = 0; i < model.vertices.length; i++)
    push_vertex_normal(&vertices, model.vertices.data[i], (Normal){0,1,0});

  vec_int indices = vec_int_create();

  for (int f = 0; f < model.faces.length; f++) {
    const Face* face = &model.faces.data[f];
    assert_m(face->indices.length >= 3);

    #define INDEX_TO_ID(i) index_to_id(face->indices.data[i], &vertices, &model.normals) 
    int start_id = INDEX_TO_ID(0);
    int mid_id = INDEX_TO_ID(1);
    for (int i = 2; i < face->indices.length; i++) {
      int cur_id = INDEX_TO_ID(i);
      vec_int_push(&indices, start_id);
      vec_int_push(&indices, mid_id);
      vec_int_push(&indices, cur_id);
      mid_id = cur_id;
    }
    #undef INDEX_TO_ID
  }

  // Step 2. Place model bottom at z = 0
  for (int i = 2; i < vertices.length; i+=6)
    vertices.data[i] -= lowest_y;

  // Step 3. Create and configure mesh
  Mesh mesh = mesh_create();

  MeshAttrib attribs[] = {
      {3, sizeof(float), GL_FLOAT}, // Pos (3 floats)
      {3, sizeof(float), GL_FLOAT}, // Color/normal (3 floats)
  };
  mesh_bind_consecutive_attribs(mesh, 0, attribs, sizeof(attribs) / sizeof(attribs[0]));

  // Step 4. Send data to GPU.
  mesh_set_vertex_data(&mesh, vertices.data, vertices.length * sizeof(float), GL_STATIC_DRAW);
  mesh_set_indices_int_tuples(&mesh, indices.data, indices.length, GL_STATIC_DRAW);

  vec_float_free(vertices);
  vec_int_free(indices);
  obj_model_free(model);
  return mesh;
}
static int index_to_id(FaceIndex index, vec_float* vertices, const vec_Normal* norm_src) {
  int id = index.point - 1;
  
  if (index.normal >= 1) {
    assert_m(vertices->length >= id * 6 + 6);
    Normal n = norm_src->data[index.normal - 1];
    vertices->data[id * 6 + 3] = n.z;
    vertices->data[id * 6 + 4] = n.x;
    vertices->data[id * 6 + 5] = n.y;
  }

  return id;
}

static void push_vertex_normal(vec_float* dest, Vertex vertex, Normal normal) {
  // We swap cuz we have different axes positions
  vec_float_push(dest, vertex.z);
  vec_float_push(dest, vertex.x);
  vec_float_push(dest, vertex.y);

  vec_float_push(dest, normal.z);
  vec_float_push(dest, normal.x);
  vec_float_push(dest, normal.y);
}