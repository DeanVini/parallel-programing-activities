#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define QUEUE_SIZE 6
#define VECTOR_CLOCK_SIZE 3

pthread_mutex_t mutex;
pthread_cond_t condFull;
pthread_cond_t condEmpty;

typedef struct {
    int id;
    int vectorClock[VECTOR_CLOCK_SIZE];
} VectorClock;

typedef struct {
    VectorClock vectorClockQueue[QUEUE_SIZE];
    int arraySize;
} Queue;

int producerTime;
int consumerTime;
Queue queue = {.arraySize = 0};

void addToQueue(Queue *queue, VectorClock vectorClock) {
    pthread_mutex_lock(&mutex);
    int arraySize = queue->arraySize;
    
    while(arraySize == QUEUE_SIZE) {
        printf("Queue is full\n");
        pthread_cond_wait(&condFull, &mutex);
        arraySize = queue->arraySize; // Update after waiting
    }
    
    vectorClock.id = arraySize;
    queue->vectorClockQueue[arraySize] = vectorClock;
    queue->arraySize++;
    
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&condEmpty);
}

VectorClock removeFromQueue(Queue *queue) {
    pthread_mutex_lock(&mutex);
    
    while (queue->arraySize == 0){
        printf("Queue is empty\n");
        pthread_cond_wait(&condEmpty, &mutex);
    }

    VectorClock removedVectorClock = queue->vectorClockQueue[0];
    
    for(int i = 0; i < queue->arraySize - 1; i++){
        queue->vectorClockQueue[i] = queue->vectorClockQueue[i + 1];
    }
    
    queue->arraySize--;
    
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&condFull);
    
    return removedVectorClock;
}

void* producerClock(void* arg){
    int id = *(int *) arg;
    VectorClock vectorClock;
    while(1){
        for(int i = 0; i < VECTOR_CLOCK_SIZE; i++){
            vectorClock.vectorClock[i] = id + 1;
        }
        addToQueue(&queue, vectorClock);
        printf("Producer %d produced: [%d, %d, %d]\n", id, vectorClock.vectorClock[0], vectorClock.vectorClock[1], vectorClock.vectorClock[2]);
        
        sleep(producerTime);
    }
}

void* consumerClock(void* arg){
    int id = *(int *) arg;
    VectorClock removedVectorClock;
    
    while(1){
        removedVectorClock = removeFromQueue(&queue);
        printf("Consumer %d consuming: [%d, %d, %d]\n", id, removedVectorClock.vectorClock[0], removedVectorClock.vectorClock[1], removedVectorClock.vectorClock[2]);
        
        sleep(consumerTime);
    }
}

int main(int argc, char* argv[]) {
    int scenario;
    printf("Enter 1 for the scenario where the queue will be full\n");
    printf("Enter 2 for the scenario where the queue will be empty\n");
    scanf("%d", &scenario);
    
    switch(scenario){
        case 1: 
            producerTime = 1;
            consumerTime = 2;
            break;
        case 2:
            producerTime = 2;
            consumerTime = 1;
            break;
        default:
            printf("Invalid option, option 1 will be chosen by default.\n");
            producerTime = 1;
            consumerTime = 2;
    }
    
    printf("\n\n\n");
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condEmpty, NULL);
    pthread_cond_init(&condFull, NULL);
    
    pthread_t producers[3], consumers[3];
    int producerIds[3], consumerIds[3];
    
    for (int i = 0; i < 3; i++) {
        producerIds[i] = i + 1;
        pthread_create(&producers[i], NULL, producerClock, &producerIds[i]);
    }
    
    for (int i = 0; i < 3; i++) {
        consumerIds[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumerClock, &consumerIds[i]);
    }
    
    for (int i = 0; i < 3; i++) {
        pthread_join(producers[i], NULL);
    }
    
    for (int i = 0; i < 3; i++) {
        pthread_join(consumers[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condEmpty);
    pthread_cond_destroy(&condFull);
    return 0;
}
