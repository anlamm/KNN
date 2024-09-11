#include "data.h"

data::data(){
    feature_vector = new std::vector<uint8_t>;
}

data::~data(){
    
}

void data::set_feature_vector(std::vector<uint8_t> * vect){
    feature_vector = vect;
}

void data::append_to_feature_vector(uint8_t val){
    feature_vector->push_back(val);
}

void data::set_label(uint8_t val){
    label = val;
}

void data::set_enumerated_label(int val){
    enum_label = val;
}

unsigned int data::get_feature_vector_size(){
    return feature_vector->size();
}

uint8_t data::get_label(){
    return label;
}

int data::get_enum_label(){
    return enum_label;
}

std::vector<uint8_t> * data::get_feature_vector(){
    return feature_vector;
}

void data::insert_heap(std::pair<data *, unsigned int> node){
    heap.push_back(node);
    int i = heap.size() - 1;
    while (i != 1 && heap[i].second < heap[(i-1)/2].second){ // (i-1)/2 is the parent node
        _swap(i, (i-1)/2);
        i = (i-1)/2;
    }
}

void data::_swap(int i, int pi){
    std::pair<data *, unsigned int> temp;
    temp.first = heap.at(i).first;
    temp.second = heap.at(i).second;
    heap.at(i).first = heap.at(pi).first;
    heap.at(i).second = heap.at(pi).second;
    heap.at(pi).first = temp.first;
    heap.at(pi).second = temp.second;
}

data* data::get_min(){
    data* ans = heap.at(0).first;
    pop_min();
    return ans;
}

void data::pop_min(){
    int n = heap.size() - 1;
    _swap(0, n);
    heap.pop_back();
    --n;
    int i = 0;
    while(1){
        int m = i;
        if (i*2 + 1 <= n && heap.at(i*2 + 1).second < heap.at(i).second){
            m = i*2 + 1;
        }
        if (i*2 + 2 <= n && heap.at(i*2 + 2).second < heap.at(m).second){
            m = i*2 + 2;
        }
        if (m == i){
            break;
        }
        _swap(m, i);
        i = m;
    }
}