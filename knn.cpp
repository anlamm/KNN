#include "knn.h"
#include <cmath>
#include <map>
#include <stdint.h>
#include <utility>
#include <vector>
#include <algorithm>
#include "data_handler.h"

knn::knn(int val): k(val){}
knn::~knn(){
    delete neighbors;
    delete training_data;
    delete testing_data;
    delete validation_data;
}

void knn::find_knearest(data *query_point){
    for (unsigned int i = 0; i < training_data->size(); ++i){
        unsigned int d = calculate_distance(query_point, training_data->at(i));
        std::pair<data*, unsigned int> p;
        p = std::make_pair(training_data->at(i), d);
        query_point->insert_heap(p);
    }
    neighbors = new data*[k];
    for (int i = 0; i < k; ++i){
        data *d = query_point->get_min();
        neighbors[i] = d;
    }
    // for (int i = 0; i < this->k; ++i){
    //     data *d = nullptr;
    //     d = query_point->get_min();
    //     nei[0] = d;
    //     printf("neighbor = %d", (int)d);
    //     neighbors->push_back(d);
    //     printf("finish push back\n");
    // }
}

void knn::set_training_data(std::vector<data *> *vect){
    training_data = vect;
}

void knn::set_testing_data(std::vector<data *> *vect){
    testing_data = vect;
}

void knn::set_validaiton_data(std::vector<data *> *vect){
    validation_data = vect;
}
void knn::set_k(int val){
    k = val;
}

int knn::predict(){
    std::vector<std::pair<uint8_t, unsigned int>> class_freq;
    for (int i = 0; i < k; ++i){
        data *d = neighbors[i];
        if (std::find_if(class_freq.begin(), class_freq.end(), [d](const auto& p){return p.first == d->get_label();}) == class_freq.end()){
            class_freq.push_back(std::make_pair(d->get_label(), 1));
        }
        else{
            ++(std::find_if(class_freq.begin(), class_freq.end(), [d](const auto& p){return p.first == d->get_label();})->second);
        }
    }
    unsigned int _max = 0;
    int label = -1;
    for (unsigned int i = 0; i < class_freq.size(); ++i){
        if (class_freq[i].second > _max){
            label = class_freq[i].first;
            _max = class_freq[i].second;
        }
    }
    return label;
}

double knn::calculate_distance(data* query_point, data* input){
    double distance = 0.0;
    if(query_point->get_feature_vector_size() != input->get_feature_vector_size()){
        printf("Err vector size mismatch.\n");
        exit(1);
    }
    for (unsigned i = 0; i < query_point->get_feature_vector_size(); ++i){
        distance += pow(query_point->get_feature_vector()->at(i) - input->get_feature_vector()->at(i), 2);
    }
    distance = sqrt(distance);
// #ifdef EUCLID
//     for (unsigned i = 0; i < query_point->get_feature_vector_size(); ++i){
//         distance += pow(query_point->get_feature_vector()->at(i) - input->get_feature_vector()->at(i), 2);
//     }
//     distance = sqrt(distance);
// #elif defined MANHATTAN
// #endif
    return distance;
}

double knn::validate_performance(){
    double current_performance = 0;
    int count = 0;
    int data_index = 0;
    for (data* query_point: *validation_data){
        // while (neighbors->size() > 0){
        //     neighbors->pop_back();
        // }
        find_knearest(query_point);
        int prediction = predict();
        if (prediction == query_point->get_label()){
            ++count;
        }
        ++data_index;
        printf("Current Performance = %.3f %%\n", ((double)count*100.0)/((double)data_index));
    }
    current_performance = ((double)count*100.0)/((double)validation_data->size());
    printf("Validation Performance for K = %d: %.3f %%\n", k, current_performance);
    return current_performance;
}

double knn::test_performance(){
    double current_performance = 0;
    int count = 0;
    for (data* query_point: *testing_data){
        // while (neighbors->size() > 0){
        //     neighbors->pop_back();
        // }
        find_knearest(query_point);
        int prediction = predict();
        if (prediction == query_point->get_label()){
            ++count;
        }
    }
    current_performance = ((double)count*100.0)/((double)testing_data->size());
    printf("Tested performance = %3.f %%\n", current_performance);
    return current_performance;
}

int main(){
    data_handler *dh = new data_handler();
    dh->read_feature_vector("./data/train-images.idx3-ubyte");
    dh->read_feature_labels("./data/train-labels.idx1-ubyte");
    dh->split_data();
    dh->count_classes();
    knn *knearest = new knn();
    knearest->set_training_data(dh->get_training_data());
    knearest->set_validaiton_data(dh->get_validation_data());
    knearest->set_testing_data(dh->get_testing_data());
    double performance = 0;
    double best_performance = 0;
    int best_k = 1;
    for (int i = 1; i <= 4; ++i){
        knearest->set_k(i);
        performance = knearest->validate_performance();
        if (best_performance < performance){
            best_performance = performance;
            best_k = i;
        }
    }
    printf("Best K = %d", best_k);
    knearest->set_k(best_k);
    knearest->test_performance();
    return 0;
}