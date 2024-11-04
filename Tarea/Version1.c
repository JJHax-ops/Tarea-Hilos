#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max;

typedef struct {
    int counter;
    char *letter;
} thread_data;

void *mythread(void *arg) {
    thread_data *data = (thread_data *)arg;
    int i; // stack (private per thread)
    printf("%s: begin [addr of i: %p]\n", data->letter, &i);
    
    for (i = 0; i < max; i++) {
        data->counter++;
    }
    
    printf("%s: done [counter = %d]\n", data->letter, data->counter);
    return (void *)(long)data->counter; // devolver el contador
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: main-first <loopcount>\n");
        exit(1);
    }
    max = atoi(argv[1]);

    pthread_t p1, p2;
    
    // Crear datos de hilo de forma dinámica para evitar compartir
    thread_data *data1 = malloc(sizeof(thread_data));
    thread_data *data2 = malloc(sizeof(thread_data));
    data1->counter = 0;
    data1->letter = "A";
    data2->counter = 0;
    data2->letter = "B";

    // Imprimir direcciones de las estructuras
    printf("main: data1 address: %p\n", (void*)data1);
    printf("main: data2 address: %p\n", (void*)data2);

    printf("main: begin [should: %d]\n", max * 2);
    pthread_create(&p1, NULL, mythread, data1);
    pthread_create(&p2, NULL, mythread, data2);

    void *result1;
    void *result2;
    pthread_join(p1, &result1);
    pthread_join(p2, &result2);

    int total_counter = data1->counter + data2->counter;

    printf("main: done\n [counter: %d]\n [should: %d]\n",
           total_counter, max * 2);

    // Liberar la memoria asignada
    free(data1);
    free(data2);

    return 0;
}


