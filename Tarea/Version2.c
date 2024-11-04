#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//IMPORTANTE, al ejecutar el primer argumento es el numero de hilos y el segundo es el numero de ejecuciones

int max;
int *counters;
pthread_mutex_t mutex;

void *mythread(void *arg) {
    int index = *((int *)arg);
    free(arg);
    
    printf("Thread %d: begin [addr of index: %p]\n", index + 1, (void*)&index); // +1 para empezar desde 1
    for (int i = 0; i < max; i++) {
        pthread_mutex_lock(&mutex);
        counters[index]++;
        pthread_mutex_unlock(&mutex);
    }
    printf("Thread %d: done [addr of index: %p]\n", index + 1, (void*)&index); // +1 para empezar desde 1
    
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: main <num_threads> <loopcount>\n");
        exit(1);
    }

    int num_threads = atoi(argv[1]);
    max = atoi(argv[2]);
    counters = malloc(num_threads * sizeof(int));
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < num_threads; i++) {
        counters[i] = 0;
        int *index = malloc(sizeof(int));
        *index = i;
        pthread_create(&threads[i], NULL, mythread, index);
        pthread_join(threads[i], NULL); // Espera que el hilo termine antes de continuar
    }

    int total = 0;
    for (int i = 0; i < num_threads; i++) {
        total += counters[i];
    }

    printf("main: done\n [counter: %d]\n [should: %d]\n", total, max * num_threads);

    free(counters);
    free(threads);
    pthread_mutex_destroy(&mutex);
    return 0;
}


