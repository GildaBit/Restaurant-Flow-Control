#include <unistd.h>
#include "restaurant.h"
#include "log.h"

using namespace std;


void* restCtrl_prod(void* arg) {
    auto* info = static_cast<pair<SharedState*, RequestType>*>(arg);
    SharedState* state = info->first;
    RequestType reqType = info->second;
    delete info; // clean up allocated memory



    while(true) {

        // Simulate production time
        unsigned int sleepTime = (reqType == GeneralTable) ? state->sleep_g :  state->sleep_v;
        if (sleepTime > 0) usleep(sleepTime * 1000); // Convert ms to us
        

        // Lock the shared state mutex
        pthread_mutex_lock(&state->mutex);

        // Stop taking requests if maxRequests reached
        if (state->totalProduced >= state->maxRequests) {
            pthread_mutex_unlock(&state->mutex);
            break;
        }

        // wait if the queue is full or VIP limit reached for VIP requests
        while (state->requestQueue.size() >= QUEUE_MAX_SIZE || (reqType == VIPRoom && state->inQueue[VIPRoom] >= VIP_LIMIT)) {
            pthread_cond_wait(&state->not_full, &state->mutex);
        }

        if (state->totalProduced < state->maxRequests) {
            // Add request to the queue
            state->requestQueue.push(reqType);
            state->produced[reqType]++;
            state->inQueue[reqType]++;
            state->totalProduced++;

            // Output log
            output_request_added(reqType, state->produced, state->inQueue);

            // Signal that the queue is not empty
            pthread_cond_broadcast(&state->not_empty);
        }

        // Unlock the mutex
        pthread_mutex_unlock(&state->mutex);


    }

    // Wake up any waiting consumers
    pthread_mutex_lock(&state->mutex);
    pthread_cond_broadcast(&state->not_empty);
    pthread_mutex_unlock(&state->mutex);

    pthread_exit(nullptr);

}