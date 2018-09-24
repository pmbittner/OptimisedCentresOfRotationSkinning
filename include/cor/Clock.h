//
// Created by bittner on 9/24/18.
//

#ifndef CORCALCULATOR_CLOCK_H
#define CORCALCULATOR_CLOCK_H

#include <chrono>
#include <string>

namespace CoR {
    struct Clock {
        std::chrono::high_resolution_clock::time_point StartTime;
        void clockStart();
        void clockMessageAtCurrentTime(const std::string & message = "") const;
    };
}

#endif //CORCALCULATOR_CLOCK_H
