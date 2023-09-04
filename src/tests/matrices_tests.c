#include <check.h>
#include <float.h>

#include "../s21_matrix/s21_matrix.h"
#include "../util/prettify_c.h"

#define EPSILON 0.00001
#undef M_PI
#define M_PI 3.14159265358979323846264338327950288

#define Rad(deg) (deg) * M_PI / 180.0

#define SIMPLE_TEST(sx, sy, sz, ex, ey, ez)                    \
  matrix_t vector;                                             \
  s21_create_matrix(4, 1, &vector);                            \
  vector.matrix[0][0] = (sx);                                  \
  vector.matrix[1][0] = (sy);                                  \
  vector.matrix[2][0] = (sz);                                  \
  vector.matrix[3][0] = (1.0);                                 \
                                                               \
  matrix_t result;                                             \
  ck_assert(s21_mult_matrix(&matrix, &vector, &result) is OK); \
  s21_remove_matrix(&matrix);                                  \
  s21_remove_matrix(&vector);                                  \
  ck_assert_int_eq(result.columns, 1);                         \
  ck_assert_int_eq(result.rows, 4);                            \
                                                               \
  ck_assert_float_eq_tol(result.matrix[0][0], (ex), EPSILON);  \
  ck_assert_float_eq_tol(result.matrix[1][0], (ey), EPSILON);  \
  ck_assert_float_eq_tol(result.matrix[2][0], (ez), EPSILON);  \
  s21_remove_matrix(&result);

START_TEST(rot_test_1) {
  matrix_t matrix = s21_create_rotation_matrix(Rad(90), AXIS_X, AXIS_Y);
  SIMPLE_TEST(1, 0, 0,   /**/   0, 1, 0);
}

START_TEST(rot_test_2) {
  matrix_t matrix = s21_create_rotation_matrix(Rad(90), AXIS_Y, AXIS_Z);
  SIMPLE_TEST(1, 1, 1,   /**/   1, -1, 1);
}

START_TEST(rot_test_3) {
  matrix_t matrix = s21_create_rotation_matrix(Rad(90), AXIS_Y, AXIS_X);
  SIMPLE_TEST(1, 0, 0,   /**/   0, -1, 0);
}

START_TEST(rot_test_4) {
  matrix_t matrix = s21_create_rotation_matrix(Rad(90), AXIS_Z, AXIS_Y);
  SIMPLE_TEST(1, 1, 1,   /**/   1, 1, -1);
}

START_TEST(rot_test_5) {
  matrix_t matrix = s21_create_rotation_matrix(Rad(90), AXIS_Z, AXIS_Z);
  SIMPLE_TEST(1, 1, 1,   /**/   1, 1, 1);
}


START_TEST(rot_test_zero) {
  matrix_t matrix = s21_create_rotation_matrix(Rad(0), AXIS_X, AXIS_Z);
  SIMPLE_TEST(1, 1, 1,   /**/   1, 1, 1);
}




START_TEST(scale_test_1) {
  matrix_t matrix = s21_create_scale_matrix(6.0, 3.0, 2.0);
  SIMPLE_TEST(1, 2, 3,   /**/   6, 6, 6);
}

START_TEST(scale_test_2) {
  matrix_t matrix = s21_create_scale_matrix(-6.0, -3.0, -2.0);
  SIMPLE_TEST(1, 2, 0,   /**/   -6, -6, 0.0);
}

START_TEST(scale_test_unit) {
  matrix_t matrix = s21_create_scale_matrix(1.0, 1.0, 1.0);
  SIMPLE_TEST(1, 2, 3,   /**/   1, 2, 3);
}


START_TEST(shift_test_1) {
  matrix_t matrix = s21_create_shift_matrix(10.0, -5.0, 0.0);
  SIMPLE_TEST(10, 5, 6,   /**/   20, 0, 6);
}

START_TEST(shift_test_2) {
  matrix_t matrix = s21_create_shift_matrix(0.0, 0.0, 0.0);
  SIMPLE_TEST(10, 5, 6,   /**/   10, 5, 6);
}


START_TEST(scaleshift_test) {
  matrix_t matrix = s21_create_scaleshift_matrix(0.0, 0.0, 3.0, 1.0, 2.0, 3.0);
  SIMPLE_TEST(5, 6, 7,   /**/   1, 2, 24);
}


START_TEST(projection_test) {
  matrix_t matrix = s21_create_projection_matrix(1, 0.1, 1000.0, 1.0);
  // It is only used to render, not to change 3D model
  s21_remove_matrix(&matrix);
}

START_TEST(perspective_test) {
  matrix_t matrix = s21_create_perspective_matrix(1, 0.1, 1000.0, 1.0);
  // It is only used to render, not to change 3D model
  s21_remove_matrix(&matrix);
}

START_TEST(opengl_to_camera) {
  matrix_t matrix = s21_create_view_to_camera();
  debugln("OPENGL TO CAMERA MATRIX: %$matrix_t", matrix);
  SIMPLE_TEST(1, 1, 1, /**/ 1, 1, -1);
}

START_TEST(rotations_test) {
  matrix_t matrix = s21_create_rotations_camera(Rad(180), Rad(180), Rad(180));
  SIMPLE_TEST(1, 1, 1, /**/ 1, 1, 1);
}


Suite *rotations_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Rotations");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, rot_test_1);
  tcase_add_test(tc_core, rot_test_2);
  tcase_add_test(tc_core, rot_test_3);
  tcase_add_test(tc_core, rot_test_4);
  tcase_add_test(tc_core, rot_test_5);
  tcase_add_test(tc_core, rot_test_zero);

  tcase_add_test(tc_core, scale_test_1);
  tcase_add_test(tc_core, scale_test_2);
  tcase_add_test(tc_core, scale_test_unit);

  tcase_add_test(tc_core, shift_test_1);
  tcase_add_test(tc_core, shift_test_2);
  tcase_add_test(tc_core, scaleshift_test);
  
  tcase_add_test(tc_core, projection_test);
  tcase_add_test(tc_core, perspective_test);
  tcase_add_test(tc_core, opengl_to_camera);
  tcase_add_test(tc_core, rotations_test);

  suite_add_tcase(s, tc_core);
  return s;
}