Restaurant Flow Control
Author: Gilad Bitton
RedID: 130621085
Course: CS 480 — Operating Systems (Fall 2025)
Assignment: A4 — Multithreaded Producer–Consumer Simulation
-------------------------------------------------------------

Overview
-------------------------------------------------------------
This project simulates a restaurant flow control system using the 
producer–consumer model with POSIX threads and monitors 
(mutex + condition variables).

Two greeter robots act as producers generating customer seating 
requests for:
 - General Table requests
 - VIP Room requests

Two concierge robots act as consumers retrieving and processing 
those requests.

The system uses a bounded queue (max 20 total, max 6 VIP requests)
to manage request flow between producers and consumers.
Synchronization ensures safe access to shared data and realistic
concurrent execution.

-------------------------------------------------------------
Features
-------------------------------------------------------------
• Two producer threads:
    - GeneralTable Producer
    - VIPRoom Producer (cannot exceed 6 VIP requests waiting)

• Two consumer threads:
    - T-X Consumer
    - Rev-9 Consumer

• Monitor-based synchronization using:
    - pthread_mutex_t
    - pthread_cond_t

• Simulated processing times via usleep()

• Clean thread termination with broadcast signaling

• Logging and summary reporting via provided helper functions:
    - output_request_added()
    - output_request_removed()
    - output_production_history()

-------------------------------------------------------------
Directory Structure
-------------------------------------------------------------
Flow_Control_Assignment_4/
├── code_files/
│   ├── cpp_files/
│   │   ├── main.cpp
│   │   ├── producer.cpp
│   │   ├── consumer.cpp
│   │   ├── log.cpp
│   ├── header_files/
│   │   ├── restaurant.h
│   │   ├── producer.h
│   │   ├── consumer.h
│   │   ├── seating.h
│   │   ├── log.h
├── object_files/
│   ├── main.o
│   ├── producer.o
│   ├── consumer.o
│   ├── log.o
├── sample_output.txt
├── Makefile
└── dineseating

-------------------------------------------------------------
Compilation Instructions
-------------------------------------------------------------
Make sure you are on a Linux environment with g++ and make installed.

To build:
    make

To clean:
    make clean

This compiles all .cpp files in code_files/cpp_files/ and places
object files in code_files/object_files/, then links them into
the executable "dineseating".

-------------------------------------------------------------
Execution
-------------------------------------------------------------
Run the program with optional arguments to control timing 
and total requests:

Usage:
    ./dineseating [-s numRequests] [-x T-X ms] [-r Rev9 ms] [-g general ms] [-v VIP ms]

Arguments:
    -s N   Total number of seating requests to produce (default 110)
    -x N   Sleep time (ms) for T-X consumer (default 0)
    -r N   Sleep time (ms) for Rev-9 consumer (default 0)
    -g N   Sleep time (ms) for General Table producer (default 0)
    -v N   Sleep time (ms) for VIP Room producer (default 0)

Example runs:
    ./dineseating
    ./dineseating -s 100 -x 20 -r 35 -g 15 -v 10
    ./dineseating -v 3 -g 9 -x 37 -r 40

-------------------------------------------------------------
Sample Output
-------------------------------------------------------------
See "sample_output.txt" for reference output format.

Each line shows the queue state after a request is added or removed, e.g.:

    Request queue: 1 GT + 0 VIP = 1. Added General table request.
    Produced: 1 GT + 0 VIP = 1 in 0.000 s.

At completion, a summary report prints total produced and consumed requests:

    REQUEST REPORT
    ----------------------------------------
    General table request producer generated 62 requests
    VIP room request producer generated 38 requests
    T-X consumed 42 GT + 21 VIP = 63 total
    REV-9 consumed 20 GT + 17 VIP = 37 total
    Elapsed time 1.354 s

-------------------------------------------------------------
Implementation Details
-------------------------------------------------------------
• Mutual exclusion enforced using a pthread_mutex_t in SharedState.
• Producers block when:
    - Queue is full (20 requests total)
    - VIP queue count >= 6
• Consumers block when queue is empty.
• Producers broadcast not_empty on exit to wake waiting consumers.
• Consumers exit cleanly when:
    - All production is complete AND
    - Queue is empty.
• Logging functions are always called inside critical sections
  to preserve output order.

-------------------------------------------------------------
Testing and Validation
-------------------------------------------------------------
• Verified correct queue size limits (20 total, 6 VIP).
• Verified proper interleaving and FIFO order.
• Verified termination in all timing combinations.
• Passes all autograder tests including queue fullness
  and VIP-first scenarios.

-------------------------------------------------------------
Author Statement
-------------------------------------------------------------
This project was implemented individually by Gilad Bitton 
(RedID: 130621085) for CS 480 Operating Systems (Fall 2025).
It follows the SDSU academic honesty policy and was developed 
without unauthorized assistance.
