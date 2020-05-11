//
// Created by Philip on 5/11/2020.
//

#ifndef CELL_BEZIERCURVE_H
#define CELL_BEZIERCURVE_H

#include <glm/glm.hpp>
#include <vector>


class BezierCurve {
public:
    BezierCurve(std::vector<glm::vec2> controlPoints, double stepSize);

    // TODO: std::vector<glm::vec2> bezierDeCasteljau(); // generate geometrically
    // TODO: std::vector<glm::vec2> bezierMatrix();      // generate via matrix operations
    std::vector<glm::vec2> bezierNaive(); // generate via polynomial function
private:
    // degree of polynomial representing curve
    int degree;
    // step size of parameter t s.t. 0 < t < 1;
    double stepSize;
    // points that define the curve in 2d space
   std::vector<glm::vec2> controlPoints;
};


#endif //CELL_BEZIERCURVE_H
