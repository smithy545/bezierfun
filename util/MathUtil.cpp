//
// Created by Philip on 5/11/2020.
//

#include "MathUtil.h"

std::vector<int*> MathUtil::generatedBinomialCoeffs;
int MathUtil::maxGeneratedDegree = 0;

int MathUtil::binomialCoeff(int n, int k) {
    while(n >= maxGeneratedDegree) {
        int s = maxGeneratedDegree;
        int* coeffs = new int[s + 1];
        coeffs[0] = 1;
        for(int i = 1, prev = s - 1; i < s; i++)
            coeffs[i] = generatedBinomialCoeffs[prev][i-1] + generatedBinomialCoeffs[prev][i];
        coeffs[s] = 1;
        generatedBinomialCoeffs.push_back(coeffs);
        maxGeneratedDegree++;
    }

    return generatedBinomialCoeffs[n][k];
}

int *MathUtil::binomialMatrix(int n, int k) {
    return nullptr;
}
