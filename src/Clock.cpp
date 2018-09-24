//
// Created by bittner on 9/24/18.
//

#include <iostream>
#include <cor/Clock.h>

namespace CoR {
    void Clock::clockStart()
    {
        StartTime = std::chrono::high_resolution_clock::now();
    }

    void Clock::clockMessageAtCurrentTime(const std::string & message) const
    {
        std::chrono::duration<double> dt = std::chrono::high_resolution_clock::now() - StartTime;
        std::cout << message << " [" << dt.count() << " s]" << std::endl;
    }
}