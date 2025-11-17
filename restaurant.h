#ifndef RESTAURANT_H
#define RESTAURANT_H

using namespace std;

#include <pthread.h>
#include <queue>
#include "seating.h"

struct SharedState {
    queue<RequestType> requestQueue; // Queue to hold seating requests (GeneralTable or VIPRoom)

    int produced[RequestTypeN] = {0}; // Count of produced requests by type
    int consumed[ConsumerTypeN][RequestTypeN] = {{0}}; // Count of consumed requests by consumer and type 
    // e.g. consumed[TX][VIPRoom] = 5 means T-X robot has served 5 VIP rooms
    int inQueue[RequestTypeN] = {0}; // Current number of each request type in the queue

    unsigned int totalProduced = 0; // Total number of produced requests
    unsigned int maxRequests; // Maximum number of requests to be processed
    
    // Monitor synchronization
    pthread_mutex_t mutex;
    pthread_cond_t not_full; // Condition variable to signal queue not full
    pthread_cond_t not_empty; // Condition variable to signal queue not empty
};

#endif