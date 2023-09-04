#ifndef SRC_TESTS_TESTS_H_
#define SRC_TESTS_TESTS_H_

#include "../s21_matrix/s21_matrix.h"

int s21_compare_doubles(double a, double b);
void s21_fill_matrix_from_local_array(double *local_array, int rows,
                                      int columns, matrix_t *matrix);

#endif