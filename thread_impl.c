#include "thread_interface.h"
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

void* thread_create(thread_func_t func, void* arg) {
    return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, arg, 0, NULL);
}

void thread_join(void* thread) {
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
}

void thread_sleep(int ms) { Sleep(ms); }

unsigned long get_thread_id(void) { return GetCurrentThreadId(); }

#else
#include <pthread.h>
#include <unistd.h>

void* thread_create(thread_func_t func, void* arg) {
    pthread_t* thread = malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, (void*(*)(void*))func, arg);
    return thread;
}

void thread_join(void* thread) {
    pthread_join(*(pthread_t*)thread, NULL);
    free(thread);
}

void thread_sleep(int ms) { usleep(ms * 1000); }

unsigned long get_thread_id(void) { return (unsigned long)pthread_self(); }
#endif