#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

START_TEST(test_s21_inverse_matrix_invalid_matrix) {
  matrix_t A, result;
  s21_nullify_matrix(&A);
  s21_nullify_matrix(&result);

  int ret_val = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(ret_val, ERROR);
}
END_TEST

START_TEST(test_s21_inverse_matrix_invalid_size) {
  matrix_t A, result;
  s21_create_matrix(2, 3, &A);
  s21_nullify_matrix(&result);

  int ret_val = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(ret_val, CALC_ERROR);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_s21_inverse_matrix_singular) {
  double source_a[][2] = {{1, 2}, {2, 4}};
  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 2, 2, &A);
  s21_nullify_matrix(&result);

  int ret_val = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(ret_val, CALC_ERROR);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_s21_inverse_matrix_valid_1) {
  double source_a[][2] = {{4, 7}, {2, 6}};
  double expected_result[][2] = {{0.6, -0.7}, {-0.2, 0.4}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 2, 2, &A);

  int ret_val = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 2, 2, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_inverse_matrix_valid_2) {
  double source_a[][3] = {{1, 2, 3}, {0, 1, 4}, {5, 6, 0}};
  double expected_result[][3] = {{-24, 18, 5}, {20, -15, -4}, {-5, 4, 1}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_inverse_matrix(&A, &result);
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

START_TEST(test_s21_inverse_matrix_valid_3) {
  double source_a[][3] = {{2, 0, 0}, {0, 3, 0}, {0, 0, 4}};
  double expected_result[][3] = {{0.5, 0, 0}, {0, 1.0 / 3, 0}, {0, 0, 0.25}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_inverse_matrix(&A, &result);
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

START_TEST(test_s21_inverse_matrix_valid_4) {
  double source_a[][2] = {{3, 0}, {0, 3}};
  double expected_result[][2] = {{1.0 / 3, 0}, {0, 1.0 / 3}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 2, 2, &A);

  int ret_val = s21_inverse_matrix(&A, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 2, 2, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_inverse_matrix_valid_5) {
  double source_a[][3] = {{1, 0, 0}, {0, 2, 0}, {0, 0, 3}};
  double expected_result[][3] = {{1, 0, 0}, {0, 0.5, 0}, {0, 0, 1.0 / 3}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_inverse_matrix(&A, &result);
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

Suite *s21_inverse_matrix_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_inverse_matrix");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_inverse_matrix_invalid_matrix);
  tcase_add_test(tc_core, test_s21_inverse_matrix_invalid_size);
  tcase_add_test(tc_core, test_s21_inverse_matrix_singular);
  tcase_add_test(tc_core, test_s21_inverse_matrix_valid_1);
  tcase_add_test(tc_core, test_s21_inverse_matrix_valid_2);
  tcase_add_test(tc_core, test_s21_inverse_matrix_valid_3);
  tcase_add_test(tc_core, test_s21_inverse_matrix_valid_4);
  tcase_add_test(tc_core, test_s21_inverse_matrix_valid_5);

  suite_add_tcase(s, tc_core);

  return s;
}
