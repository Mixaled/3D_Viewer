#include <stdlib.h>

#include "s21_matrix.h"
#include "s21_minor.h"

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int ret_val = OK;

  if (result == NULL) {
    ret_val = ERROR;
  } else {
    s21_nullify_matrix(result);
    if (!s21_is_matrix_valid(A)) {
      ret_val = ERROR;
    } else if (A->rows != A->columns) {
      ret_val = CALC_ERROR;
    } else if (A->rows == 1 && A->columns == 1) {
      ret_val = s21_create_matrix(1, 1, result);

      if (ret_val == OK) result->matrix[0][0] = 1.0;
    } else if ((ret_val = s21_create_matrix(A->rows, A->columns, result)) ==
               OK) {
      for (int i = 0; i < A->rows && ret_val == OK; i++) {
        for (int j = 0; j < A->columns && ret_val == OK; j++) {
          minor_t minor = s21_get_minor_of_matrix(A, i, j);
          double determinant = 0.0;
          ret_val = s21_determinant_minor(&minor, &determinant);
          double sign = (((i + j) % 2) == 0) ? 1.0 : -1.0;
          result->matrix[i][j] = determinant * sign;
        }
      }

      if (ret_val != OK) s21_remove_matrix(result);
    }
  }

  return ret_val;
}