#include "Kmeans.h"


Kmeans::Kmeans(int kClusters, int numThread) : id(++counter) { 
    k = kClusters;
    numThreads = numThread;
    dataArray = new std::map<int, Point*>;
    currentCluster = new std::map<int, Cluster*>;
    featureNames = new std::vector<std::string>;
}


Kmeans::~Kmeans() {

    done = 0;
    flag = 0;
    for (int i = 0; i != dataArray->size(); ++i) {
        delete (*dataArray)[i];
    }
    delete dataArray;

    for (auto iter = currentCluster->begin(); iter != currentCluster->end(); ++iter) {
        delete iter->second;
    }
    delete currentCluster;

    delete featureNames;
}


//private
void Kmeans::readCsv(std::string path, std::string type, int kClusters) {

    this->k = kClusters;
    std::ifstream file;
    file.open(path);

    if (file.is_open()) {
       
        std::string line;
        std::getline(file, line);
        if (featureNames->size() == 0) {
            std::string name;
            std::stringstream str_stream(line);
            while (std::getline(str_stream, name, ',')){
                featureNames->push_back(name);
            }
        }

        int idCluster = 0;
        int idPoint = 0;
        
        while (std::getline(file, line)) {
            if (type == "first"){
                n += 1;
                
                Point *point = new Point;
                std::stringstream str_stream(line);
                std::string cell;
                while (std::getline(str_stream, cell, ',')){
                    double value = std::stod(cell);
                    point->appendToFeatureVector(value);
                }

                point->setLabel(0);
                point->getFeatureVector()->pop_back();
                point->setMinDistance(0.0);
                point->setId(idPoint);
                featureVectorSize = point->getFeatureVectorSize();

                dataArray->insert(std::make_pair(idPoint, point));
                idPoint += 1;
            }
        }
        if (type == "first") {
            std::cout << "Файл " << path << " успешно прочитан, size = " << dataArray->size() << std::endl;
        } else {
            std::cout << "Error: type != data and type != centers" << std::endl; 
        }

        file.close();
    } else {
        std::cout << "Ошибка чтения из файла" << std::endl;
    }
    this->zeroInitCluster("current");
    currentCluster->at(0)->nNeigbours = n;
}   


void Kmeans::writeCsv(std::string output_path, std::string type) {

    std::ofstream file;
    file.open(output_path);
    if (file.is_open()) {

        for (auto item : *featureNames) {
            file << item << ",";
        }
        file << "\n";


        if (type == "centers") {
            for (auto item : *currentCluster) {
                for (auto value : *item.second->center) {
                    file << value << ",";
                }
                file << item.first << "\n";
            } 
        } 

        if (type == "data") {
            for (auto item : *dataArray) {
                for (auto value : *item.second->getFeatureVector()) {
                    file << value << ",";
                }
                file << item.second->getLabel() << "\n";
            } 
        }

        std::cout << "Файл " << output_path << " успешно записан" << std::endl;
        file.close();
    } else {
        std::cout << "Ошибка записи из файла" << std::endl;
    }
}


void Kmeans::zeroInitCluster(std::string type) {
    for (int i = 0; i != k; i++) {
        Cluster* cluster = new Cluster;
        for (int j = 0; j != featureVectorSize; j++) {
            cluster->center->push_back(0.0);
        } 
        cluster->id = i;
        cluster->nNeigbours = 0;
        currentCluster->insert(std::make_pair(i, cluster));
    }
}


void Kmeans::makeZeroCluster() {
    for (auto c : *currentCluster) {
        for (auto v : *c.second->center) {
            v = 0.0;
        }
        c.second->nNeigbours = 0;
    } 
}


void Kmeans::setDataArray(std::map<int, Point*>* dataArray) {
    this->dataArray = dataArray;    
}


void Kmeans::setNumThreads(int numThreads) {
    this->numThreads = numThreads;
}


void Kmeans::setFeatureVectorSize(int featureVectorSize) {
    this->featureVectorSize = featureVectorSize;
}


void Kmeans::setFeatureNames(std::vector<std::string>* featureNames) {
    for (int i = 0; i != featureVectorSize + 1; i++) {
        this->featureNames->push_back(featureNames->at(i));
    }
}


void Kmeans::updatePoints() {

    double minDistance = std::numeric_limits<double>::max();
    double currentDistance = 0;
    int minDistanceClusterId = 0; 

    for (int i = 0; i != k; i++ ) {
        currentCluster->at(minDistanceClusterId)->nNeigbours += 0;
    }

    for (int i = 0; i != dataArray->size(); ++i) {
        
        minDistance = std::numeric_limits<double>::max();
        minDistanceClusterId = 0; 
       
        for (auto iter = currentCluster->begin(); iter != currentCluster->end(); ++iter) { 

            currentDistance = EuclideanDistance(dataArray->at(i)->getFeatureVector(), iter->second->center); 

            if (currentDistance <= minDistance) {
                minDistance = currentDistance;
                minDistanceClusterId = iter->first;
            }
        }

        currentCluster->at(dataArray->at(i)->getLabel())->neigbours->erase(i);
        currentCluster->at(minDistanceClusterId)->neigbours->insert(i);
        currentCluster->at(minDistanceClusterId)->nNeigbours += 1;
        dataArray->at(i)->setLabel(minDistanceClusterId);
    }
}


void Kmeans::KmeansPlusPlus() {
            
    double sumDistance = 0;
    double currentDistance = 0;
    double minDistance = std::numeric_limits<double>::max();
    double minDistanceClusterId = 0;
    double randPercent = 0;


    int firstCenter = rand() % (dataArray->size() - 1); 
    
    for (int i = 0; i != featureVectorSize; i++) {
        currentCluster->at(0)->center->at(i) = dataArray->at(firstCenter)->getFeatureVector()->at(i);
    }

    currentCluster->at(0)->neigbours->insert(firstCenter);
    currentCluster->at(0)->id = 0;
    currentCluster->at(0)->nNeigbours = 1;  
    
        
    for (int i = 1; i != k; i++) {
        sumDistance = 0;


        for (auto iter : *dataArray) {
            minDistance = std::numeric_limits<double>::max();
            for (int j = 0; j != i; j++) {
                currentDistance = EuclideanDistance(iter.second->getFeatureVector(), 
                                                     currentCluster->at(j)->center);
                        
                currentDistance = pow(currentDistance, 2);

                if ((currentDistance < minDistance)) {
                    minDistanceClusterId = j;
                    minDistance = currentDistance;
                }
            }

            sumDistance += minDistance;
        }

       
        randPercent = ((rand() % 100))*sumDistance/100; 
        sumDistance = 0;
        std::unordered_set<int> usedIndex;
        int index = 0;  
       
        while (usedIndex.size() < (dataArray->size() - 1) && (randPercent > sumDistance)) {
            minDistance = std::numeric_limits<double>::max();
            index = rand() % (dataArray->size() - 1);
            if (usedIndex.find(index) == usedIndex.end()) {
                for (int j = 0; j != i; j++) {
                    currentDistance = EuclideanDistance(dataArray->at(index)->getFeatureVector(), 
                                                         currentCluster->at(j)->center);
                    currentDistance = pow(currentDistance, 2);
                    if ((currentDistance < minDistance)) {
                        minDistanceClusterId = j;
                        minDistance = currentDistance;
                    }
                }
                sumDistance += minDistance;
                usedIndex.insert(index);
            }

        } 
                 
        for (int j = 0; j != featureVectorSize; j++) {
            currentCluster->at(i)->center->at(j) = dataArray->at(index)->getFeatureVector()->at(j);
        } 

        currentCluster->at(i)->neigbours->insert(index);
        currentCluster->at(i)->id = i;
        currentCluster->at(i)->nNeigbours = 1;
    }
}


void Kmeans::fitCenters(std::map<int, Cluster*>* bufferCluster , int start, int finish) {

    for (int i = 0; i != k; i++) {
        for (int j = 0; j != featureVectorSize; j++) {
            bufferCluster->at(i)->center->at(j) = 0.0; 
        } 
        bufferCluster->at(i)->nNeigbours = 0;
    }


    double minDistance = std::numeric_limits<double>::max();
    double currentDistance = 0;
    int minDistanceClusterId = 0; 
    
    // finish doesnt include
    for (int i = start; i != finish; ++i) {
        
        minDistance = std::numeric_limits<double>::max();
        minDistanceClusterId = 0; 
       
        for (auto iter = currentCluster->begin(); iter != currentCluster->end(); ++iter) { 
            currentDistance = EuclideanDistance(dataArray->at(i)->getFeatureVector(), iter->second->center); 
            if (currentDistance <= minDistance) {
                minDistance = currentDistance;
                minDistanceClusterId = iter->first;
            }
        }
        

        bufferCluster->at(minDistanceClusterId)->nNeigbours += 1;
        std::transform(bufferCluster->at(minDistanceClusterId)->center->begin(),
                        bufferCluster->at(minDistanceClusterId)->center->end(), 
                        dataArray->at(i)->getFeatureVector()->begin(), 
                        bufferCluster->at(minDistanceClusterId)->center->begin(), 
                        std::plus<double>());
    }          
   
}


void Kmeans::fitIterCenters(std::map<int, Cluster*>* bufferCluster , int start, int finish, int iter) { 
    for (int i = 0; i != iter; i++) {
        fitCenters(bufferCluster, start, finish);       
        std::unique_lock<std::mutex> locker_flag(mutex_flag);
        flag += 1;
        conditional_flag.wait(locker_flag);
    } 
    std::unique_lock<std::mutex> locker_done(mutex_done);
    done += 1;
    locker_done.unlock();
}


void Kmeans::reg(std::vector<std::map<int, Cluster*>*> bufferClusters, int iteration) {
    int iter = 1;
    while (done != numThreads) {
        if ((flag != 0) && (flag == numThreads)) {
            {   
                std::unique_lock<std::mutex> locker(mutex_update);
                if (iter % 10 == 0) {
                    std::cout << " iteration = " << iter << "/" << iteration << std::endl;
                }
                iter += 1;
                this->makeZeroCluster();
                int currentN = 0;
                
                for (int i = 0; i != k; i++) {
                    currentN = 0;
                    for (int j = 0; j != bufferClusters.size(); j++) {
                        currentN += bufferClusters.at(j)->at(i)->nNeigbours;
                        std::transform(currentCluster->at(i)->center->begin(),
                                       currentCluster->at(i)->center->end(), 
                                       bufferClusters.at(j)->at(i)->center->begin(),
                                       currentCluster->at(i)->center->begin(),
                                       std::plus<double>());
                    }
                    if (currentN != 0) {
                        std::transform(currentCluster->at(i)->center->begin(), 
                                    currentCluster->at(i)->center->end(), 
                                    currentCluster->at(i)->center->begin(),
                                    [currentN](double num) { return num / currentN; });
                    }
                } 
            } 
            flag = 0;
            conditional_flag.notify_all();
        }
    }
}


void Kmeans::fitThreads(int iteration) {
    std::vector<std::map<int, Cluster*>*> bufferClusters;
    for (int num = 0; num != numThreads; num++) {
        std::map<int, Cluster*>* bufferCluster = new std::map<int, Cluster*>;
        for (int i = 0; i != k; i++) {
            Cluster* cluster = new Cluster;
            for (int j = 0; j != featureVectorSize; j++) {
                cluster->center->push_back(0.0);
            } 
            cluster->id = i;
            cluster->nNeigbours = 0;
            bufferCluster->insert(std::make_pair(i, cluster));
        }
        bufferClusters.push_back(bufferCluster);
    }

 
    std::thread loggerThread([this, bufferClusters, iteration]() {
        this->reg(bufferClusters, iteration);    
    });
    
    std::vector<std::thread> threads;
    int d = dataArray->size() / numThreads;
    int start = 0;
    int finish = 0;
    for(int i = 0; i != numThreads; ++i) {
        start = finish;
        finish = finish + d;

        threads.emplace_back([this, &bufferClusters, i,  start, finish, iteration] () {
                this->fitIterCenters(bufferClusters.at(i), start, finish, iteration);
        });
    }

    for(auto &t: threads) {
        t.join();
    }
    loggerThread.join();

    for (int num = 0; num != numThreads; num++) {
        for (int i = 0; i != k; i++) {
            delete bufferClusters.at(num)->at(i);
        }
        delete bufferClusters.at(num);
    }
}


int Kmeans::getK() {
    return k;
}


int Kmeans::getN() {
    return n;
}


int Kmeans::getFeatureVectorSize() {
    return featureVectorSize;
}


std::vector<std::string>* Kmeans::getFeatureNames() {
    return featureNames;
}


std::map<int, Point*>* Kmeans::getDataArray() {
    return dataArray;
}


// public
std::map<int, Cluster*>* Kmeans::getCurrentCluster() {
    return currentCluster;
}


void Kmeans::readDataCsv(std::string intputPath) {
    this->readCsv(intputPath, "first", k); 
}


void Kmeans::writeDataCsv(std::string outputPath) {
    this->writeCsv(outputPath, "data");
}


void Kmeans::writeCentersCsv(std::string outputPath) {
    this->writeCsv(outputPath, "centers");
}


void Kmeans::fit(int iteration) {
    this->KmeansPlusPlus();
    this->updatePoints();
    this->fitThreads(iteration);
    this->updatePoints();
}

int Kmeans::predict(std::vector<double>* point) {
    double minDistance = std::numeric_limits<double>::max();
    int minDistanceClusterId = 0; 
    double currentDistance = 0;

    for (auto iter = currentCluster->begin(); iter != currentCluster->end(); ++iter) { 
        currentDistance = EuclideanDistance(point, iter->second->center); 
        if (currentDistance <= minDistance) {
            minDistance = currentDistance;
            minDistanceClusterId = iter->first;
        }
    }

    return minDistanceClusterId; 
}