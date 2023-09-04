#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

START_TEST(test_s21_transpose_valid_1) {
  double source_a[][2] = {{1, 2}, {3, 4}};
  double expected_result[][2] = {{1, 3}, {2, 4}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 2, 2, &A);

  int ret_val = s21_transpose(&A, &result);
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

START_TEST(test_s21_transpose_valid_2) {
  double source_a[][3] = {{1, 2, 3}, {4, 5, 6}};
  double expected_result[][2] = {{1, 4}, {2, 5}, {3, 6}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 2, 3, &A);

  int ret_val = s21_transpose(&A, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 3, 2, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_transpose_valid_3) {
  double source_a[][2] = {{1, 4}, {2, 5}, {3, 6}};
  double expected_result[][3] = {{1, 2, 3}, {4, 5, 6}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 2, &A);

  int ret_val = s21_transpose(&A, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 2, 3, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_transpose_valid_4) {
  double source_a[][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  double expected_result[][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 3, &A);

  int ret_val = s21_transpose(&A, &result);
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

START_TEST(test_s21_transpose_valid_5) {
  double source_a[][1] = {{1}, {2}, {3}};
  double expected_result[][3] = {{1, 2, 3}};

  matrix_t A, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 1, &A);

  int ret_val = s21_transpose(&A, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 1, 3, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_s21_transpose_null_pointer_A) {
  matrix_t result;
  s21_nullify_matrix(&result);

  int ret_val = s21_transpose(NULL, &result);
  ck_assert_int_eq(ret_val, ERROR);
}
END_TEST

START_TEST(test_s21_transpose_null_pointer_result) {
  double source_a[][2] = {{1, 2}, {3, 4}};

  matrix_t A;
  s21_fill_matrix_from_local_array((double *)source_a, 2, 2, &A);

  int ret_val = s21_transpose(&A, NULL);
  ck_assert_int_eq(ret_val, ERROR);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_s21_transpose_invalid_size_A) {
  matrix_t A, result;
  s21_create_matrix(0, 2, &A);
  s21_nullify_matrix(&result);

  int ret_val = s21_transpose(&A, &result);
  ck_assert_int_eq(ret_val, ERROR);

  s21_remove_matrix(&A);
}
END_TEST

Suite *s21_transpose_suite(void) {
  Suite *s;
  TCase *tc_core, *tc_edge;

  s = suite_create("s21_transpose_matrix");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_transpose_valid_1);
  tcase_add_test(tc_core, test_s21_transpose_valid_2);
  tcase_add_test(tc_core, test_s21_transpose_valid_3);
  tcase_add_test(tc_core, test_s21_transpose_valid_4);
  tcase_add_test(tc_core, test_s21_transpose_valid_5);
  suite_add_tcase(s, tc_core);

  tc_edge = tcase_create("s21_transpose_matrix edges");

  tcase_add_test(tc_edge, test_s21_transpose_null_pointer_A);
  tcase_add_test(tc_edge, test_s21_transpose_null_pointer_result);
  tcase_add_test(tc_edge, test_s21_transpose_invalid_size_A);
  suite_add_tcase(s, tc_edge);

  return s;
}