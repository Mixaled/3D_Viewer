#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

START_TEST(test_s21_mult_number_valid) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double expected_result[][3] = {{2, 4, 6}, {8, 10, 12}, {14, 16, 18}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_mult_number(&A, 2, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 3, 3, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_mult_number_zero) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double expected_result[][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_mult_number(&A, 0, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 3, 3, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_mult_number_negative) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double expected_result[][3] = {{-1, -2, -3}, {-4, -5, -6}, {-7, -8, -9}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_mult_number(&A, -1, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 3, 3, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_mult_number_large) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double expected_result[][3] = {
      {1000, 2000, 3000}, {4000, 5000, 6000}, {7000, 8000, 9000}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_mult_number(&A, 1000, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 3, 3, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_mult_number_fraction) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double expected_result[][3] = {{0.5, 1, 1.5}, {2, 2.5, 3}, {3.5, 4, 4.5}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_mult_number(&A, 0.5, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 3, 3, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_mult_number_invalid_matrix) {
  matrix_t A, result;
  s21_nullify_matrix(&A);
  s21_nullify_matrix(&result);

  int ret_val = s21_mult_number(&A, 2, &result);
  ck_assert_int_eq(ret_val, ERROR);
}
END_TEST

START_TEST(test_s21_mult_number_null_pointers) {
  matrix_t A;
  int ret_val = s21_mult_number(NULL, 2, &A);
  ck_assert_int_eq(ret_val, ERROR);

  ret_val = s21_mult_number(&A, 2, NULL);
  ck_assert_int_eq(ret_val, ERROR);
}
END_TEST

Suite *s21_mult_number_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_mult_number");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_mult_number_valid);
  tcase_add_test(tc_core, test_s21_mult_number_zero);
  tcase_add_test(tc_core, test_s21_mult_number_negative);
  tcase_add_test(tc_core, test_s21_mult_number_large);
  tcase_add_test(tc_core, test_s21_mult_number_fraction);
  tcase_add_test(tc_core, test_s21_mult_number_invalid_matrix);
  tcase_add_test(tc_core, test_s21_mult_number_null_pointers);

  suite_add_tcase(s, tc_core);

  return s;
}