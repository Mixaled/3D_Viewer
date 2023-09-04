#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_matrix.h"

#define EPS 1e-7

void s21_nullify_matrix(matrix_t *matrix) {
  matrix->matrix = NULL;
  matrix->rows = 0;
  matrix->columns = 0;
}

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int ret_value = OK;

  if (result == NULL) {
    ret_value = ERROR;
  } else if (rows <= 0 || columns <= 0) {
    ret_value = ERROR;
    s21_nullify_matrix(result);
  } else {
    s21_nullify_matrix(result);
    size_t data_size = rows * columns * sizeof(double);
    size_t pointers_size = rows * sizeof(double *);

    size_t total_size = data_size + pointers_size;
    double **array = (double **)calloc(total_size, 1);
    double *data_ptr = (double *)(array + rows);

    if (array == NULL) {
      ret_value = ERROR;
    } else {
      for (int i = 0; i < rows; i++) array[i] = data_ptr + columns * i;

      result->matrix = array;
      result->rows = rows;
      result->columns = columns;
    }
  }

  return ret_value;
}

void s21_remove_matrix(matrix_t *a) {
  if (s21_is_matrix_valid(a)) {
    free(a->matrix);
    s21_nullify_matrix(a);
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int result = SUCCESS;

  if (!s21_is_matrix_valid(A) || !s21_is_matrix_valid(B)) {
    result = FAILURE;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    result = FAILURE;
  } else {
    for (int i = 0; i < A->rows && result == SUCCESS; i++)
      for (int j = 0; j < A->columns && result == SUCCESS; j++)
        if (fabsl(A->matrix[i][j] - B->matrix[i][j]) >= EPS) result = FAILURE;
  }

  return result;
}

static int s21_add_sub(matrix_t *A, matrix_t *B, matrix_t *result,
                       int operation) {
  s21_nullify_matrix(result);
  int ret_val = OK;

  if (!s21_is_matrix_valid(A) || !s21_is_matrix_valid(B)) {
    ret_val = ERROR;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    ret_val = CALC_ERROR;
  } else if ((ret_val = s21_create_matrix(A->rows, A->columns, result)) == OK) {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        if (operation == 0)
          result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        else
          result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
  }

  return ret_val;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return s21_add_sub(A, B, result, 0);
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  return s21_add_sub(A, B, result, 1);
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int ret_val = OK;
  if (result == NULL) {
    ret_val = ERROR;
  } else {
    s21_nullify_matrix(result);

    if (!s21_is_matrix_valid(A)) {
      ret_val = ERROR;
    } else if ((ret_val = s21_create_matrix(A->rows, A->columns, result)) ==
               OK) {
      for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < A->columns; j++)
          result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }

  return ret_val;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int ret_val = OK;

  if (result == NULL) {
    ret_val = ERROR;
  } else {
    s21_nullify_matrix(result);
    if (!s21_is_matrix_valid(A)) {
      ret_val = ERROR;
    } else if ((ret_val = s21_create_matrix(A->columns, A->rows, result)) ==
               OK) {
      for (int i = 0; i < A->rows; i++)
        for (int j = 0; j < A->columns; j++)
          result->matrix[j][i] = A->matrix[i][j];
    }
  }

  return ret_val;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int ret_val = OK;

  if (result == NULL) {
    ret_val = ERROR;
  } else {
    s21_nullify_matrix(result);
    if (!s21_is_matrix_valid(A) || !s21_is_matrix_valid(B)) {
      ret_val = ERROR;
    } else if (A->columns != B->rows) {
      ret_val = CALC_ERROR;
    } else if ((ret_val = s21_create_matrix(A->rows, B->columns, result)) ==
               OK) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
          result->matrix[i][j] = 0.0;

          for (int k = 0; k < A->columns; k++)
            result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
        }
      }
    }
  }

  return ret_val;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  s21_nullify_matrix(result);
  int ret_value = OK;

  if (!s21_is_matrix_valid(A)) {
    ret_value = ERROR;
  } else {
    double determinant = 0.0;
    ret_value = s21_determinant(A, &determinant);
    if (ret_value == OK) {
      if (determinant == 0.0) {
        ret_value = CALC_ERROR;
      } else {
        ret_value = s21_calc_complements(A, result);

        if (ret_value == OK) {
          matrix_t complements;
          ret_value = s21_transpose(result, &complements);
          s21_remove_matrix(result);

          if (ret_value == OK) {
            ret_value =
                s21_mult_number(&complements, 1.0 / determinant, result);
            s21_remove_matrix(&complements);
          }
        }
      }
    }
  }

  return ret_value;
}

bool s21_is_matrix_valid(const matrix_t *A) {
  return A != NULL && A->matrix != NULL && A->rows > 0 && A->columns > 0;
}