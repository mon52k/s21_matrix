#ifndef SRC_S21_HELPERS_H_
#define SRC_S21_HELPERS_H_

#include "s21_matrix.h"

int check_zero_negative(int rows, int columns);
int compare_rows_columns(matrix_t *A, matrix_t *B);
int compare_rows_columns_for_mult_transp(matrix_t *A, matrix_t *B);
void remove_rows_columns(matrix_t *A, matrix_t *cut_matrix, int i, int j);
int comparison(double A, double B);
double calc_determinant(matrix_t *A);
#endif