#include "s21_matrix.h"

#include <math.h>
#include "../util/prettify_c.h"

matrix_t s21_create_unit_matrix() {
    matrix_t result;
    assert_m(s21_create_matrix(4, 4, &result) is OK);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.matrix[i][j] = i == j ? 1 : 0;

    return result;
}

matrix_t s21_create_shift_matrix(double dx, double dy, double dz) {
    matrix_t result = s21_create_unit_matrix();

    result.matrix[0][3] = dx;
    result.matrix[1][3] = dy;
    result.matrix[2][3] = dz;
    result.matrix[3][3] = 1.0;

    return result;
}

matrix_t s21_create_scale_matrix(double sx, double sy, double sz) {
    matrix_t result = s21_create_unit_matrix();

    result.matrix[0][0] = sx;
    result.matrix[1][1] = sy;
    result.matrix[2][2] = sz;
    result.matrix[3][3] = 1.0;

    return result;
}


matrix_t s21_create_scaleshift_matrix(double sx, double sy, double sz, double dx, double dy, double dz) {
    matrix_t result = s21_create_unit_matrix();

    result.matrix[0][0] = sx;
    result.matrix[1][1] = sy;
    result.matrix[2][2] = sz;
    result.matrix[0][3] = dx;
    result.matrix[1][3] = dy;
    result.matrix[2][3] = dz;
    result.matrix[3][3] = 1.0;

    return result;
}


matrix_t s21_create_projection_matrix(double size, double near, double far, double aspect_ratio) {
    matrix_t result = s21_create_unit_matrix();

    result.matrix[0][0] = 1.0 / size;
    result.matrix[1][1] = 1.0 / size * aspect_ratio;
    result.matrix[2][2] = 2.0 / (far - near);
    result.matrix[3][3] = 1.0;

    result.matrix[2][3] = (far + near) / (far - near);
    // result.matrix[3][2] = 0.000;

    return result;
}

matrix_t s21_create_perspective_matrix(double fov, double near, double far, double aspect_ratio) {
    matrix_t result = s21_create_unit_matrix();

    result.matrix[0][0] = 1.0 / (aspect_ratio * tan(fov / 2.0));
    result.matrix[1][1] = 1.0 / tan(fov / 2.0);
    result.matrix[2][2] = (far + near) / (far - near);
    result.matrix[3][3] = 0.0;

    result.matrix[2][3] = 2.0 * (far * near) / (far - near);
    result.matrix[3][2] = -1.0;

    return result;
}

FloatArray16 s21_matrix_to_farray(const matrix_t* m) {
    assert_m(m->columns is 4);
    assert_m(m->rows is 4);
    
    FloatArray16 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.data[row * 4 + col] = m->matrix[row][col];
        }
    }

    return result;
}

matrix_t s21_create_rotation_matrix(double angle, int axis_from, int axis_into) {
    matrix_t result = s21_create_unit_matrix();

    result.matrix[axis_from][axis_from] = cos(angle);
    result.matrix[axis_into][axis_into] = cos(angle);
    result.matrix[axis_from][axis_into] = -sin(angle);
    result.matrix[axis_into][axis_from] = sin(angle);

    return result;
}

matrix_t s21_create_view_to_camera() {
    matrix_t result;
    assert_m(s21_create_matrix(4, 4, &result) is OK);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result.matrix[i][j] = 0;

    result.matrix[0][0] = 1;
    result.matrix[1][2] = 1;
    result.matrix[2][1] = -1;
    result.matrix[3][3] = 1;

    return result;
}

matrix_t s21_create_rotations_camera(double axz, double ayz, double axy) {
    matrix_t xz = s21_create_rotation_matrix(axz, AXIS_X, AXIS_Z);
    matrix_t yz = s21_create_rotation_matrix(ayz, AXIS_Y, AXIS_Z);
    matrix_t xy = s21_create_rotation_matrix(axy, AXIS_X, AXIS_Y);

    matrix_t a;
    matrix_t b;
    assert_m(s21_mult_matrix(&xz, &yz, &a) is OK);
    assert_m(s21_mult_matrix(&a, &xy, &b) is OK);

    s21_remove_matrix(&xz);
    s21_remove_matrix(&yz);
    s21_remove_matrix(&xy);
    s21_remove_matrix(&a);

    return b;
}

void s21_matrix_print(const matrix_t* mat, OutStream os){
  x_sprintf(os, "Mat %dx%d: [\n", mat->rows, mat->columns);
  for (int row = 0; row < mat->rows; row++) {
    for (int col = 0; col < mat->columns; col++)
      x_sprintf(os, "%.5lf, ", mat->matrix[row][col]);
    x_sprintf(os, "\n");
  }
  x_sprintf(os, "]");
}