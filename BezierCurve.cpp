//
// Created by Philip on 5/11/2020.
//

#include "BezierCurve.h"

#include <cmath>
#include <iostream>
#include <utility>

#include "util/MathUtil.h"

BezierCurve::BezierCurve(std::vector<glm::vec2> controlPoints, double stepSize) :
                                                                degree(controlPoints.size()-1),
                                                                stepSize(stepSize > 0 ? stepSize : 0.1),
                                                                controlPoints(std::move(controlPoints)) {}

std::vector<glm::vec2> BezierCurve::bezierNaive() {
    std::vector<glm::vec2> curvePoints;
    double t = 0.0;
    while(t <= 1.0) {
        double x{0}, y{0};
        double subT = 1.0 - t;
        for (int k = 0; k <= degree; k++) {
            int subK = degree - k;
            float coeff = MathUtil::binomialCoeff(degree, k) * std::pow(subT, subK) * std::pow(t, k);
            // round to zero at threshold=0.001
            coeff = coeff > 0.001 ? coeff : 0;
            x += controlPoints[k].x * coeff;
            y += controlPoints[k].y * coeff;
        }

        curvePoints.emplace_back(x, y);
        t += stepSize;
    }

    return curvePoints;
}
