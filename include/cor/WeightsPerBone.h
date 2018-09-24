//
// Created by bittner on 9/24/18.
//

#ifndef CORCALCULATOR_WEIGHTSPERBONE_H
#define CORCALCULATOR_WEIGHTSPERBONE_H

#include <vector>
#include <cmath>

namespace CoR {
    struct WeightsPerBone;

    float skinningWeightsDistance(const WeightsPerBone &wp, const WeightsPerBone &wv);
    float similarity(const WeightsPerBone &wp, const WeightsPerBone &wv, float sigma);

    struct WeightsPerBone {
        std::vector<float> weights;

        WeightsPerBone() = default;
        explicit WeightsPerBone(unsigned long n) {
            weights = std::vector<float>(n, 0);
        }

        unsigned long size() const {
            return weights.size();
        }

        float & operator[](int idx) {
            return weights[idx];
        }

        float operator[](int idx) const {
            return weights[idx];
        }

        WeightsPerBone operator + (const WeightsPerBone &other) const {
            WeightsPerBone sum(size());
            for (int i = 0; i < size(); ++i)
                sum.weights[i] = weights[i] + other.weights[i];
            return sum;
        }

        WeightsPerBone operator - (const WeightsPerBone &other) const {
            WeightsPerBone dif(size());
            for (int i = 0; i < size(); ++i)
                dif.weights[i] = weights[i] - other.weights[i];
            return dif;
        }

        WeightsPerBone operator * (const float scalar) const {
            WeightsPerBone ret(size());
            for (int i = 0; i < size(); ++i)
                ret.weights[i] = weights[i] * scalar;
            return ret;
        }

        float norm() const {
            float norm = 0;
            for (int i = 0; i < size(); ++i)
                norm += weights[i] * weights[i];
            return std::sqrt(norm);
        }
    };
}

#endif //CORCALCULATOR_WEIGHTSPERBONE_H
