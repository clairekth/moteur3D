#include "Matrix.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    Matrix test0({{1, 2},
                  {2, 3}});

    Matrix test1({{1, 0, 2, -1},
                  {3, 0, 0, 5},
                  {2, 1, 4, -3},
                  {1, 0, 5, 0}});

    Matrix test2({{1, 2, 3, 5, 4, -7, 1},
                  {-4, 2, 5, -8, 8, 1, 2},
                  {1, 1, 2, 6, 5, 4, 3},
                  {2, 3, 5, 7, 8, 9, 10},
                  {2, 5, 1, 4, -4, 9, 6},
                  {3, 4, 6, 8, 2, 8, 5},
                  {8, 5, -5, -2, -1, 4, 7}});

    Matrix test3({{1, 2, 3},
                  {4, 5, 6},
                  {2, 4, 6}});

    std::cout << "Test déterminant" << std::endl;
    bool test0_determinant = test0.determinant() == -1;
    std::cout << "Test 0: " << (test0_determinant ? "True" : "False") << std::endl;
    bool test1_determinant = test1.determinant() == 30;
    std::cout << "Test 1: " << (test1_determinant ? "True" : "False") << std::endl;
    bool test2_determinant = test2.determinant() == 935472;
    std::cout << "Test 2: " << (test2_determinant ? "True" : "False") << std::endl;
    bool test3_determinant = test3.determinant() == 0;
    std::cout << "Test 3: " << (test3_determinant ? "True" : "False") << std::endl;

    std::cout << "Test inverse" << std::endl;
    // https://www.dcode.fr/inverse-matrice
    Matrix test0_inverse = test0.inverse();
    std::cout << "Test 0: \n"
              << test0_inverse.to_string() << std::endl;

    // Résultat bon

    Matrix test1_inverse = test1.inverse();
    std::cout << "Test 1: \n"
              << test1_inverse.to_string() << std::endl;
    // Résultat bon

    Matrix test2_inverse = test2.inverse();
    std::cout << "Test 2: \n"
              << test2_inverse.to_string() << std::endl;
    // Résultat bon

    std::cout << "Test transpose" << std::endl;
    Matrix test0_transpose = test0.transpose();
    std::cout << "Test 0: \n"
              << test0_transpose.to_string() << std::endl;
    // Résultat bon

    Matrix test1_transpose = test1.transpose();
    std::cout << "Test 1: \n"
              << test1_transpose.to_string() << std::endl;
    // Résultat bon

    Matrix test2_transpose = test2.transpose();
    std::cout << "Test 2: \n"
              << test2_transpose.to_string() << std::endl;
    // Résultat bon

    Matrix test3_transpose = test3.transpose();
    std::cout << "Test 3: \n"
              << test3_transpose.to_string() << std::endl;
    // Résultat bon

    return 0;
}
