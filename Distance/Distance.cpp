#include "Distance.h"


double EuclideanDistance(const std::vector<double>* query_point, const std::vector<double>* input) {
    double distance = 0.0;

    if (query_point->size() != input->size()) {
        std::cout << "Ошибка размерности входного вектора";
    } else {
        for (int i = 0; i != query_point->size(); i++) {
            distance += pow( query_point->at(i) - input->at(i), 2);
        }
    }

    distance = sqrt(distance);
    return distance;
}
