#ifndef UI_SKYBOX_
#define UI_SKYBOX_

#include "texture.h"
#include "shader_loader.h"
#include "mesh.h"
#include "../s21_matrix/s21_matrix.h"

typedef struct Skybox {
    Mesh skybox_mesh;
    GlProgram shader;
    TextureArray textures;

    FloatArray16 object_mat;
} Skybox;

Skybox skybox_create();
void skybox_draw(const Skybox* sky, FloatArray16 skybox_viewproj);
void skybox_free(Skybox sky);

#endif //UI_SKYBOX_
