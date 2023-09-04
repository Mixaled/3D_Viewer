#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

START_TEST(test_s21_sum_matrix_null_pointers) {
  matrix_t A, B, result;
  s21_nullify_matrix(&A);
  s21_nullify_matrix(&B);
  s21_nullify_matrix(&result);

  int ret_val = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, ERROR);
  ck_assert(!s21_is_matrix_valid(&result));
}
END_TEST

START_TEST(test_s21_sum_matrix_invalid_sizes) {
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  s21_nullify_matrix(&result);

  int ret_val = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, CALC_ERROR);
  ck_assert(!s21_is_matrix_valid(&result));

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_sum_matrix_valid) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  s21_nullify_matrix(&result);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;

  int ret_val = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, OK);
  ck_assert(s21_is_matrix_valid(&result));

  ck_assert_double_eq(result.matrix[0][0], 6.0);
  ck_assert_double_eq(result.matrix[0][1], 8.0);
  ck_assert_double_eq(result.matrix[1][0], 10.0);
  ck_assert_double_eq(result.matrix[1][1], 12.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_sub_matrix_null_pointers) {
  matrix_t A, B, result;
  s21_nullify_matrix(&A);
  s21_nullify_matrix(&B);
  s21_nullify_matrix(&result);

  int ret_val = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, ERROR);
  ck_assert(!s21_is_matrix_valid(&result));
}
END_TEST

START_TEST(test_s21_sub_matrix_invalid_sizes) {
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  s21_nullify_matrix(&result);

  int ret_val = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, CALC_ERROR);
  ck_assert(!s21_is_matrix_valid(&result));

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_sub_matrix_valid) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  s21_nullify_matrix(&result);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;

  int ret_val = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, OK);
  ck_assert(s21_is_matrix_valid(&result));

  ck_assert_double_eq(result.matrix[0][0], -4.0);
  ck_assert_double_eq(result.matrix[0][1], -4.0);
  ck_assert_double_eq(result.matrix[1][0], -4.0);
  ck_assert_double_eq(result.matrix[1][1], -4.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_sum_matrix_big_matrices) {
  matrix_t A, B, result;
  s21_create_matrix(7, 7, &A);
  s21_create_matrix(7, 7, &B);
  s21_nullify_matrix(&result);

  double A_data[7][7] = {
      {1, 2, 3, 4, 5, 6, 7},        {8, 9, 10, 11, 12, 13, 14},
      {15, 16, 17, 18, 19, 20, 21}, {22, 23, 24, 25, 26, 27, 28},
      {29, 30, 31, 32, 33, 34, 35}, {36, 37, 38, 39, 40, 41, 42},
      {43, 44, 45, 46, 47, 48, 49},
  };

  double B_data[7][7] = {
      {49, 48, 47, 46, 45, 44, 43}, {42, 41, 40, 39, 38, 37, 36},
      {35, 34, 33, 32, 31, 30, 29}, {28, 27, 26, 25, 24, 23, 22},
      {21, 20, 19, 18, 17, 16, 15}, {14, 13, 12, 11, 10, 9, 8},
      {7, 6, 5, 4, 3, 2, 1},
  };

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      A.matrix[i][j] = A_data[i][j];
      B.matrix[i][j] = B_data[i][j];
    }
  }

  int ret_val = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, OK);
  ck_assert(s21_is_matrix_valid(&result));

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      ck_assert_double_eq(result.matrix[i][j], 50.0);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_sub_matrix_big_matrices) {
  matrix_t A, B, result;
  s21_create_matrix(7, 7, &A);
  s21_create_matrix(7, 7, &B);
  s21_nullify_matrix(&result);

  double A_data[7][7] = {
      {1, 2, 3, 4, 5, 6, 7},        {8, 9, 10, 11, 12, 13, 14},
      {15, 16, 17, 18, 19, 20, 21}, {22, 23, 24, 25, 26, 27, 28},
      {29, 30, 31, 32, 33, 34, 35}, {36, 37, 38, 39, 40, 41, 42},
      {43, 44, 45, 46, 47, 48, 49},
  };

  double B_data[7][7] = {
      {49, 48, 47, 46, 45, 44, 43}, {42, 41, 40, 39, 38, 37, 36},
      {35, 34, 33, 32, 31, 30, 29}, {28, 27, 26, 25, 24, 23, 22},
      {21, 20, 19, 18, 17, 16, 15}, {14, 13, 12, 11, 10, 9, 8},
      {7, 6, 5, 4, 3, 2, 1},
  };

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      A.matrix[i][j] = A_data[i][j];
      B.matrix[i][j] = B_data[i][j];
    }
  }

  int ret_val = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, OK);
  ck_assert(s21_is_matrix_valid(&result));

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      ck_assert_double_eq(result.matrix[i][j], A_data[i][j] - B_data[i][j]);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

Suite *s21_sumsub_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_sum_sub");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_sum_matrix_null_pointers);
  tcase_add_test(tc_core, test_s21_sum_matrix_invalid_sizes);
  tcase_add_test(tc_core, test_s21_sum_matrix_valid);
  tcase_add_test(tc_core, test_s21_sub_matrix_null_pointers);
  tcase_add_test(tc_core, test_s21_sub_matrix_invalid_sizes);
  tcase_add_test(tc_core, test_s21_sub_matrix_valid);
  tcase_add_test(tc_core, test_s21_sum_matrix_big_matrices);
  tcase_add_test(tc_core, test_s21_sub_matrix_big_matrices);

  suite_add_tcase(s, tc_core);

  return s;
}