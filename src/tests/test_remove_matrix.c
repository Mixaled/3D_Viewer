#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

START_TEST(test_s21_remove_matrix_valid) {
  matrix_t matrix;
  int ret_value = s21_create_matrix(3, 3, &matrix);

  ck_assert_int_eq(ret_value, OK);
  ck_assert_ptr_nonnull(matrix.matrix);
  ck_assert_int_eq(matrix.rows, 3);
  ck_assert_int_eq(matrix.columns, 3);

  s21_remove_matrix(&matrix);

  ck_assert_ptr_null(matrix.matrix);
  ck_assert_int_eq(matrix.rows, 0);
  ck_assert_int_eq(matrix.columns, 0);
}
END_TEST

START_TEST(test_s21_remove_matrix_null) {
  matrix_t matrix = {NULL, 0, 0};

  s21_remove_matrix(&matrix);

  ck_assert_ptr_null(matrix.matrix);
  ck_assert_int_eq(matrix.rows, 0);
  ck_assert_int_eq(matrix.columns, 0);
}
END_TEST

START_TEST(test_s21_remove_matrix_invalid) {
  matrix_t matrix = {(double **)0xdeadbeef, -1, -1};

  s21_remove_matrix(&matrix);

  ck_assert_ptr_eq(matrix.matrix, (double **)0xdeadbeef);
  ck_assert_int_eq(matrix.rows, -1);
  ck_assert_int_eq(matrix.columns, -1);
}
END_TEST

Suite *s21_remove_matrix_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_remove_matrix");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_remove_matrix_valid);
  tcase_add_test(tc_core, test_s21_remove_matrix_null);
  tcase_add_test(tc_core, test_s21_remove_matrix_invalid);
  suite_add_tcase(s, tc_core);

  return s;
}