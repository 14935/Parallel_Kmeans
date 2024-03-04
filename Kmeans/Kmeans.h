#pragma once

#include <fstream>
#include <sstream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include "Point.h"
#include "../Distance/Distance.h"


struct Cluster{
    std::vector<double>* center;
    std::set<int>* neigbours;
    int nNeigbours;
    int id;

    Cluster() {
        center = new std::vector<double>;
        neigbours = new std::set<int>;
        nNeigbours = 0;
    }

    ~Cluster() {
        delete center;
        delete neigbours;
    }
};


class Kmeans {
    private:

        inline static int counter = 0;
        const int id;
        
        std::map<int, Point*>* dataArray; // all data
        std::map<int, Cluster*>* currentCluster;

        std::vector<std::string>* featureNames;
        int numThreads;
        int featureVectorSize;
        int k;
        int n;
        int flag;
        int done;

        std::mutex  mutex_flag;
        std::mutex  mutex_done;
        std::mutex  mutex_update;
        std::condition_variable conditional_flag;

        void readCsv(std::string intputPath, std::string type, int kClusters);
        void writeCsv(std::string outputPath, std::string type);
        void zeroInitCluster(std::string type);
        void makeZeroCluster();
        void setDataArray(std::map<int, Point*>* dataArray);
        void setNumThreads(int numThreads);
        void setFeatureVectorSize(int val);
        void setFeatureNames(std::vector<std::string>* names);
        void updatePoints();

        void KmeansPlusPlus();
        void fitCenters(std::map<int, Cluster*>* bufferCluster, int start, int finish);
        void fitIterCenters(std::map<int, Cluster*>* bufferCluster, int start, int finish, int iter);
        void fitThreads(int iteration);
        void reg(std::vector<std::map<int, Cluster*>*> bufferClusters, int iteration);
        
        int getK();  
        int getN();
        int getFeatureVectorSize();

        std::vector<std::string>* getFeatureNames();


    public:
        Kmeans(int kClusters, int numThread);
        ~Kmeans();

        std::map<int, Point*>* getDataArray();
        std::map<int, Cluster*>* getCurrentCluster();

        void readDataCsv(std::string intputPath);
        void writeDataCsv(std::string outputPath);
        void writeCentersCsv(std::string outputPath);

        void fit(int iteration);
        int predict(std::vector<double>* point);
};

