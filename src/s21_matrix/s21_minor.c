#include "s21_minor.h"

minor_t s21_get_minor_of_matrix(const matrix_t* matrix, int remove_row,
                                int remove_column) {
  minor_t m = {
      .ref_type = REFTYPE_MATRIX,
      .removed_row = remove_row,
      .removed_column = remove_column,
      .rows = (remove_row >= 0) ? (matrix->rows - 1) : (matrix->rows),
      .columns =
          (remove_column >= 0) ? (matrix->columns - 1) : (matrix->columns),
  };
  m.reference.matrix = matrix;
  return m;
}

minor_t s21_get_minor_of_minor(const minor_t* minor, int remove_row,
                               int remove_column) {
  minor_t m = {
      .ref_type = REFTYPE_MINOR,
      .removed_row = remove_row,
      .removed_column = remove_column,
      .rows = (remove_row >= 0) ? (minor->rows - 1) : (minor->rows),
      .columns = (remove_column >= 0) ? (minor->columns - 1) : (minor->columns),
  };
  m.reference.minor = minor;
  return m;
}

double s21_minor_get(const minor_t* minor, int i, int j) {
  int underlying_i =
      (i >= minor->removed_row && minor->removed_row >= 0) ? (i + 1) : i;
  int underlying_j =
      (j >= minor->removed_column && minor->removed_column >= 0) ? (j + 1) : j;

  double result;
  if (minor->ref_type == REFTYPE_MATRIX) {
    result = minor->reference.matrix->matrix[underlying_i][underlying_j];
  } else if (minor->ref_type == REFTYPE_MINOR) {
    result = s21_minor_get(minor->reference.minor, underlying_i, underlying_j);
  } else {
    result = -42.0;
    // unreachable
  }

  return result;
}