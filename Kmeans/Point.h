#pragma once

#include <vector>
#include <cmath>


class Point {
    private:
        std::vector<double>* featureVector;
        int id;
        int label;
        double minDistance;

    public:
        Point();
        ~Point();

        void setMinDistance(double minDistance);
        void setId(int id);
        void setFeatureVector(std::vector<double>* featureVector);
        void setLabel(int label);
        void appendToFeatureVector(double val);

        int getFeatureVectorSize();
        int getLabel();
        int getId();
        
        double getMinDistance();
    
        std::vector<double>* getFeatureVector();
};