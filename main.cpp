#include <iostream>
#include <unistd.h>
#include "restaurant.h"

using namespace std;

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
            case 'x': s = atoi(optarg); break;
            case 'r': s = atoi(optarg); break;
            case 'g': s = atoi(optarg); break;
            case 'v': s = atoi(optarg); break;
            default:
                cerr << "Usage: " << argv[0]
                     << " [-s numRequests] [-x T-X ms] [-r Rev9 ms] [-g general ms] [-v VIP ms]]" << endl;
                     exit(0);
        }
    }

    SharedState state;
    state.maxRequests = s;

    // Intialize mutex and condition variables
    pthread_mutex_init(&state.mutex, nullptr);
    pthread_cond_init(&state.not_full, nullptr);
    pthread_cond_init(&state.not_empty, nullptr);

    // Before exiting
    pthread_mutex_destroy(&state.mutex);
    pthread_cond_destroy(&state.not_full);
    pthread_cond_destroy(&state.not_empty);
    return 0;
}