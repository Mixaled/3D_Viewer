#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

START_TEST(test_s21_determinant_invalid_matrix) {
  matrix_t A;
  double result;
  s21_nullify_matrix(&A);

  int ret_val = s21_determinant(&A, &result);
  ck_assert_int_eq(ret_val, ERROR);
}
END_TEST

START_TEST(test_s21_determinant_non_square_matrix) {
  matrix_t A;
  double result;
  s21_create_matrix(2, 3, &A);

  int ret_val = s21_determinant(&A, &result);
  ck_assert_int_eq(ret_val, CALC_ERROR);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_s21_determinant_1x1_matrix) {
  matrix_t A;
  double result;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;

  int ret_val = s21_determinant(&A, &result);
  ck_assert_int_eq(ret_val, OK);
  ck_assert_double_eq(result, 5.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_s21_determinant_2x2_matrix) {
  double source_a[][2] = {{4, 7}, {2, 6}};
  matrix_t A;
  double result;
  s21_fill_matrix_from_local_array((double *)source_a, 2, 2, &A);

  int ret_val = s21_determinant(&A, &result);
  ck_assert_int_eq(ret_val, OK);
  ck_assert_double_eq(result, 10.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_s21_determinant_3x3_matrix) {
  double source_a[][3] = {{1, 2, 3}, {0, 1, 4}, {5, 6, 0}};
  matrix_t A;
  double result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_determinant(&A, &result);
  ck_assert_int_eq(ret_val, OK);
  ck_assert_double_eq_tol(result, 1.0, 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

Suite *s21_determinant_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_determinant");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_determinant_invalid_matrix);
  tcase_add_test(tc_core, test_s21_determinant_non_square_matrix);
  tcase_add_test(tc_core, test_s21_determinant_1x1_matrix);
  tcase_add_test(tc_core, test_s21_determinant_2x2_matrix);
  tcase_add_test(tc_core, test_s21_determinant_3x3_matrix);

  suite_add_tcase(s, tc_core);

  return s;
}