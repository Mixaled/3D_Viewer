#include <check.h>
#include <stdlib.h>


typedef Suite *(*SuiteFn)();

Suite *rotations_suite(void);
Suite *transformations_suite(void);

Suite *s21_remove_matrix_suite(void);
Suite *s21_create_matrix_suite(void);
Suite *s21_sumsub_suite(void);
Suite *s21_matrix_mult_suite(void);
Suite *s21_matrix_mult_suite_2(void);
Suite *s21_mult_number_suite(void);
Suite *s21_eq_matrix_suite(void);
Suite *s21_inverse_matrix_suite(void);
Suite *s21_transpose_suite(void);
Suite *s21_calc_complements_suite(void);
Suite *s21_determinant_suite(void);

int main(void) {
  const SuiteFn suites[] = {s21_remove_matrix_suite, s21_create_matrix_suite,
                            s21_sumsub_suite,        s21_matrix_mult_suite,
                            s21_matrix_mult_suite_2, s21_mult_number_suite,
                            s21_eq_matrix_suite,     s21_inverse_matrix_suite,
                            s21_transpose_suite,     s21_calc_complements_suite,
                            s21_determinant_suite};
  int suites_len = sizeof(suites) / sizeof(suites[0]);

  SRunner *sr = srunner_create(NULL);
  srunner_add_suite(sr, rotations_suite());
  srunner_add_suite(sr, transformations_suite());
  for (int i = 0; i < suites_len; i++) srunner_add_suite(sr, suites[i]());

  srunner_run_all(sr, CK_NORMAL);

  int number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);

  return (number_failed == 0) ? 0 : 1;
}