#include "SimplerTimerCluster.h"

SimplerTimerCluster::SimplerTimerCluster(int val) {
    id = val;
    start = std::chrono::high_resolution_clock::now();
}

SimplerTimerCluster::~SimplerTimerCluster(){
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    float result = duration.count();
    //std::cout << "id = " << id << " Duration " <<  result << std::endl; 
}

double SimplerTimerCluster::getDuration(){
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    float result = duration.count();
    return result;
}


