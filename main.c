#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "thread_interface.h"

int main(int argc, char* argv[]) {
    if (argc < 3) { print_usage(argv[0]); return 1; }

    int max_threads = atoi(argv[1]);
    int length = atoi(argv[2]);
    int numb_aray = (argc == 4) ? atoi(argv[3]) : 5;
    
    if (max_threads <= 0 || length <= 0 || numb_aray <= 0) {
        printf("Ошибка: параметры должны быть > 0\n");
        return 1;
    }
    
    printf("Параметры: %d потоков, %d элементов, %d массивов\n", 
           max_threads, length, numb_aray);
    
    int strategy = choose_strategy(numb_aray, length, max_threads);
    printf("Стратегия: %s\n", strategy ? "по массивам" : "по элементам");
    
    int** arrays = NULL;
    int* result = malloc(length * sizeof(int));
    generate_test_data(&arrays, result, numb_aray, length);

    void** threads = malloc(max_threads * sizeof(void*));
    thread_data_t* data = malloc(max_threads * sizeof(thread_data_t));
    
    int chunk = (strategy == 0) ? 
        ((length + max_threads - 1) / max_threads) :
        ((numb_aray + max_threads - 1) / max_threads);
    
    clock_t start = clock();
    
    for (int i = 0; i < max_threads; i++) {
        data[i] = (thread_data_t){arrays, result, numb_aray, length, i * chunk, 
            (i == max_threads-1) ? 
            (strategy ? numb_aray : length) :
            (i+1) * chunk, strategy, i+1};
            
        threads[i] = thread_create(array_sum_thread, &data[i]);
    }
    
    for (int i = 0; i < max_threads; i++) 
        thread_join(threads[i]);
    
    double time = ((double)(clock() - start)) / CLOCKS_PER_SEC;
    
    printf("Время: %.3f сек\n", time);
    printf("Результат: %s\n", verify_result(arrays, result, numb_aray, length) ? "все хорошо" : "все плохо");
    
    printf("Первые 5 элементов: ");
    for (int i = 0; i < 5 && i < length; i++) 
        printf("%d ", result[i]);
    printf("\n");

    
    free(threads);
    free(data);
    free_memory(arrays, result, numb_aray);
    
    return 0;
}