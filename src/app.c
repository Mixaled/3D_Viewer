#include "app.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "s21_matrix/s21_matrix.h"
#include "util/prettify_c.h"
#include "util/cur_time.h"
#include "util/common_vecs.h"
#include "obj_parser/obj_parser.h"

#define SIDEBAR_WIDTH 300
#define SENSITIVITY 0.005
#define M_PI 3.14159265359
#define FOV 90.0
#define EDIT_FLAGS NK_EDIT_SIMPLE | NK_EDIT_SELECTABLE | NK_EDIT_CLIPBOARD

static Mesh create_tex_square_mesh();
static void app_load_model(App* this, const char* filename);

App* app_create(GLFWwindow* window) {
  debugln("Creating app...");
  App* result = (App*)malloc(sizeof(App));
  assert_alloc(result);

  str_t model_filename = str_literal("assets/teapot_normals.obj");
  AppSettings settings = app_settings_load("assets/settings.bin", &model_filename);

  (*result) = (App){
    .window = window,

    .resources = app_resources_create(),
    .input = app_input_create(),

    .model_filename = model_filename,
    .model_indices_count = 0,
    .model_vertices_count = 0,

    .settings = settings,
  };

  nk_textedit_init_default(&result->model_to_load);
  debugln("App created");

  app_load_model(result, model_filename.string);

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClearDepth(0.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  return result;
}

void app_free(App* app) {
  const char* model_name = app->resources.has_model ? app->model_filename.string : "";
  app_settings_save(&app->settings, model_name, "assets/settings.bin");

  app_resources_free(app->resources);
  app_input_free(app->input);
  nk_textedit_free(&app->model_to_load);
  str_free(app->model_filename);
  app_settings_free(app->settings);
  free(app);
}




AppSettings app_settings_create() {
  return (AppSettings) {
    .is_perspective = true,
    .projection_size = 50.0,
    .line_width = 1.0,


    .wireframe = false,
    .has_sky = true,
    .has_floor = true,
    .dotted_lines = false,
    .show_model = true,
    .background_color = {.r=0, .g=0, .b=0, .a=1},

    .object_pos = (Vec3){0,0,0},
    .object_rot = (Vec3){0,0,0},
    .object_scale = (Vec3){1,1,1},

    .velocity = 10.0,
    .camera_pos = {0, -2, 0},
    .camera_rot = {0, 0, 0.5},

    .vertices_draw_type = VERTICES_DRAW_NONE,
    .vertex_size = 1.0,
    .vertices_color = {.r = 1.0, .g = 0.0, .b = 0.0, .a = 1.0},

    .solid_color_model = false,
    .model_color = {.r = 1.0, .g = 1.0, .b = 0.0, .a = 1.0},
  };
}
AppResources app_resources_create() {
  return (AppResources) {
    //.model = ???
    .has_model = false,

    .tex_square = create_tex_square_mesh(),

    .shader = gl_program_from_2_paths("assets/shaders/common.vert", "assets/shaders/minimal.frag"),
    .shader_tex = gl_program_from_2_paths("assets/shaders/textured.vert", "assets/shaders/textured.frag"),
    .shader_points = gl_program_from_2_paths("assets/shaders/common.vert", "assets/shaders/points.frag"),

    .concrete = texture_load_repeat("assets/img/concrete.jpg"),
    .sky = skybox_create(),
  };
}
AppInput app_input_create() {
  return (AppInput) {
    .last_mouse_x = 0.0,
    .last_mouse_y = 0.0,
    .is_mouse_locked = false,
    .are_keys_pressed = vec_Bool_with_capacity(256),
  };
}

void app_settings_free(AppSettings settings) {
  unused(settings);
  // Currently, settings does not own any resources
}

void app_resources_free(AppResources resources) {
  if (resources.has_model)
    mesh_delete(resources.model);
  mesh_delete(resources.tex_square);

  gl_program_free(resources.shader);
  gl_program_free(resources.shader_tex);
  gl_program_free(resources.shader_points);

  texture_free(resources.concrete);
  skybox_free(resources.sky);
}

void app_input_free(AppInput input) {
  vec_Bool_free(input.are_keys_pressed);
}
















static GLint glLoc(GLuint program, const char* name) {
  return glGetUniformLocation(program, name);
}

static FloatArray16 get_view_persp_matrix(double fov_deg, double aspect_ratio, Vec3 camera_pos, Vec3 camera_rot);
static FloatArray16 get_view_proj_matrix(double size, double aspect_ratio, Vec3 camera_pos, Vec3 camera_rot);

static void draw_object_textured(const matrix_t* transform, Mesh mesh, Texture tex, const GlProgram* program);

static FloatArray16 app_calc_total_vp(const App* this, GLFWwindow* window);
static FloatArray16 app_calc_skybox_vp(const App* this, GLFWwindow* window);

static void app_draw_background(App* this, GLFWwindow* window);
static void app_draw_skybox(App* this, GLFWwindow* window);
static void app_draw_model(App* this, GLFWwindow* window, const FloatArray16* object);
static void app_draw_points(App* this, GLFWwindow* window, const FloatArray16* object);
static void app_draw_floor(App* this, GLFWwindow* window);
static void app_draw_ui(App* this, struct nk_context* ctx, GLFWwindow* window);

static matrix_t get_object_transform(const App* this);

void app_render(App* this, struct nk_context* ctx, GLFWwindow* window) {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  if (width is 0 or height is 0) return;

  matrix_t object_mat = get_object_transform(this);
  FloatArray16 object = s21_matrix_to_farray(&object_mat);
  s21_remove_matrix(&object_mat);


  app_draw_background(this, window);

  if (this->settings.has_sky)    app_draw_skybox(this, window);
  if (this->resources.has_model and this->settings.show_model) 
    app_draw_model(this, window, &object);
  if (this->settings.has_floor)  app_draw_floor(this, window);

  if (this->settings.vertices_draw_type is_not VERTICES_DRAW_NONE)
    app_draw_points(this, window, &object);

  app_draw_ui(this, ctx, window);
}





static double get_aspect_ratio(GLFWwindow* window) {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  return (double) width / (double) height;
}

static FloatArray16 app_calc_total_vp(const App* this, GLFWwindow* window) {
  double aspect_ratio = get_aspect_ratio(window);
  FloatArray16 total_mvp_arr;
  if (this->settings.is_perspective)
    total_mvp_arr  = get_view_persp_matrix(FOV, aspect_ratio, this->settings.camera_pos, this->settings.camera_rot);
  else
    total_mvp_arr = get_view_proj_matrix(this->settings.projection_size, aspect_ratio, this->settings.camera_pos, this->settings.camera_rot);
  
  return total_mvp_arr;
}

static FloatArray16 app_calc_skybox_vp(const App* this, GLFWwindow* window) {
  double aspect_ratio = get_aspect_ratio(window);

  FloatArray16 skybox_vp;
  if (this->settings.is_perspective)
    skybox_vp = get_view_persp_matrix(FOV, aspect_ratio, (Vec3){0,0,0}, this->settings.camera_rot);
  else
    skybox_vp = get_view_proj_matrix(0.25 - (0.25 / (1.0 + this->settings.projection_size)), aspect_ratio, (Vec3){0,0,0}, this->settings.camera_rot); 

  return skybox_vp;
}

static void app_draw_background(App* this, GLFWwindow* window) {
  unused(window);
  glClearColor(
    this->settings.background_color.r, 
    this->settings.background_color.g, 
    this->settings.background_color.b, 
    1.0
  );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void app_draw_skybox(App* this, GLFWwindow* window) {
  FloatArray16 skybox_vp = app_calc_skybox_vp(this, window);
  skybox_draw(&this->resources.sky, skybox_vp);
  glClear(GL_DEPTH_BUFFER_BIT);
}


static matrix_t get_object_transform(const App* this) {
  matrix_t obj_shift = s21_create_shift_matrix       (this->settings.object_pos.x, this->settings.object_pos.y, this->settings.object_pos.z);
  matrix_t obj_scale = s21_create_scale_matrix       (this->settings.object_scale.x, this->settings.object_scale.y, this->settings.object_scale.z);
  matrix_t obj_rotation = s21_create_rotations_camera(this->settings.object_rot.x, this->settings.object_rot.y, this->settings.object_rot.z);
  matrix_t object_temp1, object;

  assert_m(s21_mult_matrix(&obj_shift, &obj_rotation, &object_temp1) is OK); // shift(3) <- rotation(2)
  assert_m(s21_mult_matrix(&object_temp1, &obj_scale, &object) is OK); // (shift(3) <- rotation(2)) <- scale(1)
  s21_remove_matrix(&obj_shift);
  s21_remove_matrix(&obj_scale);
  s21_remove_matrix(&obj_rotation);
  s21_remove_matrix(&object_temp1);
  
  return object;
}

static void app_draw_model(App* this, GLFWwindow* window, const FloatArray16* object) {
  if (this->settings.wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(this->settings.line_width);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_GEQUAL);

  double aspect_ratio = get_aspect_ratio(window);
  GLuint prog = this->resources.shader.program;
  FloatArray16 total_mvp_arr = app_calc_total_vp(this, window);

  glUseProgram(prog);
  glUniform1f(glLoc(prog, "u_aspect_ratio"), (float) aspect_ratio);
  glUniform1i(glLoc(prog, "is_stripple"), this->settings.dotted_lines ? 1 : 0); // uniform from sleepy
  glUniform1i(glLoc(prog, "u_is_solid_color"), this->settings.solid_color_model ? 1 : 0); // uniform from sleepy
  glUniformMatrix4fv(glLoc(prog, "u_vp"), 1, GL_TRUE, total_mvp_arr.data);
  glUniformMatrix4fv(glLoc(prog, "u_object"), 1, GL_TRUE, object->data); 
  glUniform4f(
    glLoc(prog, "u_solid_color"), 
    this->settings.model_color.r,  
    this->settings.model_color.g,  
    this->settings.model_color.b,  
    this->settings.model_color.a  
  ); 
  
  mesh_bind(this->resources.model);
  mesh_draw(this->resources.model);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


static void app_draw_points(App* this, GLFWwindow* window, const FloatArray16* object) {
  double aspect_ratio = get_aspect_ratio(window);
  GLuint prog = this->resources.shader_points.program;
  FloatArray16 total_mvp_arr = app_calc_total_vp(this, window);

  glUseProgram(prog);
  glUniform1f(glLoc(prog, "u_aspect_ratio"), (float) aspect_ratio);
  glUniform1i(glLoc(prog, "u_is_circle"), this->settings.vertices_draw_type is VERTICES_DRAW_CIRCLE ? 1 : 0); // uniform from sleepy
  glUniformMatrix4fv(glLoc(prog, "u_vp"), 1, GL_TRUE, total_mvp_arr.data);
  glUniform4f(
    glLoc(prog, "u_points_color"), 
    this->settings.vertices_color.r,  
    this->settings.vertices_color.g,  
    this->settings.vertices_color.b,  
    this->settings.vertices_color.a  
  );
  glUniformMatrix4fv(glLoc(prog, "u_object"), 1, GL_TRUE, object->data);
  
  mesh_bind(this->resources.model);

  glPointSize(this->settings.vertex_size);
  mesh_draw_points(this->resources.model);
}



static void app_draw_floor(App* this, GLFWwindow* window) {
  FloatArray16 total_mvp_arr = app_calc_total_vp(this, window);

  glUseProgram(this->resources.shader_tex.program);
  glUniformMatrix4fv(glLoc(this->resources.shader_tex.program, "u_vp"), 1, GL_TRUE, total_mvp_arr.data);
  matrix_t floor = s21_create_scaleshift_matrix(1000.0, 1000.0, 1.0, 0.0, 0.0, -0.6);

  glUniform2f(glLoc(this->resources.shader_tex.program, "u_texture_scale"), 1000.0, 1000.0);
  draw_object_textured(&floor, this->resources.tex_square, this->resources.concrete, &this->resources.shader_tex);
  s21_remove_matrix(&floor);
}

#define RGB_PICKER(ctx, label, short_name, ptr) \
 {\
    nk_label(ctx, (label), NK_TEXT_ALIGN_LEFT);\
    nk_layout_row_dynamic(ctx, 100, 1);\
    (*(ptr)) = nk_color_picker(ctx, (*(ptr)), NK_RGB);\
    nk_layout_row_dynamic(ctx, 30, 1);\
    nk_property_float(ctx, short_name " - R", 0.0, &((ptr)->r), 1.0, 0.0, 0.01);\
    nk_property_float(ctx, short_name " - G", 0.0, &((ptr)->g), 1.0, 0.0, 0.01);\
    nk_property_float(ctx, short_name " - B", 0.0, &((ptr)->b), 1.0, 0.0, 0.01);\
 }

static void app_draw_ui(App* this, struct nk_context* ctx, GLFWwindow* window) {
  int height;
  glfwGetWindowSize(window, null, &height);

  if (nk_begin(ctx, "Main window", nk_rect(0, 0, SIDEBAR_WIDTH, height), 0)) {
    // Sidebar
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "3D Viewer", NK_TEXT_ALIGN_LEFT);
    if(nk_button_label(ctx, "Reset settings")) {
      app_settings_free(this->settings);
      this->settings = app_settings_create();
    }
    
    nk_spacer(ctx);
    nk_label(ctx, "Load model", NK_TEXT_ALIGN_CENTERED);
    nk_label(ctx, "Filename:", NK_TEXT_ALIGN_LEFT);
    nk_edit_buffer(ctx, EDIT_FLAGS, &this->model_to_load, nk_filter_default);
    nk_label(ctx, this->model_filename.string, NK_TEXT_ALIGN_LEFT);
    

    if (nk_button_label(ctx, "Load")) {
      StrSlice filename_slice = {
        .start = nk_str_get_const(&this->model_to_load.string), 
        .length = nk_str_len(&this->model_to_load.string),
      };
      str_t filename = str_slice_to_owned(filename_slice);
      app_load_model(this, filename.string);
      str_free(filename);
    }

    str_t model_info = str_owned("Vertices: %d | Indices: %d", this->model_vertices_count, this->model_indices_count);
    nk_label(ctx, model_info.string, NK_TEXT_ALIGN_LEFT);
    str_free(model_info);

    nk_spacer(ctx);
    nk_label(ctx, "View", NK_TEXT_ALIGN_CENTERED);
    
    nk_spacer(ctx);
    nk_label(ctx, "View - Environment", NK_TEXT_ALIGN_LEFT);
    nk_checkbox_label(ctx, "Floor", &this->settings.has_floor);
    nk_checkbox_label(ctx, "Sky", &this->settings.has_sky);
    nk_checkbox_label(ctx, "Show model", &this->settings.show_model);

    RGB_PICKER(ctx, "Background color", "Background", &this->settings.background_color);

    
    nk_spacer(ctx);
    nk_label(ctx, "View - Model", NK_TEXT_ALIGN_LEFT);
    nk_checkbox_label(ctx, "Wireframe", &this->settings.wireframe);
    nk_checkbox_label(ctx, "Dotted Lines", &this->settings.dotted_lines);

    nk_checkbox_label(ctx, "Solid model color", &this->settings.solid_color_model);
    RGB_PICKER(ctx, "Model color", "Model", &this->settings.model_color);

    nk_label(ctx, "Vertex type", NK_TEXT_ALIGN_LEFT);
    nk_layout_row_dynamic(ctx, 30, 2);
    
    #define VERTEX_PICKER(name, type) \
      if (nk_check_label(ctx, (name), this->settings.vertices_draw_type is (type))) \
        this->settings.vertices_draw_type = (type);    
  
    VERTEX_PICKER("(V) None", VERTICES_DRAW_NONE)
    VERTEX_PICKER("(V) Circle", VERTICES_DRAW_CIRCLE)
    VERTEX_PICKER("(V) Square", VERTICES_DRAW_SQUARE)

    nk_layout_row_dynamic(ctx, 30, 1);
    RGB_PICKER(ctx, "Vertex color", "Vertices", &this->settings.vertices_color);
    nk_property_double(ctx, "Vertex size", 1.0, &this->settings.vertex_size, 10.0, 0.0, 0.1);

    nk_spacer(ctx);
    nk_label(ctx, "Object", NK_TEXT_ALIGN_CENTERED);

    nk_label(ctx, "Position", NK_TEXT_ALIGN_LEFT);
    nk_property_double(ctx, "OX", -DBL_MAX, &this->settings.object_pos.x, DBL_MAX, 0.01, 0.01);
    nk_property_double(ctx, "OY", -DBL_MAX, &this->settings.object_pos.y, DBL_MAX, 0.01, 0.01);
    nk_property_double(ctx, "OZ", -DBL_MAX, &this->settings.object_pos.z, DBL_MAX, 0.01, 0.01);

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Rotation", NK_TEXT_ALIGN_LEFT);
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_property_double(ctx, "OYZ", -M_PI, &this->settings.object_rot.x, M_PI, 0.01, 0.01);
    nk_property_double(ctx, "OXZ", -M_PI, &this->settings.object_rot.y, M_PI, 0.01, 0.01);
    nk_property_double(ctx, "OXY", -M_PI, &this->settings.object_rot.z, M_PI, 0.01, 0.01);

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Scale", NK_TEXT_ALIGN_LEFT);
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_property_double(ctx, "OSX", -DBL_MAX, &this->settings.object_scale.x, DBL_MAX, 0.01, 0.01);
    nk_property_double(ctx, "OSY", -DBL_MAX, &this->settings.object_scale.y, DBL_MAX, 0.01, 0.01);
    nk_property_double(ctx, "OSZ", -DBL_MAX, &this->settings.object_scale.z, DBL_MAX, 0.01, 0.01);



    nk_layout_row_dynamic(ctx, 30, 1);
    nk_spacer(ctx);
    nk_label(ctx, "Camera", NK_TEXT_ALIGN_CENTERED);
    nk_checkbox_label(ctx, "Perspective", &this->settings.is_perspective);
    nk_property_double(ctx, "Projection size", 0.0, &this->settings.projection_size, 1000.0, 0, 0.01);

    nk_property_double(ctx, "Pos X", -DBL_MAX, &this->settings.camera_pos.x, DBL_MAX, 0.01, 0.01);
    nk_property_double(ctx, "Pos Y", -DBL_MAX, &this->settings.camera_pos.y, DBL_MAX, 0.01, 0.01);
    nk_property_double(ctx, "Pos Z", -DBL_MAX, &this->settings.camera_pos.z, DBL_MAX, 0.01, 0.01);
    nk_property_double(ctx, "Velocity", 0.0, &this->settings.velocity, DBL_MAX, 0.01, 0.01);
    
    nk_property_double(ctx, "Rot YZ", -M_PI, &this->settings.camera_rot.x, M_PI, 0.01, 0.01);
    nk_property_double(ctx, "Rot XZ", -M_PI, &this->settings.camera_rot.y, M_PI, 0.01, 0.01);
    nk_property_double(ctx, "Rot XY", -M_PI, &this->settings.camera_rot.z, M_PI, 0.01, 0.01);
  }

  nk_end(ctx);
}



#define SETTINGS_BUF_SIZE 10240


/* 
Settings file is a blob of following format: 
  value of sizeof(AppSettings)    - (size_t)
  entire AppSettings struct       - (AppSettings)
  length of model filepath        - (size_t)
  model filepath                  - (array of chars)
*/
AppSettings app_settings_load(const char* filepath, str_t* out_model_name) {
  FILE* file = fopen(filepath, "rb");
  AppSettings result;

  if (file) {
    size_t struct_size = 42;
    assert_m(fread(&struct_size, sizeof(size_t), 1, file));

    if (struct_size != sizeof(AppSettings)) {
      debugln("Found settings file, but it contains incompatible AppSettings structure");
      result = app_settings_create();
    } else {
      debugln("Loading settings from file...");
      assert_m(fread(&result, sizeof(AppSettings), 1, file));

      size_t string_length = 42;
      assert_m(fread(&string_length, sizeof(size_t), 1, file));
      
      assert_m(string_length <= SETTINGS_BUF_SIZE);
      char buffer[SETTINGS_BUF_SIZE];
      assert_m(fread(buffer, string_length, 1, file));
      debugln("Filepath length: %d", (int)string_length);

      str_t new_model_name = str_slice_to_owned((StrSlice) {
        .length = string_length,
        .start = buffer,
      });
      debugln("New model name %d: %s", (int)string_length, new_model_name.string);
      str_free(*out_model_name);
      (*out_model_name) = new_model_name;
    }

    fclose(file);
  } else {
    debugln("Cannot open settings file");
    result = app_settings_create();
  }

  return result;
}
void app_settings_save(const AppSettings* settings, const char* model_name, const char* filepath) {
  FILE* file = fopen(filepath, "wb+");
  assert_m(file);

  size_t struct_size = sizeof(AppSettings);
  fwrite(&struct_size, sizeof(size_t), 1, file);
  fwrite(settings, sizeof(AppSettings), 1, file);

  size_t model_name_len = strlen(model_name);
  fwrite(&model_name_len, sizeof(size_t), 1, file);
  fwrite(model_name, model_name_len, 1, file);

  fclose(file);
}











static void draw_object_textured(const matrix_t* transform, Mesh mesh, Texture tex, const GlProgram* program) {
  // We need to transform matrix into 1D row-major array of floats to pass it into opengl
  FloatArray16 array = s21_matrix_to_farray(transform);

  // GL_TRUE means row-major. GL_FALSE would mean column-major
  glUniformMatrix4fv(glLoc(program->program, "u_object"), 1, GL_TRUE, array.data);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex.texture_id);
  glUniform1i(glLoc(program->program, "u_texture"), 0);
  
  mesh_bind(mesh);
  mesh_draw(mesh);
}

static matrix_t get_camera_view_matrix(Vec3 camera_pos, Vec3 camera_rot) {
  matrix_t camera_shift_mat = s21_create_shift_matrix(-camera_pos.x, -camera_pos.y, -camera_pos.z);
  matrix_t camera_rot_mat = s21_create_rotations_camera(-camera_rot.y, -camera_rot.x, -camera_rot.z);

  matrix_t camera;
  // camera = camera_rot_mat * camera_shift_mat;
  assert_m(s21_mult_matrix(&camera_rot_mat, &camera_shift_mat, &camera) is OK);
  s21_remove_matrix(&camera_shift_mat);
  s21_remove_matrix(&camera_rot_mat);

  return camera;
}

static matrix_t mul_proj_by_view(matrix_t proj, matrix_t view) {
  matrix_t view_to_camera = s21_create_view_to_camera();

  matrix_t temp_a;
  // projection * view_to_camera * camera^-1
  assert_m(s21_mult_matrix(&proj, &view_to_camera, &temp_a) is OK);
  s21_remove_matrix(&proj);
  s21_remove_matrix(&view_to_camera);

  matrix_t total_mvp;
  assert_m(s21_mult_matrix(&temp_a, &view, &total_mvp) is OK);
  s21_remove_matrix(&temp_a);
  s21_remove_matrix(&view);

  return total_mvp;
}

static FloatArray16 get_view_persp_matrix(double fov_deg, double aspect_ratio, Vec3 camera_pos, Vec3 camera_rot) {
  matrix_t camera = get_camera_view_matrix(camera_pos, camera_rot);
  matrix_t projection = s21_create_perspective_matrix(fov_deg * M_PI / 180.0, 0.1, 2000.0, aspect_ratio);
  matrix_t total = mul_proj_by_view(projection, camera);

  FloatArray16 total_mvp_arr = s21_matrix_to_farray(&total);
  s21_remove_matrix(&total);

  return total_mvp_arr;
}


static FloatArray16 get_view_proj_matrix(double size, double aspect_ratio, Vec3 camera_pos, Vec3 camera_rot) {
  matrix_t camera = get_camera_view_matrix(camera_pos, camera_rot);

  matrix_t projection = s21_create_projection_matrix(size, 0.1, 2000.0, aspect_ratio);
  matrix_t total = mul_proj_by_view(projection, camera);

  FloatArray16 total_mvp_arr = s21_matrix_to_farray(&total);
  s21_remove_matrix(&total);

  return total_mvp_arr;
}

void app_on_scroll(App* this, double x, double y) {
  unused(this);
  unused(x);
  unused(y);
}


void app_on_mouse_move(App* this, double pos_x, double pos_y) {
  double dx = pos_x - this->input.last_mouse_x; 
  double dy = pos_y - this->input.last_mouse_y; 

  this->input.last_mouse_x = pos_x;
  this->input.last_mouse_y = pos_y;


  if (isnan(dx) or isinf(dx) or isnan(dy) or isinf(dy))
    return;

  if (this->input.is_mouse_locked) {
    this->settings.camera_rot.z -= dx * SENSITIVITY;
    this->settings.camera_rot.x -= dy * SENSITIVITY;

    if (this->settings.camera_rot.x > (M_PI / 2.0))
      this->settings.camera_rot.x = M_PI / 2.0;
      
    if (this->settings.camera_rot.x < (-M_PI / 2.0))
      this->settings.camera_rot.x = -M_PI / 2.0;

    this->settings.camera_rot.z = fmod(this->settings.camera_rot.z + M_PI, 2.0 * M_PI) - M_PI;
  }

}

void app_on_mouse_click(App* this, int button, int action, int mods) {
  unused(button);
  unused(mods);
  if (
    not this->input.is_mouse_locked and 
    this->input.last_mouse_x > SIDEBAR_WIDTH and 
    action is GLFW_PRESS
  ) {
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    this->input.is_mouse_locked = true;
  }
}

void app_update(App* this, double dt) {
  if (this->input.is_mouse_locked) {
    double velocity = this->settings.velocity;
    double movement[2] = {0, 0};

    if (app_input_is_pressed(&this->input, GLFW_KEY_W)) movement[1] += velocity * dt;
    if (app_input_is_pressed(&this->input, GLFW_KEY_A)) movement[0] += velocity * dt;
    if (app_input_is_pressed(&this->input, GLFW_KEY_S)) movement[1] -= velocity * dt;
    if (app_input_is_pressed(&this->input, GLFW_KEY_D)) movement[0] -= velocity * dt;
      
    if (app_input_is_pressed(&this->input, GLFW_KEY_SPACE))
      this->settings.camera_pos.z += velocity * dt;
    if (app_input_is_pressed(&this->input, GLFW_KEY_LEFT_SHIFT))
      this->settings.camera_pos.z -= velocity * dt;

    this->settings.camera_pos.x += -movement[0] * cos(this->settings.camera_rot.z) - movement[1] * sin(this->settings.camera_rot.z);
    this->settings.camera_pos.y += -movement[0] * sin(this->settings.camera_rot.z) + movement[1] * cos(this->settings.camera_rot.z);

  }
  
  #define FLOOR -0.6
  #define MIN_Z (FLOOR + sqrt(2) * 0.1)

  if (this->settings.has_floor and this->settings.camera_pos.z < MIN_Z)
    this->settings.camera_pos.z = MIN_Z;
  
  #undef FLOOR
  #undef MIN_Z
}

void app_on_key_press(App* this, int key, int scancode, int action, int mods) {
  unused(scancode);
  unused(mods);
  app_input_on_key(&this->input, key, action);

  if(key is GLFW_KEY_ESCAPE and action is GLFW_PRESS) {
    this->input.is_mouse_locked = false;
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}


bool app_input_is_pressed(const AppInput* input, int keycode) {
  if (input->are_keys_pressed.length <= keycode)
    return false;
  else
    return input->are_keys_pressed.data[keycode];
}
void app_input_on_key(AppInput* input, int keycode, int action) {
  if (not (action is GLFW_PRESS or action is GLFW_RELEASE))
    return;

  while (input->are_keys_pressed.length < (keycode + 1))
    vec_Bool_push(&input->are_keys_pressed, false);

  input->are_keys_pressed.data[keycode] = action is GLFW_PRESS ? true : false;
}



static Mesh create_tex_square_mesh() {
  Mesh mesh = mesh_create();

  MeshAttrib attribs[] = {
      {3, sizeof(float), GL_FLOAT},
      {2, sizeof(float), GL_FLOAT},
  };
  mesh_bind_consecutive_attribs(mesh, 0, attribs,
                                sizeof(attribs) / sizeof(attribs[0]));

  float vertices[] = {
      -1.0, -1.0, 0.0,  /*-*/ 0.0, 0.0, /*-*/
       1.0, -1.0, 0.0,  /*-*/ 1.0, 0.0, /*-*/
       1.0,  1.0, 0.0,  /*-*/ 1.0, 1.0, /*-*/
      -1.0,  1.0, 0.0,  /*-*/ 0.0, 1.0, /*-*/
  };
  mesh_set_vertex_data(&mesh, vertices, sizeof(vertices), GL_STATIC_DRAW);

  int indices[] = {
      0, 1, 2, 0, 3, 2,
  };
  mesh_set_indices_int_tuples(
      &mesh, indices, sizeof(indices) / sizeof(indices[0]), GL_STATIC_DRAW);

  return mesh;
}

#include "obj_parser/obj_mdl_to_mesh.h"


static void app_load_model(App* this, const char* filename) {
  FILE* file = fopen(filename, "r");

  if (file) {
    fclose(file);
    ObjModel mdl = obj_parse_model(filename);

    debugln("Parsed model, gonna convert to mesh! It has %d vertices and %d faces", mdl.vertices.length, mdl.faces.length);
    Mesh mesh = obj_model_to_mesh(mdl);

    if (this->resources.has_model)
      mesh_delete(this->resources.model);
    this->resources.model = mesh;
    this->model_vertices_count = mdl.vertices.length;
    this->model_indices_count = mesh.indices_count;

    str_t new_model_filename = str_owned("%s", filename);
    str_free(this->model_filename);
    this->model_filename = new_model_filename;

    this->resources.has_model = true;
  } else {
    str_free(this->model_filename);
    this->model_filename = str_owned("Cannot open file '%s'", filename);
  }
}