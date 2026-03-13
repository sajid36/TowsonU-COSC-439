#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    sem_t mutex;
    sem_t empty;
    sem_t full;
} shared_data;

int main() {

    int shmid = shmget(1234, sizeof(shared_data), 0666 | IPC_CREAT);
    shared_data *data = (shared_data *)shmat(shmid, NULL, 0);

    sem_init(&data->mutex, 1, 1);
    sem_init(&data->empty, 1, BUFFER_SIZE);
    sem_init(&data->full, 1, 0);

    data->in = 0;

    int item = 1;

    while(1) {

        sem_wait(&data->empty);
        sem_wait(&data->mutex);

        data->buffer[data->in] = item;
        printf("Produced: %d\n", item);

        data->in = (data->in + 1) % BUFFER_SIZE;
        item=(item%5)+1;

        sem_post(&data->mutex);
        sem_post(&data->full);

        sleep(1);
    }

    return 0;
}