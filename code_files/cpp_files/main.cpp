/**
 * Author: Gilad Bitton
 * RedID: 130621085
 */
#include <iostream>
#include <unistd.h>
#include "restaurant.h"
#include "producer.h"
#include "consumer.h"
#include "log.h"

using namespace std;

/**
 * Main function to set up and run the restaurant seating system
 * 
 * @param argc Argument count
 * @param argv Argument vector
 * @return Exit status
 */
int main(int argc, char** argv) {
    int opt;
    int s = 110; // Total number of seating requests (production limit)
    int x = 0; // Specifies the time that T-X robot uses on average to process a seating request
    int r = 0; // Similar to above for Rev-9 robot
    int g = 0; // Average time for the general table greeter robot to producea general table request
    int v = 0; // Average time for VIP room greeter roboto to produce VIP room request
    while ((opt = getopt(argc, argv, "s:x:r:g:v:")) != -1) {
        switch (opt) {
            case 's': s = atoi(optarg); break;
            case 'x': x = atoi(optarg); break;
            case 'r': r = atoi(optarg); break;
            case 'g': g = atoi(optarg); break;
            case 'v': v = atoi(optarg); break;
            default:
                cerr << "Usage: " << argv[0]
                     << " [-s numRequests] [-x T-X ms] [-r Rev9 ms] [-g general ms] [-v VIP ms]]" << endl;
                     exit(0);
        }
    }

    SharedState state;
    state.maxRequests = s;

    // Set sleep times
    state.sleep_x = x;
    state.sleep_r = r;
    state.sleep_g = g;
    state.sleep_v = v;

    // Intialize mutex and condition variables
    pthread_mutex_init(&state.mutex, nullptr);
    pthread_cond_init(&state.not_full, nullptr);
    pthread_cond_init(&state.not_empty, nullptr);

    // Create producer and consumer threads
    pthread_t producers[2], consumers[2];

    // Creating General Table and VIP Room producer threads
    pthread_create(&producers[0], nullptr, restCtrl_prod, 
                   new pair<SharedState*, RequestType>(&state, GeneralTable));
    pthread_create(&producers[1], nullptr, restCtrl_prod, 
                   new pair<SharedState*, RequestType>(&state, VIPRoom));

    // Creating T-X and Rev-9 consumer robots
    pthread_create(&consumers[0], nullptr, restCtrl_cons, 
                   new pair<SharedState*, ConsumerType>(&state, TX));
    pthread_create(&consumers[1], nullptr, restCtrl_cons, 
                   new pair<SharedState*, ConsumerType>(&state, Rev9));
    
    // wait for all threads to finish
    for (auto& p : producers) pthread_join(p, nullptr);
    for (auto& c : consumers) pthread_join(c, nullptr);

    // Output production history
    unsigned int* consumedPtrs[ConsumerTypeN];
    for (int i = 0; i < ConsumerTypeN; ++i) 
        consumedPtrs[i] = state.consumed[i];
    output_production_history(state.produced, consumedPtrs);

    // Before exiting
    pthread_mutex_destroy(&state.mutex);
    pthread_cond_destroy(&state.not_full);
    pthread_cond_destroy(&state.not_empty);

    return 0;
}