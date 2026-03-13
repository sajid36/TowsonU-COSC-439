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

    int shmid = shmget(1234, sizeof(shared_data), 0666);
    shared_data *data = (shared_data *)shmat(shmid, NULL, 0);

    data->out = 0;

    while(1) {

        sem_wait(&data->full);
        sem_wait(&data->mutex);

        int item = data->buffer[data->out];
        printf("Consumed: %d\n", item);

        data->out = (data->out + 1) % BUFFER_SIZE;

        sem_post(&data->mutex);
        sem_post(&data->empty);

        sleep(2);
    }

    return 0;
}