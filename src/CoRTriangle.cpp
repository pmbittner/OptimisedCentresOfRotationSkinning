//
// Created by bittner on 9/24/18.
//

#include <cor/CoRTriangle.h>

namespace CoR {
    int &CoRTriangle::operator[](int idx) {
        assert(idx < 3);
        return *(&alpha + idx);
    }

    bool CoRTriangle::tryAddNeighbour(CoRTriangle &triangle) {
        for (CoRTriangle *neighbour : _neighbours)
            if (neighbour == &triangle)
                return false;

        int equalVertices = 0;
        // two vertices have to be equal
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (operator[](i) == triangle[j]) {
                    if (++equalVertices == 2) {
                        _neighbours.push_back(&triangle);
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool CoRTriangle::tryAddNeighbour(CoRTriangle &triangle, int equalVertex) {
        for (CoRTriangle *neighbour : _neighbours)
            if (neighbour == &triangle)
                return false;

        // two vertices have to be equal
        for (int i = 0; i < 3; ++i) {
            if (operator[](i) == equalVertex) continue;
            for (int j = 0; j < 3; ++j) {
                if (triangle[j] == equalVertex) continue;
                if (operator[](i) == triangle[j]) {
                    _neighbours.push_back(&triangle);
                    return true;
                }
            }
        }
        return false;
    }
}