#include <iostream>

#include "tester/tester.h"

int main() {
    Matrix<int> matrix(5,6);
    matrix.set(0,1,1);
    matrix.set(0,3,2);
    matrix.set(0,5,3);
    matrix.set(1,2,4);
    matrix.set(1,4,5);
    matrix.set(2,1,1);
    matrix.set(2,3,2);
    matrix.set(3,3,3);
    matrix.set(3,5,4);
    matrix.set(4,3,5);
    matrix.set(4,1,1);
    matrix.set(4,5,2);

    Matrix<int> matrix2(6,4);
    matrix2.set(0,1,1);
    matrix2.set(1,3,2);
    matrix2.set(2,3,3);
    matrix2.set(3,2,4);
    matrix2.set(4,0,5);
    matrix2.set(4,1,1);
    matrix2.set(5,2,2);
    Matrix<int> matrix3 = (matrix*matrix2);
    Tester::execute();
    return EXIT_SUCCESS;
}

