#include "Metrics.h"

double ClusterSilhouetteIndex(Kmeans* data) {
    double result = 0.0;

    std::vector<double>* s = new std::vector<double>;

    int nearnestCluster = 0;
    double sumDistance = 0.0;
    
    double currentSumDistance = 0.0;
    double sumMinDistance = std::numeric_limits<double>::max();

    for (auto cluster : *data->getCurrentCluster()) {
        for (auto point : *cluster.second->neigbours) {
            sumMinDistance = std::numeric_limits<double>::max();
            Point* currentPoint = new Point;
            currentPoint = data->getDataArray()->at(point);
            nearnestCluster = currentPoint->getLabel();
            sumDistance = 0.0;
            for (auto pointId: *data->getCurrentCluster()->at(nearnestCluster)->neigbours) {
                sumDistance += EuclideanDistance(currentPoint->getFeatureVector(), data->getDataArray()->at(pointId)->getFeatureVector());
            }
            sumDistance = sumDistance / data->getCurrentCluster()->at(nearnestCluster)->nNeigbours;
            int minDistanceClusterId = 0;
            

            sumMinDistance = std::numeric_limits<double>::max();
            for (auto clusterId : *data->getCurrentCluster()) {
                currentSumDistance = 0.0;
                if (clusterId.first != nearnestCluster) {
                    for (auto pointId: *data->getCurrentCluster()->at(clusterId.first)->neigbours) {
                        currentSumDistance += EuclideanDistance(currentPoint->getFeatureVector(), data->getDataArray()->at(pointId)->getFeatureVector());
                    }
                    
                    if (currentSumDistance < sumMinDistance) {
                        sumMinDistance = currentSumDistance;
                        minDistanceClusterId = clusterId.first; 
                    }    
                }
            }

            sumMinDistance = sumMinDistance / data->getCurrentCluster()->at(minDistanceClusterId)->nNeigbours;
 
            s->push_back((sumMinDistance - sumDistance) / std::max(sumMinDistance, sumDistance)); 
        }  
        double sumCluster = 0.0;
        for (auto iter : *s) {
            sumCluster += iter;
        }
        result += sumCluster / s->size();
    }

    result = result / data->getCurrentCluster()->size();

    return result;
}