#ifndef SRC_MATRIX_S21_MINOR_H_
#define SRC_MATRIX_S21_MINOR_H_

#include "s21_matrix.h"

enum RefType {
  REFTYPE_MATRIX,
  REFTYPE_MINOR,
};

typedef struct matrix_minor {
  union {
    const matrix_t* matrix;
    const struct matrix_minor* minor;
  } reference;

  int ref_type;

  int removed_row;
  int removed_column;
  int rows;
  int columns;
} minor_t;

minor_t s21_get_minor_of_matrix(const matrix_t* matrix, int remove_row,
                                int remove_column);
minor_t s21_get_minor_of_minor(const minor_t* minor, int remove_row,
                               int remove_column);
double s21_minor_get(const minor_t* minor, int i, int j);

int s21_determinant_minor(const minor_t* minor, double* result);  // done
#endif