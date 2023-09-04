#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

START_TEST(test_s21_create_matrix_valid) {
  matrix_t result;
  int ret_value = s21_create_matrix(3, 3, &result);

  ck_assert_int_eq(ret_value, OK);
  ck_assert_ptr_nonnull(result.matrix);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);

  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_create_matrix_invalid_rows) {
  matrix_t result;
  int ret_value = s21_create_matrix(-1, 3, &result);

  ck_assert_int_eq(ret_value, ERROR);
  ck_assert_ptr_null(result.matrix);
  ck_assert_int_eq(result.rows, 0);
  ck_assert_int_eq(result.columns, 0);
}
END_TEST

START_TEST(test_s21_create_matrix_invalid_columns) {
  matrix_t result;
  int ret_value = s21_create_matrix(3, -1, &result);

  ck_assert_int_eq(ret_value, ERROR);
  ck_assert_ptr_null(result.matrix);
  ck_assert_int_eq(result.rows, 0);
  ck_assert_int_eq(result.columns, 0);
}
END_TEST

START_TEST(test_s21_create_matrix_null_result) {
  int ret_value = s21_create_matrix(3, 3, NULL);

  ck_assert_int_eq(ret_value, ERROR);
}
END_TEST

Suite *s21_create_matrix_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_create_matrix");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_create_matrix_valid);
  tcase_add_test(tc_core, test_s21_create_matrix_invalid_rows);
  tcase_add_test(tc_core, test_s21_create_matrix_invalid_columns);
  tcase_add_test(tc_core, test_s21_create_matrix_null_result);
  suite_add_tcase(s, tc_core);

  return s;
}