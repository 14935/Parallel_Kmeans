#include "SimplerTimerCluster/SimplerTimerCluster.h"
#include "Metrics/Metrics.h"

std::string inputDataPath = "./generator_data/input/input_data.csv";   
std::string outputDataPath = "./generator_data/output/output_data.csv"; 
std::string outputCentersPath = "./generator_data/output/output_centers.csv"; 

int main() {

  
    int k = 4;
    int numThreads = 4; 
    int iteration = 100;

    std::cin >> k >> numThreads >> iteration;
       
    Kmeans model(k, numThreads);
    model.readDataCsv(inputDataPath);
    SimplerTimerCluster s(1);
    model.fit(iteration);
    std::cout << " time = "  << s.getDuration() << std::endl;
    model.writeDataCsv(outputDataPath);
    model.writeCentersCsv(outputCentersPath);  
    
    
    return 0;
}