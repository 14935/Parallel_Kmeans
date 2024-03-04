#pragma once

#include <iostream>
#include <chrono>

class SimplerTimerCluster {
    private:
        std::chrono::time_point<std::chrono::_V2::system_clock> start, end;
        std::chrono::duration<float> duration;
    
    public:
        int id;
        SimplerTimerCluster (int val);
        ~SimplerTimerCluster ();
        double getDuration();
};