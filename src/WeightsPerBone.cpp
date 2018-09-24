//
// Created by bittner on 9/24/18.
//

#include <cor/WeightsPerBone.h>

namespace CoR {
    float skinningWeightsDistance(const WeightsPerBone & wp, const WeightsPerBone & wv)
    {
        return (wp - wv).norm();
    }

    float similarity(const WeightsPerBone &wp, const WeightsPerBone &wv, float sigma)
    {
        float sigmaSquared = sigma * sigma;
        float sim = 0;
        for (int j = 0; j < wp.size(); ++j) {
            for (int k = 0; k < wv.size(); ++k) {
                if (j != k) {
                    float exponent = wp[j] * wv[k] - wp[k] * wv[j];
                    exponent *= exponent;
                    exponent /= sigmaSquared;

                    sim += wp[j] * wp[k] * wv[j] * wv[k] * std::exp(-exponent);
                }
            }
        }
        return sim;
    }
}