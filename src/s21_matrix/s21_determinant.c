#include <math.h>
#include <stdlib.h>

#include "s21_matrix.h"
#include "s21_minor.h"

int s21_determinant(matrix_t *A, double *result) {
  *result = NAN;
  int ret_value = OK;

  if (!s21_is_matrix_valid(A))
    ret_value = ERROR;
  else if (A->rows != A->columns)
    ret_value = CALC_ERROR;
  else {
    minor_t matrix = s21_get_minor_of_matrix(A, -1, -1);
    ret_value = s21_determinant_minor(&matrix, result);
  }

  return ret_value;
}

int s21_determinant_minor(const minor_t *minor, double *result) {
  *result = NAN;
  int ret_value = OK;

  if (minor->rows == 1 || minor->columns == 1) {
    if (minor->rows != 1 || minor->columns != 1)
      ret_value = CALC_ERROR;
    else
      *result = s21_minor_get(minor, 0, 0);
  } else {
    double sum = 0.0;

    const int i = 0;
    for (int j = 0; j < minor->columns && ret_value == OK; j++) {
      minor_t section = s21_get_minor_of_minor(minor, i, j);
      double section_determinant = 0.0;
      ret_value = s21_determinant_minor(&section, &section_determinant);

      double sign = (((i + j) % 2) == 0) ? 1.0 : -1.0;
      sum += sign * s21_minor_get(minor, i, j) * section_determinant;
    }

    (*result) = sum;
  }

  return ret_value;
}