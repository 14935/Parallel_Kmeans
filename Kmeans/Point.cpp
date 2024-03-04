#include "Point.h"


Point::Point() {
    featureVector = new std::vector<double>;
}

Point::~Point() {
    delete featureVector;
}

void  Point::setFeatureVector(std::vector<double>* featureVector) {
    this->featureVector = featureVector;
}

void Point::setLabel(int label) {
    this->label = label;
}

void Point::setId(int id) {
    this->id = id;
}

void Point::appendToFeatureVector(double val) {
    this->featureVector->push_back(val);
}

int Point::getFeatureVectorSize() {
    return this->featureVector->size();
}

int Point::getLabel() {
    return label;
}

int Point::getId() {
    return id;
}


std::vector<double>* Point::getFeatureVector(){
    return this->featureVector;
}

void Point::setMinDistance(double minDistance) {
    this->minDistance = minDistance;
}

double Point::getMinDistance() {
    return minDistance;
}