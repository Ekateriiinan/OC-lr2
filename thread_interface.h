#pragma once

#include <stddef.h>

typedef struct {
    int** arrays;
    int* result;
    int numb_aray, length, start_idx, end_idx, strategy, thread_id;
} thread_data_t;

typedef void* (*thread_func_t)(void*);

void* thread_create(thread_func_t func, void* arg);
void thread_join(void* thread);
void thread_sleep(int ms);
unsigned long get_thread_id(void);

void* array_sum_thread(void* arg);
int choose_strategy(int numb_aray, int length, int max_threads);
void generate_test_data(int*** arrays, int* result, int numb_aray, int length);
void free_memory(int** arrays, int* result, int numb_aray);
int verify_result(int** arrays, int* result, int numb_aray, int length);
void print_usage(const char* name);