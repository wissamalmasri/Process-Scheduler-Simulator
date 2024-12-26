//Wissam Almasri
//RedID: 130183310
#include <stdio.h>
#include "log.h"

/* Handle C++ namespaces, ignore if compiled in C
* C++ usually uses this #define to declare the C++ standard.
* It will not be defined if a C compiler is used.
*/
#ifdef __cplusplus
using namespace std;
#endif

/*
* Data section -
*/
/* Names of producer threads and request types */
const char *executionStopReason[] = {"enter io", "quantum expired", "completed"};

/**
* @brief Logs the execution of a CPU burst
*
* @param procID
* @param cpuExecutedTime
* @param ioExecutedTime
* @param totalElapsedTime
* @param stopReason
*/
void log_cpuburst_execution(unsigned int procID,
                            unsigned int cpuExecutedTime,
                            unsigned int ioExecutedTime,
                            unsigned int totalElapsedTime,
                            ExecutionStopReasonType stopReason) {
    // Print according to this format
    // P0: cpu executed = 3, io executed = 0, time elapsed = 3, enter io
    printf("P%d: cpu executed = %d, io executed = %d, time elapsed = %d, %s\n",
           procID, cpuExecutedTime, ioExecutedTime, totalElapsedTime,
           executionStopReason[stopReason]);
}

/**
* @brief Logs the process bursts
*
* @param procID
* @param bursts - 1D array of unsigned integers (changed to const unsigned int)
*/
void log_process_bursts(unsigned int procID, const unsigned int bursts[], size_t numOfBursts) {  // Change here
    printf("P%d: ", procID);
    for (size_t i = 0; i < numOfBursts; i++) {
        // Print integers on one line
        printf("%u ", bursts[i]);  // Use %u for unsigned int
    }
    printf("\n");
}

/**
* @brief Logs the estimated bursts from exponential averaging
*
* @param procID
* @param bursts - 1D array of floating points
*/
void log_process_estimated_bursts(unsigned int procID, float bursts[], size_t numOfBursts) {
    printf("P%d: ", procID);  // Ensure uppercase P
    for (size_t i = 0; i < numOfBursts; i++) {
        printf("%.2f ", bursts[i]);
    }
    printf("\n");
}


/**
* @brief Logs the completion of a process
*
* @param procID
* @param completionTime
* @param totalWaitTime - wait time is the time spent in the ready queue
*/
void log_process_completion(unsigned int procID,
                            unsigned int completionTime,
                            unsigned int totalWaitTime) {
    // Ensure uppercase P
    printf("P%d: turnaround time = %d, wait time = %d\n", procID, completionTime, totalWaitTime);
}