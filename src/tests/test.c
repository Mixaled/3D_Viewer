#include "test.h"

#include <assert.h>
#include <check.h>
#include <math.h>

#include "../s21_matrix/s21_matrix.h"

int s21_compare_doubles(double a, double b) { return fabs(a - b) < 1e-7; }

void s21_fill_matrix_from_local_array(double *local_array, int rows,
                                      int columns, matrix_t *matrix) {
  assert(local_array != NULL);
  assert(rows > 0 && columns > 0);
  assert(s21_create_matrix(rows, columns, matrix) == OK);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      matrix->matrix[i][j] = local_array[i * columns + j];
    }
  }
}