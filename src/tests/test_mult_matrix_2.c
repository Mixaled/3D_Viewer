#include <check.h>

#include "../s21_matrix/s21_matrix.h"
#include "test.h"

START_TEST(test_mult_matrix_1) {
  double source_a[][1] = {{10}, {7}, {6}, {9}, {6}, {6}, {5}, {2}};
  double source_b[][10] = {{6, 8, 5, 4, 4, 5, 9, 7, 1, 10}};
  double expected_result[][10] = {{60, 80, 50, 40, 40, 50, 90, 70, 10, 100},
                                  {42, 56, 35, 28, 28, 35, 63, 49, 7, 70},
                                  {36, 48, 30, 24, 24, 30, 54, 42, 6, 60},
                                  {54, 72, 45, 36, 36, 45, 81, 63, 9, 90},
                                  {36, 48, 30, 24, 24, 30, 54, 42, 6, 60},
                                  {36, 48, 30, 24, 24, 30, 54, 42, 6, 60},
                                  {30, 40, 25, 20, 20, 25, 45, 35, 5, 50},
                                  {12, 16, 10, 8, 8, 10, 18, 14, 2, 20}};

  matrix_t A, B, result;
  s21_fill_matrix_from_local_array((double *)source_a, 8, 1, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 1, 10, &B);

  int ret_val = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 8, 10, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_matrix_2) {
  double source_a[][5] = {{5, 6, 1, 8, 10}, {1, 10, 7, 2, 4}, {5, 4, 8, 4, 1}};
  double source_b[][2] = {{4, 5}, {4, 4}, {2, 8}, {6, 4}, {9, 6}};
  double expected_result[][2] = {{184, 149}, {106, 133}, {85, 127}};

  matrix_t A, B, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 5, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 5, 2, &B);

  int ret_val = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 3, 2, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_matrix_3) {
  double source_a[][9] = {
      {6, 8, 3, 5, 4, 5, 10, 7, 2},  {2, 10, 2, 2, 4, 3, 4, 5, 7},
      {1, 6, 5, 3, 10, 7, 10, 3, 5}, {6, 10, 1, 3, 8, 5, 2, 6, 3},
      {8, 1, 4, 10, 8, 4, 3, 10, 5}, {7, 5, 5, 9, 8, 9, 3, 7, 4}};
  double source_b[][7] = {
      {9, 6, 10, 10, 7, 5, 10}, {4, 8, 5, 3, 8, 10, 1}, {4, 6, 3, 3, 4, 8, 3},
      {6, 10, 8, 7, 7, 2, 3},   {7, 3, 1, 2, 8, 5, 10}, {8, 1, 10, 3, 8, 3, 3},
      {2, 10, 3, 4, 3, 5, 4},   {7, 7, 2, 9, 4, 6, 8},  {1, 9, 9, 8, 1, 10, 4}};
  double expected_result[][7] = {
      {267, 352, 265, 270, 285, 291, 251}, {180, 277, 211, 204, 211, 279, 175},
      {243, 317, 240, 212, 279, 300, 245}, {261, 270, 240, 231, 284, 275, 245},
      {321, 353, 299, 335, 300, 279, 327}, {344, 350, 339, 313, 349, 307, 308}};

  matrix_t A, B, result;
  s21_fill_matrix_from_local_array((double *)source_a, 6, 9, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 9, 7, &B);

  int ret_val = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 6, 7, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_matrix_4) {
  double source_a[][9] = {{1, 3, 10, 9, 5, 9, 5, 5, 9},
                          {8, 3, 10, 1, 4, 6, 3, 7, 7},
                          {4, 7, 6, 5, 4, 7, 3, 10, 8}};
  double source_b[][2] = {{2, 4}, {2, 10}, {9, 7}, {5, 3}, {6, 6},
                          {9, 2}, {4, 2},  {9, 9}, {3, 1}};
  double expected_result[][2] = {{346, 243}, {291, 247}, {314, 285}};

  matrix_t A, B, result;
  s21_fill_matrix_from_local_array((double *)source_a, 3, 9, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 9, 2, &B);

  int ret_val = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 3, 2, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(test_mult_matrix_5) {
  double source_a[][5] = {
      {2, 1, 4, 4, 10}, {10, 10, 9, 7, 10}, {2, 10, 8, 7, 6}, {6, 5, 2, 5, 2},
      {5, 6, 10, 2, 1}, {10, 2, 10, 6, 3},  {6, 1, 3, 4, 2},  {7, 7, 2, 10, 3},
      {6, 6, 9, 9, 9},  {2, 5, 1, 3, 5}};
  double source_b[][9] = {{10, 8, 4, 6, 9, 8, 8, 5, 8},
                          {9, 8, 5, 6, 3, 4, 1, 4, 5},
                          {8, 10, 9, 9, 10, 4, 2, 5, 7},
                          {3, 8, 8, 9, 7, 4, 9, 5, 1},
                          {9, 6, 9, 9, 10, 2, 7, 4, 2}};
  double expected_result[][9] = {{163, 156, 171, 180, 189, 72, 131, 94, 73},
                                 {373, 366, 317, 354, 359, 204, 241, 210, 220},
                                 {249, 268, 240, 261, 237, 128, 147, 149, 141},
                                 {154, 160, 125, 147, 144, 100, 116, 93, 96},
                                 {199, 210, 165, 183, 187, 114, 91, 113, 144},
                                 {243, 262, 215, 243, 268, 158, 177, 150, 172},
                                 {123, 130, 106, 123, 135, 84, 105, 77, 82},
                                 {206, 230, 188, 219, 204, 138, 178, 135, 121},
                                 {294, 312, 288, 315, 315, 162, 216, 180, 168},
                                 {127, 120, 111, 123, 114, 62, 85, 70, 61}};

  matrix_t A, B, result;
  s21_fill_matrix_from_local_array((double *)source_a, 10, 5, &A);
  s21_fill_matrix_from_local_array((double *)source_b, 5, 9, &B);

  int ret_val = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(ret_val, OK);

  matrix_t expected;
  s21_fill_matrix_from_local_array((double *)expected_result, 10, 9, &expected);

  int eq_result = s21_eq_matrix(&result, &expected);
  ck_assert_int_eq(eq_result, SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&expected);
}
END_TEST

Suite *s21_matrix_mult_suite_2(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_mult_matrix_2");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_mult_matrix_1);
  tcase_add_test(tc_core, test_mult_matrix_2);
  tcase_add_test(tc_core, test_mult_matrix_3);
  tcase_add_test(tc_core, test_mult_matrix_4);
  tcase_add_test(tc_core, test_mult_matrix_5);
  suite_add_tcase(s, tc_core);

  return s;
}
