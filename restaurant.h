/**
 * Author: Gilad Bitton
 * RedID: 130621085
 */
#ifndef RESTAURANT_H
#define RESTAURANT_H

using namespace std;

#include <pthread.h>
#include <queue>
#include "seating.h"

#define QUEUE_MAX_SIZE 20 // Maximum size of the request queue
#define VIP_LIMIT 6 // Maximum number of concurrent VIP room requests

/**
 * @brief Shared state structure for the restaurant seating system
 */
struct SharedState {
    queue<RequestType> requestQueue; // Queue to hold seating requests (GeneralTable or VIPRoom)

    unsigned int produced[RequestTypeN] = {0}; // Count of produced requests by type
    unsigned int consumed[ConsumerTypeN][RequestTypeN] = {{0}}; // Count of consumed requests by consumer and type 
    // e.g. consumed[TX][VIPRoom] = 5 means T-X robot has served 5 VIP rooms
    unsigned int inQueue[RequestTypeN] = {0}; // Current number of each request type in the queue

    unsigned int totalProduced = 0; // Total number of produced requests
    unsigned int totalConsumed = 0; // Total number of consumed requests
    unsigned int maxRequests; // Maximum number of requests to be processed
    
    // sleep times (ms)
    unsigned int sleep_g = 0; // General table producer
    unsigned int sleep_v = 0; // VIP producer
    unsigned int sleep_x = 0; // T-X consumer
    unsigned int sleep_r = 0; // Rev-9 consumer
    
    // Monitor synchronization
    pthread_mutex_t mutex;
    pthread_cond_t not_full; // Condition variable to signal queue not full
    pthread_cond_t not_empty; // Condition variable to signal queue not empty
};

#endif