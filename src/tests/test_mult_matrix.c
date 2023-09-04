#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

START_TEST(test_mult_matrix) {
  double source_a[][3] = {{1, 2, 3}};
  double source_b[][1] = {{4}, {5}, {6}};
  double expected_result[][1] = {{32}};

  matrix_t A, B, result;
  s21_fill_matrix_from_local_array((double *)source_a, 1, 3, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 3, 1, &B);

  int ret_val = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 1, 1, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_matrix_null_pointers) {
  matrix_t A;
  int ret_val = s21_mult_matrix(NULL, &A, &A);
  ck_assert_int_eq(ret_val, ERROR);

  ret_val = s21_mult_matrix(&A, NULL, &A);
  ck_assert_int_eq(ret_val, ERROR);

  ret_val = s21_mult_matrix(&A, &A, NULL);
  ck_assert_int_eq(ret_val, ERROR);
}
END_TEST

START_TEST(test_mult_matrix_invalid_matrices) {
  matrix_t A, B, result;
  s21_nullify_matrix(&A);
  s21_nullify_matrix(&B);

  int ret_val = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, ERROR);
}
END_TEST

START_TEST(test_mult_matrix_non_matching_sizes) {
  double source_a[][3] = {{1, 2, 3}};
  double source_b[][2] = {{4, 5}, {6, 7}};

  matrix_t A, B, result;
  s21_fill_matrix_from_local_array((double *)source_a, 1, 3, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 2, 2, &B);

  int ret_val = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, CALC_ERROR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

Suite *s21_matrix_mult_suite() {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_mult_matrix");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_mult_matrix);
  tcase_add_test(tc_core, test_mult_matrix_null_pointers);
  tcase_add_test(tc_core, test_mult_matrix_invalid_matrices);
  tcase_add_test(tc_core, test_mult_matrix_non_matching_sizes);
  suite_add_tcase(s, tc_core);

  return s;
}