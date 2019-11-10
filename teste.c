#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h> //para srand

void * thr (void * id) {
    long int a = (long int) id;

    srand((unsigned) time(NULL) * a);
    printf("%d\n",1 + (rand()%100));
} 

int main () {
    pthread_t t[5];
    for (long int i = 0; i < 5; i++)
    {
        
        pthread_create(&t[i], NULL, thr, (void *) i);
    }

    for (int i = 0; i < 5; i++)
        pthread_join(t[i], NULL);

        return 0;
}