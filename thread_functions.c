#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "thread_interface.h"

void print_usage(const char* name) {
    printf("Использование: 1. исполняемый файл 2. кол-во потоков 3. длина 4. кол-во массивов\n");
    printf("Например: %s 4 10 5\n", name);
}

void* array_sum_thread(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    printf("Поток %lu начал работу\n", get_thread_id());
    
    if (data->strategy == 0) {
        for (int i = data->start_idx; i < data->end_idx; i++) {
            data->result[i] = 0;
            for (int j = 0; j < data->numb_aray; j++) 
                data->result[i] += data->arrays[j][i];
        }
    } else {
        for (int i = 0; i < data->length; i++) {
            for (int j = data->start_idx; j < data->end_idx; j++) 
                data->result[i] += data->arrays[j][i];
        }
    }
    
    printf("Поток %lu завершил\n", get_thread_id());
    return NULL; 
}


int choose_strategy(int numb_aray, int length, int max_threads) {
    long total_ops = (long)numb_aray * length;
    if (total_ops < 10000) return 0;
    
    double ratio = (double)numb_aray / length;
    if (ratio > 2.0) return 1;
    if (ratio < 0.5) return 0;
    
    return (numb_aray >= max_threads && length >= max_threads) ? 
           ((numb_aray % max_threads == 0) ? 1 : 0) : (numb_aray >= max_threads);
}

void generate_test_data(int*** arrays, int* result, int numb_aray, int length) {
    *arrays = malloc(numb_aray * sizeof(int*));
    srand(time(NULL));
    
    for (int i = 0; i < numb_aray; i++) {
        (*arrays)[i] = malloc(length * sizeof(int));
        for (int j = 0; j < length; j++) 
            (*arrays)[i][j] = rand() % 100;
    }
    
    for (int i = 0; i < length; i++) result[i] = 0;
}

int verify_result(int** arrays, int* result, int numb_aray, int length) {
    for (int i = 0; i < length; i++) {
        int sum = 0;
        for (int j = 0; j < numb_aray; j++) sum += arrays[j][i];
        if (sum != result[i]) return 0;
    }
    return 1;
}

void free_memory(int** arrays, int* result, int numb_aray) {
    if (arrays) {
        for (int i = 0; i < numb_aray; i++) free(arrays[i]);
        free(arrays);
    }
    if (result) free(result);
}