#include "data_handler.h"

data_handler::data_handler(){
    data_array = new std::vector<data *>;
    training_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
    testing_data = new std::vector<data *>;
}

data_handler::~data_handler(){
    
}

void data_handler::read_feature_vector(std::string path){
    uint32_t header[4]; //magic, num_images, rowsize, colsize
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "rb+");
    if (f){
        for(int i = 0; i < 4; ++i){
            if(fread(bytes, sizeof(bytes), 1, f)){
                header[i] = convert_to_little_endian(bytes);
            }
        }
        printf("Done getting input file header.\n");
        int image_size = header[2] * header[3];
        for (unsigned int i = 0; i < header[1]; ++i){
            data *d = new data();
            uint8_t element[1];
            for (int j = 0; j < image_size; ++j){
                if (fread(element, sizeof(element), 1, f)){
                    d->append_to_feature_vector(element[0]);
                }
                else{
                    printf("Err reading from input file when i=%d, j=%d.\n",i ,j);
                    exit(1);
                }
            }
            data_array->push_back(d);
        }
        printf("successfully read and stored feature vectors.\n");
    }
    else{
        printf("Could not find file.\n");
        exit(1);
    }
}

void data_handler::read_feature_labels(std::string path){
    uint32_t header[2]; //magic, num_images
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "rb+");
    if (f){
        for(int i = 0; i < 2; ++i){
            if(fread(bytes, sizeof(bytes), 1, f)){
                header[i] = convert_to_little_endian(bytes);
            }
        }
        printf("Done getting label file header.\n");
        for (unsigned int i = 0; i < header[1]; ++i){
            uint8_t element[1];
            if (fread(element, sizeof(element), 1, f)){
                data_array->at(i)->set_label(element[0]);
            }
            else{
                printf("Err reading from label file.\n");
                exit(1);
            }
        }
        printf("successfully read and stored labels.\n");
    }
    else{
        printf("Could not find file.\n");
        exit(1);
    }
}

void data_handler::split_data(){
    // std::vector<int> indices;
    std::unordered_set<int> used_indexes;
    int train_size = data_array->size() * TRAIN_SET_PERCENT;
    int test_size = data_array->size() * TEST_SET_PERCENT;
    int valid_size = data_array->size() * VALIDATION_PERCENT;

    int data_array_size = data_array->size();
    // for (int i = 0; i < data_array_size; ++i){
    //     indices.push_back(i);
    // }

    
    // for (int i = 0; i < train_size; ++i){
    //     int rand_index = rand() % data_array_size;
    //     training_data->push_back(data_array->at(indices.at(rand_index)));
    // }

    //Test data

    int count = 0;
    int rep = 0;
    while (count < test_size){
        if (rep > 100) {
            srand(time(NULL));
            rep = 0;
        }
        int rand_index = rand() % data_array_size;
        if (used_indexes.find(rand_index) == used_indexes.end()){
            testing_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            ++count;
            rep = 0;
        }
        else{
            ++rep;
        }
    }

    //Validation data

    count = 0;
    rep = 0;
    while (count < valid_size){
        if (rep > 100) {
            srand(time(NULL));
            rep = 0;
        }
        int rand_index = rand() % data_array_size;
        if (used_indexes.find(rand_index) == used_indexes.end()){
            validation_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            ++count;
            rep = 0;
        }
        else{
            ++rep;
        }
    }

    //Training data

    count = 0;
    rep = 0;
    while (count < train_size){
        if (rep > 100) {
            srand(time(NULL));
            rep = 0;
        }
        int rand_index = (rand() % data_array_size + rand() + rand_index)%data_array_size;
        if (used_indexes.find(rand_index) == used_indexes.end()){
            training_data->push_back(data_array->at(rand_index));
            used_indexes.insert(rand_index);
            ++count;
            rep = 0;
        }
        else{
            ++rep;
        }
    }

    printf("Training data size: %lu.\n", training_data->size());
    printf("Testing data size: %lu.\n", testing_data->size());
    printf("Validation data size: %lu.\n", validation_data->size());
}

void data_handler::count_classes(){
    int count = 0;
    for (unsigned int i = 0; i < data_array->size(); ++i){
        if (class_map.find(data_array->at(i)->get_label()) == class_map.end()){
            class_map[data_array->at(i)->get_label()] = count;
            data_array->at(i)->set_enumerated_label(count);
            ++count;
        }
    }
    num_classes = count;
    printf("successfully extracted %d unique classes.\n", num_classes);
}

uint32_t data_handler::convert_to_little_endian(const unsigned char* bytes){
    return (uint32_t) ((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]));
}

std::vector<data *> * data_handler::get_training_data(){
    return training_data;
}

std::vector<data *> * data_handler::get_testing_data(){
    return testing_data;
}

std::vector<data *> * data_handler::get_validation_data(){
    return validation_data;
}

// int main(){
//     data_handler *dh = new data_handler();
//     dh->read_feature_vector("../data/train-images.idx3-ubyte");
//     dh->read_feature_labels("../data/train-labels.idx1-ubyte");
//     dh->split_data();
//     dh->count_classes();
//     return 0;
// }