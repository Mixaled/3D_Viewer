#include <check.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../obj_parser/obj_parser.h"
#include "../s21_matrix/s21_matrix.h"
#include "../util/common_vecs.h"
#include "../util/cur_time.h"
#include "../util/prettify_c.h"

START_TEST(type_test) {
  ObjModel mdl = obj_parse_model("./tests/testis.obj");
  obj_model_free(mdl);
}

START_TEST(vertices_test) {
  ObjModel mdl = obj_parse_model("./tests/testis.obj");
  // check x
  for (int i = 0; i < 3; i++) {
    ck_assert_float_eq((float)i, mdl.vertices.data[i].x);
    ck_assert_float_eq((float)i, mdl.vertices.data[i].y);
    ck_assert_float_eq((float)i, mdl.vertices.data[i].z);
  }
  ck_assert_float_eq(3, mdl.vertices.data[3].x);
  ck_assert_float_eq(-1, mdl.vertices.data[4].x);
  ck_assert_float_eq(0.999999, mdl.vertices.data[5].x);

  ck_assert_float_eq(1, mdl.vertices.data[3].y);

  ck_assert_float_eq(2, mdl.vertices.data[3].z);
  ck_assert_float_eq(-0.999999, mdl.vertices.data[4].z);
  ck_assert_float_eq(1.000001, mdl.vertices.data[5].z);

  obj_model_free(mdl);
}

START_TEST(normals_test) {
  ObjModel mdl = obj_parse_model("./tests/testis.obj");
  // check x
  for (int i = 0; i < 3; i++) {
    ck_assert_float_eq((float)i, mdl.normals.data[i].x);
    ck_assert_float_eq((float)i, mdl.normals.data[i].y);
    ck_assert_float_eq((float)i, mdl.normals.data[i].z);
  }
  ck_assert_float_eq(3, mdl.normals.data[3].x);
  ck_assert_float_eq(-1, mdl.normals.data[4].x);
  ck_assert_float_eq(0.999999, mdl.normals.data[5].x);

  ck_assert_float_eq(1, mdl.normals.data[3].y);

  ck_assert_float_eq(2, mdl.normals.data[3].z);
  ck_assert_float_eq(-0.999999, mdl.normals.data[4].z);
  ck_assert_float_eq(1.000001, mdl.normals.data[5].z);
  
  obj_model_free(mdl);
}

START_TEST(faces_test) {
  ObjModel mdl = obj_parse_model("./tests/testis.obj");
  // check x
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_int_eq(j + 1, mdl.faces.data[i].indices.data[j].point);
      ck_assert_int_eq(j + 1, mdl.faces.data[i].indices.data[j].point);
      ck_assert_int_eq(j + 1, mdl.faces.data[i].indices.data[j].point);
    }
  }
  for (int j = 0; j < 3; j++) {
    debugln("TATA  %d", mdl.faces.data[0].indices.data[j].normal);
    ck_assert_int_eq(j + 1, mdl.faces.data[0].indices.data[j].normal);
    // ck_assert_int_eq(j+1, mdl.faces.data[0].indices.data[j].normal);
    // ck_assert_int_eq(j+1, mdl.faces.data[0].indices.data[j].normal);
  }

  debugln("Face print: %$face", mdl.faces.data[2]);
  for (int j = 0; j < 3; j++) {
    ck_assert_int_eq(j + 1, mdl.faces.data[2].indices.data[j].normal);
    ck_assert_int_eq(j + 1, mdl.faces.data[2].indices.data[j].normal);
    ck_assert_int_eq(j + 1, mdl.faces.data[2].indices.data[j].normal);
  }

  for (int i = 0; i < 5; i++) {
    ck_assert_int_eq(i + 1, mdl.faces.data[4].indices.data[i].point);
  }
  
  obj_model_free(mdl);
}

Suite *transformations_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Transformations");

  tc_core = tcase_create("Core");
  tcase_add_test(tc_core, type_test);
  tcase_add_test(tc_core, vertices_test);
  tcase_add_test(tc_core, normals_test);
  tcase_add_test(tc_core, faces_test);
  suite_add_tcase(s, tc_core);
  return s;
}
