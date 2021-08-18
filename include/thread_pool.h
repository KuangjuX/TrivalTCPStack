#include "global.h"

typedef struct {
    pthread_t* thread;
    int allocated;
} handler;

typedef struct {
    pthread_mutex_t lock;
    handler* table[THREAD_POOL_SIZE];
} thread_pool;

thread_pool* pool;

int apply() {
    pthread_mutex_lock(&pool->lock);
    int index;
    for (int i = 0; i < THREAD_POOL_SIZE; i++) {
        if (!pool->table[THREAD_POOL_SIZE]->allocated) {
            pool->table[i]->allocated = TRUE;
            index = i;
        }
    }
    pthread_mutex_unlock(&pool->lock);
    return index;
}

void destory(int index) {
    pthread_mutex_lock(&pool->lock);
    pthread_cancel(*pool->table[index]->thread);
    pool->table[index]->thread = NULL;
    pool->table[index]->allocated = FALSE;
    pthread_mutex_unlock(&pool->lock);
}