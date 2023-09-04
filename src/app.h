#ifndef SRC_APP_H_
#define SRC_APP_H_

#include <stdbool.h>
#include "util/better_string.h"
#include "util/common_vecs.h"

#include "full_nuklear.h"
#include "ui/mesh.h"
#include "ui/shader_loader.h"
#include "ui/texture.h"
#include "ui/skybox.h"

typedef struct Vec3 {
  double x, y, z;
} Vec3;

#define VERTICES_DRAW_NONE 1
#define VERTICES_DRAW_CIRCLE 2
#define VERTICES_DRAW_SQUARE 3

typedef struct AppSettings {
  bool is_perspective;
  double projection_size;
  double line_width;

  bool wireframe;
  bool has_sky;
  bool has_floor;
  bool dotted_lines;
  bool show_model;
  struct nk_colorf background_color;

  Vec3 object_pos, object_rot, object_scale;
  double velocity;
  Vec3 camera_pos, camera_rot;


  int vertices_draw_type;
  double vertex_size;
  struct nk_colorf vertices_color;

  bool solid_color_model;
  struct nk_colorf model_color;
} AppSettings;

typedef struct AppResources {
  Mesh model;
  bool has_model;

  Mesh tex_square;
  GlProgram shader, shader_tex, shader_points;
  Texture concrete;
  Skybox sky;
} AppResources;

typedef struct AppInput {
  double last_mouse_x, last_mouse_y;
  bool is_mouse_locked;
  vec_Bool are_keys_pressed;
} AppInput;

typedef struct App {
  GLFWwindow* window;
  AppResources resources;
  AppInput input;

  // Ui stuff
  struct nk_text_edit model_to_load;

  // Model stuff
  str_t model_filename;
  int model_vertices_count, model_indices_count;

  AppSettings settings;
} App;

AppSettings app_settings_create();
AppResources app_resources_create();
AppInput app_input_create();

AppSettings app_settings_load(const char* filepath, str_t* out_model_name);
void app_settings_save(const AppSettings* settings, const char* model_name, const char* filepath);
bool app_input_is_pressed(const AppInput* input, int keycode);
void app_input_on_key(AppInput* input, int keycode, int action);

void app_settings_free(AppSettings settings);
void app_resources_free(AppResources resources);
void app_input_free(AppInput input);


App* app_create(GLFWwindow* window);
void app_free(App*);

void app_render(App*, struct nk_context* ctx, GLFWwindow* window);
void app_update(App*, double dt);

void app_on_scroll(App* this, double x, double y);
void app_on_mouse_move(App* this, double pos_x, double pos_y);
void app_on_mouse_click(App* this, int button, int action, int mods);
void app_on_key_press(App* this, int key, int scancode, int action, int mods);

#endif  // SRC_APP_H_