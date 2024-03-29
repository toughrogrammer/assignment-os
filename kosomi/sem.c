/*
 * OS Assignment #3
 *
 * @file sem.c
 */

#include "sem.h"
#include <stdlib.h>
#include <stdio.h>


struct test_semaphore {
    int max_token;
    int token;

    pthread_mutex_t mutex;

    pthread_cond_t cond;
};

tsem_t *tsem_new(int value) {
    tsem_t *sem = malloc(sizeof(tsem_t));
    sem->max_token = sem->token = value;
    pthread_mutex_init(&(sem->mutex), NULL);
    pthread_cond_init(&(sem->cond), NULL);

    return sem;
}

void tsem_free(tsem_t *sem) {
    pthread_mutex_destroy(&(sem->mutex));
    pthread_cond_destroy(&(sem->cond));

    free(sem);
}

void tsem_wait(tsem_t *sem) {
    // printf("%d tsem_wait %p\n", (int) pthread_self(), sem);

    /* not yet implemented. */
    pthread_mutex_lock(&(sem->mutex));
    sem->token--;

    if (sem->token < 0) {
        int rc = 0;
        while (1) {
            rc = pthread_cond_wait(&(sem->cond), &(sem->mutex));
            if (rc == 0) {
                break;
            }

            printf("pthread_cond_wait error\n");
        };
    }

    pthread_mutex_unlock(&(sem->mutex));
}

int tsem_try_wait(tsem_t *sem) {
    pthread_mutex_lock(&(sem->mutex));
    {
        if (sem->token >= 1) {
            sem->token--;
            pthread_mutex_unlock(&(sem->mutex));
            return 0;
        }
    }
    pthread_mutex_unlock(&(sem->mutex));

    return 1;
}

void tsem_signal(tsem_t *sem) {
    // printf("%d tsem_signal %p\n", (int) pthread_self(), sem);

    /* not yet implemented. */
    pthread_mutex_lock(&(sem->mutex));

    sem->token++;

    if (sem->token <= 0) {
        int rc = 0;
        while (1) {
            rc = pthread_cond_signal(&(sem->cond));
            if (rc == 0) {
                break;
            }

            printf("pthread_cond_signal error\n");
        };
    }

    pthread_mutex_unlock(&(sem->mutex));
}
