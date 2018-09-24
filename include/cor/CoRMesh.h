//
// Created by bittner on 9/24/18.
//

#ifndef CORCALCULATOR_CORMESH_H
#define CORCALCULATOR_CORMESH_H

#include "CoRTriangle.h"

namespace CoR {
    struct CoRMesh {
        std::vector<glm::vec3> vertices;
        std::vector<CoRTriangle> triangles;
        std::vector<WeightsPerBone> weights;

        // triangle adjacency graph
        std::vector<std::vector<CoRTriangle*>> trianglesOfVertex;
        std::vector<std::vector<int>> verticesWithSimilarSkinningWeights;

        CoRMesh(
                const std::vector<glm::vec3> & vertices,
                const std::vector<CoRTriangle> & triangles,
                const std::vector<WeightsPerBone> & weights)
                : vertices(vertices),
                  triangles(triangles),
                  weights(weights)
        {

        }
    };
}

#endif //CORCALCULATOR_CORMESH_H
