#include "skybox.h"
#include "../util/prettify_c.h"

static Mesh create_skybox_mesh();

Skybox skybox_create() {
    matrix_t unit = s21_create_unit_matrix();
    FloatArray16 arr = s21_matrix_to_farray(&unit);
    s21_remove_matrix(&unit);

    const char* paths[] = {
      "assets/img/sky/up.jpg",
      "assets/img/sky/dn.jpg",
      "assets/img/sky/rt.jpg",
      "assets/img/sky/lf.jpg",
      "assets/img/sky/ft.jpg",
      "assets/img/sky/bk.jpg",
    };  

    return (Skybox) {
        .skybox_mesh = create_skybox_mesh(),
        .shader = gl_program_from_2_paths("assets/shaders/skybox.vert", "assets/shaders/skybox.frag"),
        .textures = texture_array_load_clamp(paths, LEN(paths)),
        .object_mat = arr,
    };
}

void skybox_draw(const Skybox* sky, FloatArray16 skybox_viewproj) {
    glUseProgram(sky->shader.program);
    glUniformMatrix4fv(glGetUniformLocation(sky->shader.program, "u_vp"), 1, GL_TRUE, skybox_viewproj.data);
    glUniformMatrix4fv(glGetUniformLocation(sky->shader.program, "u_object"), 1, GL_TRUE, sky->object_mat.data);
    glUniform2f(glGetUniformLocation(sky->shader.program, "u_texture_scale"), 1.0, 1.0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, sky->textures.texture_id);
    glUniform1i(glGetUniformLocation(sky->shader.program, "u_textures"), 0);

    mesh_bind(sky->skybox_mesh);
    mesh_draw(sky->skybox_mesh);
    
    glClear(GL_DEPTH_BUFFER_BIT);
}

void skybox_free(Skybox sky) {
    mesh_delete(sky.skybox_mesh);
    gl_program_free(sky.shader);
    texture_array_free(sky.textures);
}


static Mesh create_skybox_mesh() {
Mesh mesh = mesh_create();

  MeshAttrib attribs[] = {
      {3, sizeof(float), GL_FLOAT},
      {2, sizeof(float), GL_FLOAT},
      {1, sizeof(float), GL_FLOAT},
  };
  mesh_bind_consecutive_attribs(mesh, 0, attribs,
                                sizeof(attribs) / sizeof(attribs[0]));

  //Pos: X,    Y,    Z,  Tex pos: X Y,       Tex id
  float vertices[] = {
    // UP
      -0.5, -0.5,  0.5, /*-*/ 0.0, 0.0, /*-*/ 0, /*-*/
      -0.5,  0.5,  0.5, /*-*/ 0.0, 1.0, /*-*/ 0, /*-*/
       0.5, -0.5,  0.5, /*-*/ 1.0, 0.0, /*-*/ 0, /*-*/
       0.5,  0.5,  0.5, /*-*/ 1.0, 1.0, /*-*/ 0, /*-*/

    // DOWN
      -0.5, -0.5, -0.5, /*-*/ 0.0, 1.0, /*-*/ 1, /*-*/
      -0.5,  0.5, -0.5, /*-*/ 0.0, 0.0, /*-*/ 1, /*-*/
       0.5, -0.5, -0.5, /*-*/ 1.0, 1.0, /*-*/ 1, /*-*/
       0.5,  0.5, -0.5, /*-*/ 1.0, 0.0, /*-*/ 1, /*-*/

    // LEFT
      -0.5, -0.5, -0.5, /*-*/ 0.0, 0.0, /*-*/ 2, /*-*/
      -0.5, -0.5,  0.5, /*-*/ 0.0, 1.0, /*-*/ 2, /*-*/
      -0.5,  0.5, -0.5, /*-*/ 1.0, 0.0, /*-*/ 2, /*-*/
      -0.5,  0.5,  0.5, /*-*/ 1.0, 1.0, /*-*/ 2, /*-*/

    // RIGHT
       0.5, -0.5, -0.5, /*-*/ 1.0, 0.0, /*-*/ 3, /*-*/
       0.5, -0.5,  0.5, /*-*/ 1.0, 1.0, /*-*/ 3, /*-*/
       0.5,  0.5, -0.5, /*-*/ 0.0, 0.0, /*-*/ 3, /*-*/
       0.5,  0.5,  0.5, /*-*/ 0.0, 1.0, /*-*/ 3, /*-*/

    // FORWARD
      -0.5,  0.5, -0.5, /*-*/ 0.0, 0.0, /*-*/ 4, /*-*/
      -0.5,  0.5,  0.5, /*-*/ 0.0, 1.0, /*-*/ 4, /*-*/
       0.5,  0.5, -0.5, /*-*/ 1.0, 0.0, /*-*/ 4, /*-*/
       0.5,  0.5,  0.5, /*-*/ 1.0, 1.0, /*-*/ 4, /*-*/
       
    // BACKWARD
      -0.5, -0.5, -0.5, /*-*/ 1.0, 0.0, /*-*/ 5, /*-*/
      -0.5, -0.5,  0.5, /*-*/ 1.0, 1.0, /*-*/ 5, /*-*/
       0.5, -0.5, -0.5, /*-*/ 0.0, 0.0, /*-*/ 5, /*-*/
       0.5, -0.5,  0.5, /*-*/ 0.0, 1.0, /*-*/ 5, /*-*/
  };
  mesh_set_vertex_data(&mesh, vertices, sizeof(vertices), GL_STATIC_DRAW);

  int indices[] = {
      0, 1, 2, /*-*/ 2, 1, 3, /*UP*/
      4, 5, 6, /*-*/ 6, 5, 7, /*DOWN*/
      8, 9, 10, /*-*/ 10, 9, 11, /*LEFT*/
      12, 13, 14, /*-*/ 14, 13, 15, /*RIGHT*/
      16, 17, 18, /*-*/ 18, 17, 19, /*FORWARD*/
      20, 21, 22, /*-*/ 22, 21, 23, /*BACKWARD*/
  };
  mesh_set_indices_int_tuples(
      &mesh, indices, sizeof(indices) / sizeof(indices[0]), GL_STATIC_DRAW);

  return mesh;
}