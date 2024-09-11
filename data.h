#ifndef __DATA_H
#define __DATA_H

#include <vector>
#include <utility>
#include "stdint.h"
#include "stdio.h"

class data{
    std::vector<uint8_t> *feature_vector;
    std::vector<std::pair<data *, unsigned int>> heap;
    uint8_t label;
    int enum_label; //A:1, B:2

    void _swap(int i, int pi);
    void pop_min();

    public:
    data();
    ~data();
    void set_feature_vector(std::vector<uint8_t> *);
    void append_to_feature_vector(uint8_t);
    void set_label(uint8_t);
    void set_enumerated_label(int);
    void insert_heap(std::pair<data *, unsigned int>);
    data* get_min();

    unsigned int get_feature_vector_size();
    uint8_t get_label();
    int get_enum_label();

    std::vector<uint8_t> * get_feature_vector();

};

#endif