//
// Created by Philip on 5/11/2020.
//

#ifndef CELL_MATHUTIL_H
#define CELL_MATHUTIL_H

#include <vector>

class MathUtil {
public:
    static int binomialCoeff(int n, int k);
    static int* binomialMatrix(int n, int k);
private:
    static std::vector<int*> generatedBinomialCoeffs;
    static int maxGeneratedDegree;
};


#endif //CELL_MATHUTIL_H
