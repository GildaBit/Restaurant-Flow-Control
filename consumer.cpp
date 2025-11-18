/**
 * Author: Gilad Bitton
 * RedID: 130621085
 */
#include <unistd.h>
#include "restaurant.h"
#include "log.h"

using namespace std;

/**
 * Consumer thread function
 * 
 * @param arg Pointer to a pair containing SharedState pointer and ConsumerType
 * @return nullptr upon thread exit
 */
void* restCtrl_cons(void* arg) {
    auto* info = static_cast<pair<SharedState*, ConsumerType>*>(arg);
    SharedState* state = info->first;
    ConsumerType conType = info->second;
    delete info; // clean up allocated memory

    while(true) {
        // Lock the shared state mutex
        pthread_mutex_lock(&state->mutex);

        // wait if the queue is empty and production is not done
        while (state->requestQueue.empty() && state->totalProduced < state->maxRequests) {
            pthread_cond_wait(&state->not_empty, &state->mutex);
        }

        // If production is done and queue is empty, exit
        if (state->requestQueue.empty() && state->totalProduced >= state->maxRequests) {
            pthread_mutex_unlock(&state->mutex);
            break;
        }

        // Get request from the queue
        RequestType req = state->requestQueue.front();
        state->requestQueue.pop();
        state->consumed[conType][req]++;
        state->inQueue[req]--;
        state->totalConsumed++;

        // Output log
        output_request_removed(conType, req, state->consumed[conType], state->inQueue);

        // Signal that the queue is not full
        pthread_cond_broadcast(&state->not_full);

        // Unlock the mutex
        pthread_mutex_unlock(&state->mutex);

        // Simulate consumption time
        unsigned int sleepTime = (conType == TX) ? state->sleep_x :  state->sleep_r;
        if (sleepTime > 0) usleep(sleepTime * 1000); // Convert ms to us
    }



    pthread_exit(nullptr);
}