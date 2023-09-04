#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

START_TEST(test_s21_calc_complements_invalid_matrix) {
  matrix_t A, result;
  s21_nullify_matrix(&A);
  s21_nullify_matrix(&result);

  int ret_val = s21_calc_complements(&A, &result);
  ck_assert_int_eq(ret_val, ERROR);
}
END_TEST

START_TEST(test_s21_calc_complements_1x1_matrix) {
  double source_a[][1] = {{5}};
  double expected_result[][1] = {{1}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 1, 1, &A);

  int ret_val = s21_calc_complements(&A, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 1, 1, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_calc_complements_2x2_matrix) {
  double source_a[][2] = {{1, 2}, {3, 4}};
  double expected_result[][2] = {{4, -3}, {-2, 1}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 2, 2, &A);

  int ret_val = s21_calc_complements(&A, &result);
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

START_TEST(test_s21_calc_complements_3x3_matrix) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  double expected_result[][3] = {{-3, 6, -3}, {6, -12, 6}, {-3, 6, -3}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_calc_complements(&A, &result);
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

START_TEST(test_s21_calc_complements_4x4_matrix) {
  double source_a[][4] = {
      {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
  double expected_result[][4] = {
      {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 4, 4, &A);

  int ret_val = s21_calc_complements(&A, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 4, 4, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_calc_complements_non_square_matrix) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 2, 3, &A);

  int ret_val = s21_calc_complements(&A, &result);
  ck_assert_int_eq(ret_val, CALC_ERROR);

  s21_remove_matrix(&A);
}
END_TEST

Suite *s21_calc_complements_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_calc_complements");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_calc_complements_invalid_matrix);
  tcase_add_test(tc_core, test_s21_calc_complements_1x1_matrix);
  tcase_add_test(tc_core, test_s21_calc_complements_2x2_matrix);
  tcase_add_test(tc_core, test_s21_calc_complements_3x3_matrix);
  tcase_add_test(tc_core, test_s21_calc_complements_4x4_matrix);
  tcase_add_test(tc_core, test_s21_calc_complements_non_square_matrix);

  suite_add_tcase(s, tc_core);

  return s;
}