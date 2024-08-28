#include "s21_matrix.h"

#include "s21_support.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  enum error status = OK;
  if (!check_zero_negative(rows, columns) || result == NULL) {
    return status = ERROR_INCORRECT_MATRIX;
  }
  result->columns = columns;
  result->rows = rows;
  result->matrix = malloc(result->rows * sizeof(double *));
  for (int i = 0; i < result->rows; i++) {
    result->matrix[i] = malloc(result->columns * sizeof(double));
  }
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++) {
      result->matrix[i][j] = 0;
    }
  }
  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (A->matrix) {
    if (check_zero_negative(A->rows, A->columns)) {
      for (int i = 0; i < A->rows; i++) {
        free(A->matrix[i]);
      }
      free(A->matrix);
      A->rows = 0;
      A->columns = 0;
    }
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int result = SUCCESS;
  if (A == NULL || B == NULL) {
    if (A == NULL && B == NULL) {
      result = SUCCESS;
    } else {
      result = FAILURE;
    }
  }
  if (result == SUCCESS && (A->matrix == NULL || B->matrix == NULL)) {
    result = FAILURE;
  }
  if (result == SUCCESS && (A->columns != B->columns || A->rows != B->rows)) {
    result = FAILURE;
  }
  if (result == SUCCESS) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (!comparison(A->matrix[i][j], B->matrix[i][j])) {
          result = FAILURE;
          break;
        }
      }
      if (result == FAILURE) break;
    }
  }
  return result;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  enum error status = OK;
  if (A == NULL || B == NULL || result == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && (A->matrix == NULL || B->matrix == NULL)) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && !compare_rows_columns(A, B)) {
    status = CALCULATION_ERROR;
  }
  if (status == OK) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  }
  return status;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  enum error status = OK;
  if (A == NULL || B == NULL || result == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && (A->matrix == NULL || B->matrix == NULL)) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && (!check_zero_negative(A->rows, A->columns) ||
                       !check_zero_negative(B->rows, B->columns))) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && !compare_rows_columns(A, B)) {
    status = CALCULATION_ERROR;
  }
  if (status == OK) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  }
  return status;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  enum error status = OK;
  if (A == NULL || result == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && A->matrix == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && !check_zero_negative(A->rows, A->columns)) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && isnan(number)) {
    status = CALCULATION_ERROR;
  }
  if (status == OK) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] * number;
      }
    }
  }
  return status;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  enum error status = OK;
  if (A == NULL || B == NULL || result == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (A->matrix == NULL || B->matrix == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (!check_zero_negative(A->rows, A->columns) ||
      !check_zero_negative(B->rows, B->columns)) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (!compare_rows_columns_for_mult_transp(A, B) && status == OK) {
    status = CALCULATION_ERROR;
  }
  if (status == OK) {
    s21_create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = 0;
        for (int k = 0; k < A->columns; k++)
          result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
  return status;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  enum error status = OK;
  if (A == NULL || result == NULL || A->matrix == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && !check_zero_negative(A->rows, A->columns)) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK) {
    s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++)
        result->matrix[j][i] = A->matrix[i][j];
    }
  }
  return status;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  enum error status = OK;
  if (A == NULL || result == NULL) {
    return status = ERROR_INCORRECT_MATRIX;
  }
  if (A->matrix == NULL) {
    return status = ERROR_INCORRECT_MATRIX;
  }
  if (!check_zero_negative(A->rows, A->columns)) {
    return status = ERROR_INCORRECT_MATRIX;
  }
  if (A->rows != A->columns) {
    return status = CALCULATION_ERROR;
  }
  s21_create_matrix(A->rows, A->columns, result);
  if (A->rows == 1) {
    result->matrix[0][0] = A->matrix[0][0];
    return status;
  }
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      matrix_t minor = {0};
      s21_create_matrix(A->rows - 1, A->columns - 1, &minor);
      remove_rows_columns(A, &minor, i, j);
      double det = 0;
      s21_determinant(&minor, &det);
      s21_remove_matrix(&minor);
      if (det > 1e-6 || det * -1 > 1e-6) {
        result->matrix[i][j] = ((i + j) % 2 == 0) ? det : -det;
      } else {
        result->matrix[i][j] = 0;
      }
    }
  }
  return status;
}

int s21_determinant(matrix_t *A, double *result) {
  enum error status = OK;
  if (A == NULL || result == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && A->matrix == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && !check_zero_negative(A->rows, A->columns)) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (status == OK && A->rows != A->columns) {
    status = CALCULATION_ERROR;
  }
  if (status == OK) {
    *result = calc_determinant(A);
  }
  return status;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  enum error status = OK;
  if (A == NULL || result == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (A->matrix == NULL) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (A->rows != A->columns) {
    status = CALCULATION_ERROR;
  }
  if (!check_zero_negative(A->rows, A->columns)) {
    status = ERROR_INCORRECT_MATRIX;
  }
  if (A->rows == 1) {
    s21_create_matrix(A->rows, A->columns, result);
    if (A->matrix[0][0] != 0) {
      result->matrix[0][0] = 1 / A->matrix[0][0];
    } else {
      status = CALCULATION_ERROR;
    }
  }
  if (A->rows > 1) {
    double check = 0;
    s21_determinant(A, &check);
    if (check == 0) {
      status = CALCULATION_ERROR;
    } else {
      s21_create_matrix(A->rows, A->columns, result);
      matrix_t temp_matrix_complements = {0};
      matrix_t temp_matrix_transpose = {0};
      s21_create_matrix(A->rows, A->columns, &temp_matrix_complements);
      s21_create_matrix(A->rows, A->columns, &temp_matrix_transpose);
      s21_calc_complements(A, &temp_matrix_complements);
      s21_transpose(&temp_matrix_complements, &temp_matrix_transpose);
      s21_mult_number(&temp_matrix_transpose, -1, result);
      s21_remove_matrix(&temp_matrix_complements);
      s21_remove_matrix(&temp_matrix_transpose);
    }
  }
  return status;
}