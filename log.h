//Wissam Almasri
//RedID: 130183310
#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
#include <stdint.h>
#include <cstddef>
#else
#include <inttypes.h>
#include <stdbool.h>
#endif

typedef enum {
    ENTER_IO,
    QUANTUM_EXPIRED,
    COMPLETED,
} ExecutionStopReasonType;

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
                            ExecutionStopReasonType stopReason);

/**
* @brief Logs the process bursts
*
* @param procID
* @param bursts - 1D array of unsigned integers (changed from int)
*/
void log_process_bursts(unsigned int procID, const unsigned int bursts[], std::size_t numOfBursts); // Changed to const unsigned int*

/**
* @brief Logs the estimated bursts from exponential averaging
*
* @param procID
* @param bursts - 1D array of float numbers
*/
void log_process_estimated_bursts(unsigned int procID, float bursts[], std::size_t numOfBursts);

/**
* @brief Logs the completion of a process
*
* @param procID
* @param completionTime
* @param totalWaitTime - wait time is the time spent in the ready queue
*                      wait time = completionTime - total CPU bursts - total IO bursts
*/
void log_process_completion(unsigned int procID,
                            unsigned int completionTime,
                            unsigned int totalWaitTime);

#endif // LOG_H