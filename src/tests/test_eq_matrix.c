// test_eq_matrix.c

#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

#define EPS 1e-7

START_TEST(test_s21_eq_matrix_valid) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double source_b[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

  matrix_t A, B;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 3, 3, &B);

  int eq_result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_eq_matrix_invalid) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double source_b[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

  matrix_t A, B;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 3, 3, &B);

  int eq_result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(eq_result, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_eq_matrix_invalid_sizes) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double source_b[][2] = {{1, 2}, {4, 5}, {7, 8}};

  matrix_t A, B;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 3, 2, &B);

  int eq_result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(eq_result, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_eq_matrix_invalid_matrices) {
  matrix_t A, B;
  s21_nullify_matrix(&A);
  s21_nullify_matrix(&B);

  int eq_result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(eq_result, FAILURE);
}
END_TEST

START_TEST(test_s21_eq_matrix_almost_equal) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double source_b[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9 - EPS / 2}};

  matrix_t A, B;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 3, 3, &B);

  int eq_result = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

Suite *s21_eq_matrix_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_eq_matrix");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_eq_matrix_valid);
  tcase_add_test(tc_core, test_s21_eq_matrix_invalid);
  tcase_add_test(tc_core, test_s21_eq_matrix_invalid_sizes);
  tcase_add_test(tc_core, test_s21_eq_matrix_invalid_matrices);
  tcase_add_test(tc_core, test_s21_eq_matrix_almost_equal);
  suite_add_tcase(s, tc_core);

  return s;
}