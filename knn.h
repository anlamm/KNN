#ifndef __KNN_H
#define __KNN_H

#include <vector>
#include "data.h"

class knn{
    int k;
    //std::vector<data *> *neighbors; 
    data ** neighbors; //k neighbors for the query point
    std::vector<data *> *training_data;
    std::vector<data *> *testing_data;
    std::vector<data *> *validation_data;

    public:
    knn() = default;
    knn(int);
    ~knn();

    void find_knearest(data *query_point);
    void set_training_data(std::vector<data *> *vect);
    void set_testing_data(std::vector<data *> *vect);
    void set_validaiton_data(std::vector<data *> *vect);
    void set_k(int val);

    int predict();
    double calculate_distance(data* query_point, data* input);
    double validate_performance();
    double test_performance();
    
};

#endif