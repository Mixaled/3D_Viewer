#ifndef SRC_S21_MATRIX_H_
#define SRC_S21_MATRIX_H_

#include <stdbool.h>
#include "../util/better_io.h"

#define SUCCESS 1
#define FAILURE 0

#define OK 0
#define ERROR 1
#define CALC_ERROR 2

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

// Main functions
int s21_create_matrix(int rows, int columns, matrix_t *result);  // done+
void s21_remove_matrix(matrix_t *A);                             // done+

int s21_eq_matrix(matrix_t *A, matrix_t *B);  // done+

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);  // done+
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);  // done+

int s21_mult_number(matrix_t *A, double number, matrix_t *result);  // done+
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);    // done+

int s21_transpose(matrix_t *A, matrix_t *result);         // done+
int s21_calc_complements(matrix_t *A, matrix_t *result);  // done+
int s21_determinant(matrix_t *A, double *result);         // done+
int s21_inverse_matrix(matrix_t *A, matrix_t *result);    // done+

// Other very handy functions
void s21_nullify_matrix(matrix_t *matrix);    // done
bool s21_is_matrix_valid(const matrix_t *A);  // done
// void s21_print_matrix(matrix_t *A, const char *double_format);  // done

// 3D graphics stuff
matrix_t s21_create_unit_matrix();
matrix_t s21_create_shift_matrix(double dx, double dy, double dz);
matrix_t s21_create_scale_matrix(double sx, double sy, double sz);
matrix_t s21_create_scaleshift_matrix(double sx, double sy, double sz, double dx, double dy, double dz);
matrix_t s21_create_projection_matrix(double size, double near, double far, double aspect_ratio);
matrix_t s21_create_perspective_matrix(double fov, double near, double far, double aspect_ratio);
matrix_t s21_create_view_to_camera();

typedef struct FloatArray16 {
  float data[16];
} FloatArray16;
FloatArray16 s21_matrix_to_farray(const matrix_t* m);

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2
matrix_t s21_create_rotation_matrix(double angle, int axis_from, int axis_into);
matrix_t s21_create_rotations_camera(double axz, double ayz, double axy);

void s21_matrix_print(const matrix_t* mat, OutStream os);

#endif
