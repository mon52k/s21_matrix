#include "s21_support.h"

int comparison(double A, double B) {
  int result = SUCCESS;
  char *temp_A = malloc(100 * sizeof(char));
  char *temp_B = malloc(100 * sizeof(char));
  sprintf(temp_A, "%.6lf", A);
  sprintf(temp_B, "%.6lf", B);
  if (strcmp(temp_A, temp_B)) {
    result = FAILURE;
  }
  free(temp_A);
  free(temp_B);
  return result;
}

void remove_rows_columns(matrix_t *A, matrix_t *cut_matrix, int i, int j) {
  int m = 0;
  int n = 0;
  for (int k = 0; k < A->rows; k++) {
    for (int l = 0; l < A->columns; l++) {
      if (k != i && l != j) {
        cut_matrix->matrix[m][n] = A->matrix[k][l];
        n++;
        if (n == cut_matrix->columns) {
          n = 0;
          m++;
        }
      }
    }
  }
}

double calc_determinant(matrix_t *A) {
  double result = 0;
  if (A->rows == 1) {
    return result = A->matrix[0][0];
  } else if (A->rows == 2) {
    return result = A->matrix[0][0] * A->matrix[1][1] -
                    A->matrix[0][1] * A->matrix[1][0];
  }
  matrix_t cut_matrix = {0};
  s21_create_matrix(A->rows - 1, A->columns - 1, &cut_matrix);
  for (int i = 0; i < A->rows; i++) {
    remove_rows_columns(A, &cut_matrix, 0, i);
    result +=
        (i % 2 == 0 ? 1 : -1) * A->matrix[0][i] * calc_determinant(&cut_matrix);
  }
  s21_remove_matrix(&cut_matrix);
  return result;
}

int check_zero_negative(int rows, int columns) {
  int result = SUCCESS;
  if (rows < 1 || columns < 1) {
    result = FAILURE;
  }
  return result;
}

int compare_rows_columns(matrix_t *A, matrix_t *B) {
  int result = SUCCESS;
  if (A->rows != B->rows || A->columns != B->columns) {
    result = FAILURE;
  }
  return result;
}

int compare_rows_columns_for_mult_transp(matrix_t *A, matrix_t *B) {
  int result = SUCCESS;
  if (A->rows != B->columns || A->columns != B->rows) {
    result = FAILURE;
  }
  return result;
}
