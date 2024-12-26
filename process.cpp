//Wissam Almasri
//RedID: 130183310
#include "process.h"
#include "log.h"
#include <iostream>


Process::Process(int id, const std::vector<unsigned int>& bursts)
    : id(id), bursts(bursts), original_bursts(bursts), executed_cpu_bursts(0),
      executed_io_bursts(0), current_burst(0), predicted_burst(0), turnaround_time(0), wait_time(0) {}

// its the same thing as upove but its easier to read specailly while im debugging
// Constructor to initialize a Process object with an ID and burst times.
/*Process::Process(int id, const std::vector<unsigned int>& bursts)
    : id(id), bursts(bursts), executed_cpu_bursts(0), // Initialize the process ID and burst times; set executed CPU bursts to 0.
      executed_io_bursts(0), // Initialize executed IO bursts to 0.
      current_burst(0), // Initialize the current burst index to 0.
      predicted_burst(0.0), // Initialize the predicted burst time to 0.0.
      turnaround_time(0), // Initialize the turnaround time to 0.
      wait_time(0) {} // Initialize the wait time to 0.
// Method to execute the next CPU burst.   */
void Process::execute_cpu_burst() {
    // Check if the current burst is a CPU burst.
    if (current_burst < static_cast<int>(bursts.size()) && current_burst % 2 == 0) {
        // Add the current CPU burst time to the executed CPU bursts.
        executed_cpu_bursts += bursts[current_burst];
        current_burst++; // Move to the next burst.
    }
}
// Method to execute the next IO burst.
void Process::execute_io_burst() {
    // Check if the current burst is an IO burst.
    if (current_burst < static_cast<int>(bursts.size()) && current_burst % 2 != 0) {
        // Add the current IO burst time to the executed IO bursts.
        executed_io_bursts += bursts[current_burst];
        current_burst++; // Move to the next burst.
    }
}
// Method to check if the process has completed all bursts.
bool Process::is_completed() const {
// Return true if the current burst index is beyond the total number of bursts.
    return current_burst >= static_cast<int>(bursts.size());
}
// Method to retrieve the next CPU burst time.
int Process::get_next_cpu_burst() const {
    return (current_burst < static_cast<int>(bursts.size()) && current_burst % 2 == 0) ? bursts[current_burst] : 0; // Return the burst time if it's a CPU burst, otherwise return 0.
}
// Method to retrieve the next IO burst time.
int Process::get_next_io_burst() const {
    return (current_burst < static_cast<int>(bursts.size()) && current_burst % 2 != 0) ? bursts[current_burst] : 0; // Return the burst time if it's an IO burst, otherwise return 0.
}

int Process::get_previous_cpu_burst() const {
    // The last CPU burst is the one before the current burst, and must be at an even index
    if (current_burst > 0 && (current_burst - 2) >= 0) {
        return bursts[current_burst - 2];
    }
    return -1;
}

// Method to log the burst times of the process.
void Process::print_bursts() const {
    log_process_bursts(id, bursts.data(), bursts.size());// Call the log function to record the process's burst times.
}


int Process::get_next_original_cpu_burst() const {
    if (current_burst < static_cast<int>(original_bursts.size())) {
        return original_bursts[current_burst];// Return the next CPU burst from original_bursts
    }
    return -1;
}

// testingg
// making sure its updating