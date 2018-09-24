//
// Created by bittner on 9/24/18.
//

#ifndef CORCALCULATOR_CORTRIANGLE_H
#define CORCALCULATOR_CORTRIANGLE_H

#include <glm/vec3.hpp>
#include "WeightsPerBone.h"

namespace CoR {
    struct CoRTriangle {
        int alpha, beta, gamma;

        glm::vec3 center;
        WeightsPerBone averageWeight;
        float area = 0;

        std::vector<CoRTriangle*> _neighbours;

        explicit CoRTriangle(int bones = 0) : alpha(0), beta(0), gamma(0), center(glm::vec3(0)), averageWeight(WeightsPerBone(bones)), area(0), _neighbours(std::vector<CoRTriangle*>()) {
            _neighbours.reserve(3);
        }

        int & operator[](int idx);
        bool tryAddNeighbour(CoRTriangle &triangle);
        bool tryAddNeighbour(CoRTriangle &triangle, int equalVertex);
    };
}

#endif //CORCALCULATOR_CORTRIANGLE_H
