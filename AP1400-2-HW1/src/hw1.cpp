#include "../include/hw1.h"
#include <cmath>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <type_traits>


namespace algebra {
    /* create a n x m matrix with all elements equal to zero. */
    Matrix zeros(size_t n, size_t m) {
        return Matrix(n, vector<double>(m, 0));
    }

    /* create a n x m matrix with all elements equal to one. */
    Matrix ones(size_t n, size_t m) {
        return Matrix(n, vector<double>(m, 1));
    }

    /* create a n x m matrix with all elements a random number between min and max. */
    Matrix random(size_t n, size_t m, double min, double max) {
        if (min > max) {
            throw logic_error("Minimun number cannot be greater than maximun value.");
        }
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dist(min, max);

        Matrix matrix{algebra::zeros(n, m)};
        for (vector<double> &row : matrix) {
            for (double &col : row)
            {
                col = dist(gen);
            }
        }
        return matrix;
    }

    /* display the matrix. each element of the matrix have exactly 3 decimal places. */
    void show(const Matrix& matrix) {
        for (vector<double> row : matrix) {
            for (double &col : row) {
                cout << setprecision(3) << col << endl;
            }
        }
    }

    /* multiplies the matrix into the constant scalar c */
    Matrix multiply(const Matrix& matrix, double c) {
        if (matrix.empty()) {
            return Matrix{};
        }

        int n = matrix.size();
        int m = matrix[0].size();
        Matrix result = zeros(n, m);

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result[i][j] = matrix[i][j] * c;
            }
        }
        return result;
    }

    /* multiplies the matrix1 into matrix2. */
    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
        if (matrix1.empty() && matrix2.empty()) {
            return matrix1;
        } else if (matrix1.empty() || matrix2.empty()) {
            throw logic_error("One of the matrix is empty");
        }

        int rowSize1 = matrix1.size();
        int rowSize2 = matrix2.size();
        int colSize1 = matrix1[0].size();
        int colSize2 = matrix2[0].size();
        if (colSize1 != rowSize2) {
            throw logic_error("Inproper matrix size.");
        }
        Matrix result{zeros(rowSize1, colSize2)};

        for (int i = 0; i < rowSize1; i++) {
            for (int j = 0; j < colSize2; j++) {
                double value = 0;
                for (int k = 0; k < colSize1; k++) {
                    result[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
        return result;
    }

    /* adds the constant number c to every element of matrix. */
    Matrix sum(const Matrix& matrix, double c) {
        if (matrix.empty()) {
            return matrix;
        }
        
        size_t n = matrix.size();
        size_t m = matrix[0].size();
        Matrix result{zeros(n, m)};

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result[i][j] = matrix[i][j] + c;
            }
        }
        return result;
    }

    /* adds 2 matrices to each other. */
    Matrix sum(const Matrix& matrix1, const Matrix& matrix2) {
        if (matrix1.empty() && matrix2.empty()) {
            return matrix1;
        } else if (matrix1.empty() || matrix2.empty()) {
            throw logic_error("Two matrice not of equal size.");
        }
        
        size_t n1 = matrix1.size();
        size_t m1 = matrix1[0].size();
        size_t n2 = matrix2.size();
        size_t m2 = matrix2[0].size();
        if (n1 != n2 && m1 != m2) {
            throw logic_error("Tow matrices not of equal size");
        }

        Matrix result{zeros(n1, m1)};

        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < m1; j++) {
                result[i][j] = matrix1[i][j] + matrix2[i][j];
            }
        }
        return result;
    }

    /* generate the transpose matrix of the input matrix. */
    Matrix transpose(const Matrix& matrix) {
        if (matrix.empty()) {
            return Matrix{};
        }
        
        size_t n = matrix.size();
        size_t m = matrix[0].size();
        Matrix result = zeros(m, n);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result[i][j] = matrix[j][i];
            }
        }
        return result;
    }

    /* create the minor of the input matrix with respect to nth row and mth column. */
    Matrix minor(const Matrix& matrix, size_t n, size_t m) {
        if (matrix.empty()) {
            throw logic_error("The matrix is empty.");
        }

        size_t rowSize = matrix.size();
        size_t colSize = matrix[0].size();
        if (n >= rowSize || m >= colSize) {
            throw logic_error("Error in n or m");
        }

        Matrix result{zeros(rowSize - 1, colSize - 1)};
        int onRight = 0;
        int onDown  = 0;
        for (int i = 0; i < rowSize - 1; i++) {
            if (i == n) {
                onDown = 1;
            }
            for (int j = 0; j < colSize - 1; j++) {
                if (j == m){
                    onRight = 1;
                }
                result[i][j] = matrix[i + onDown][j + onRight];
            }
            onRight = 0;
        }
        return result;
    }

    /* calculates the determinant of the input matrix. */
    double determinant(const Matrix& matrix) {
        if (matrix.empty()) {
            return 1;
        }

        size_t rowSize = matrix.size();
        size_t colSize = matrix[0].size();
        if (rowSize != colSize) {
            throw logic_error("Not a n x n matrix");
        } else if (rowSize == 1) {
            return matrix[0][0];
        } else if (rowSize == 2) {
            return matrix[0][0] * matrix [1][1] - matrix[0][1] * matrix[1][0];
        }

        double result = 0;
        for (int i = 0; i < rowSize; i++) {
            result += pow(-1, i) * matrix[i][0] * determinant(minor(matrix, i, 0));
        }
        return result;
    }

    /* generates the matrix's inverse. */
    Matrix inverse(const Matrix& matrix) {
        if (matrix.empty()) {
            return matrix;
        }
        
        double det = determinant(matrix);
        if (det == 0) {
            throw logic_error("Its inverse does not exist.");
        }

        size_t rowSize = matrix.size();
        size_t colSize = matrix[0].size();
        Matrix result = zeros(rowSize, colSize);
        
        for (int i = 0; i < rowSize; i++) {
            for (int j =0; j < colSize; j++) {
                result[i][j] = pow(-1, i + j) * determinant(minor(matrix, j, i)) / det;
            }
        }
        return result;
    }

    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis) {
        if (matrix1.empty() && matrix2.empty()) {
            return matrix1;
        } else if (matrix1.empty() || matrix2.empty()) {
            throw logic_error("One of the matrix is empty.");
        }

        size_t rowSize1 = matrix1.size();
        size_t colSize1 = matrix1[0].size();
        size_t rowSize2 = matrix2.size();
        size_t colSize2 = matrix2[0].size();
        if (axis == 0) {
            if (colSize1 != colSize2) {
                throw logic_error("Colum size not equal.");
            }

            Matrix result = zeros(rowSize1 + rowSize2, colSize1);
            for (int i = 0; i < rowSize1; i++) {
                for (int j = 0; j < colSize1; j++) {
                    result[i][j] = matrix1[i][j];
                }
            }

            for (int i = 0; i < rowSize2; i++) {
                for (int j = 0 ; j < colSize2; j++) {
                    result[i + rowSize1][j] = matrix2[i][j];
                }
            }
            return result;
        } else {
            if (rowSize1 != rowSize2) {
                throw logic_error("Row size not equal.");
            }

            Matrix result = zeros(rowSize1, colSize1 + colSize2);
            for (int i = 0; i < rowSize1; i++) {
                for (int j = 0; j < colSize1; j++) {
                    result[i][j] = matrix1[i][j];
                }
            }

            for (int i = 0; i < rowSize2; i++) {
                for (int j = 0; j < rowSize2 ; j++) {
                    result[i][j + colSize1] = matrix2[i][j];
                }
            }
            return result;
        }
    }

    /* swapping two rows. */
    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2) {
        if (matrix.empty()) {
            throw std::logic_error("Empty matrix.");
        }

        size_t rowSize = matrix.size();
        size_t colSize = matrix[0].size();
        if (r1 >= rowSize || r2 >= rowSize || r1 < 0 || r2 < 0) {
            throw std::logic_error("Excel row size.");
        } else if (r1 == r2) {
            return matrix;
        }

        Matrix result = zeros(rowSize, colSize);
        for (int i = 0; i < rowSize; i++) {
            if (i == r1 || i == r2) {
                continue;
            }
            for (int j = 0; j < colSize; j++) {
                result[i][j] = matrix[i][j];
            }
        }

        for (int j = 0; j < colSize; j++) {
        
            result[r1][j] = matrix[r2][j];
            result[r2][j] = matrix[r1][j];
        }
        return result;
    }

    Matrix ero_multiply(const Matrix& matrix, size_t r, double c) {
        if (matrix.empty()) {
            throw std::logic_error("Empty matrix.");
        }

        size_t rowSize = matrix.size();
        size_t colSize = matrix[0].size();
        if (r >= rowSize || r < 0) {
            throw std::logic_error("Excel row size.");
        } else if (c == 1) {
            return matrix;
        }

        Matrix result = zeros(rowSize, colSize);
        for (int i = 0; i < rowSize; i++) {
            if (i == r) {
                continue;
            }
            for (int j = 0; j < colSize; j++) {
                result[i][j] = matrix[i][j];
            }
        }

        for (int j = 0; j < colSize; j++) {
        
            result[r][j] = matrix[r][j] * c;
        }
        return result;
    }

    /* adds r1th x c   into r2th row. */
    Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {
        if (matrix.empty()) {
            throw std::logic_error("Empty matrix.");
        }

        size_t rowSize = matrix.size();
        size_t colSize = matrix[0].size();
        if (r1 >= rowSize || r2 >= rowSize || r1 < 0 || r2 < 0) {
            throw std::logic_error("Excel row size.");
        } else if (c == 1 && r1 == r2) {
            return matrix;
        }

        Matrix result = zeros(rowSize, colSize);
        for (int i = 0; i < rowSize; i++) {
            if (i == r2) {
                continue;
            }
            for (int j = 0; j < colSize; j++) {
                result[i][j] = matrix[i][j];
            }
        }

        for (int j = 0; j < colSize; j++) {
        
            result[r2][j] = c * matrix[r1][j]  + matrix[r2][j];
        }
        return result;
    }

    Matrix upper_triangular(const Matrix& matrix) {
        if (matrix.empty()) {
            return matrix;
        }

        size_t rowSize = matrix.size();
        size_t colSize = matrix[0].size();
        if (rowSize != colSize) {
            throw std::logic_error("Row and column not of equal size.");
        }

        Matrix result = zeros(rowSize, colSize);
        for (int i = 0; i < rowSize; i++) {
            for (int j = 0; j < colSize; j++) {
                result[i][j] = matrix[i][j];
            }
        }

        for (int i = 0; i < rowSize - 1; i++) {
            if (matrix[i][i] == 0) {
                result = ero_swap(result, i, i + 1);
            }

            for (int j = i + 1; j < rowSize; j++) {
                result = ero_sum(result, i, - result[j][i] / result[i][i], j);
            }
        }
        return result;
    }
}





