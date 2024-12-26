//Wissam Almasri
//RedID: 130183310
#ifndef SCHEDULER_H // Include guard to prevent multiple inclusions of this header file.
#define SCHEDULER_H

#include "process.h"
#include <vector>
#include <queue>

class Scheduler {
public:
// Constructor to initialize the scheduler with a smoothing factor alpha and a list of processes.
    Scheduler(double alpha, const std::vector<Process*>& processes);
    ~Scheduler(); // Destructor to clean up allocated resources.
    void simulate();
    bool is_completed() const;// Method to check if the scheduling process is completed.

private:
    double alpha;// Smoothing factor for estimating CPU bursts.
    std::vector<Process*> ready_queue;
    std::queue<Process*> blocked_queue;// Queue to hold processes that are currently blocked.
    std::vector<Process*> completed_processes;// Vector to store completed processes.
    int elapsed_time;// Counter to track the total elapsed time of the simulation.
    bool completed;// Flag to indicate if the scheduling process is completed.

    void sort_ready_queue();// Method to sort the ready queue based on estimated CPU bursts.
    void handle_blocked_queue(int executed_time);// Method to manage processes in the blocked queue.
    double estimate_next_cpu_burst(Process* process) const;// Method to estimate the next CPU burst time for a given process.
    void print_stats() const; // Method to print statistics for all completed processes.

    double get_predicted_cpu_burst(Process* process) const;// Method to get the predicted CPU burst time for a process.
    void update_predicted_cpu_burst(Process* process, int actual_burst);// Method to update the predicted CPU burst time based on actual execution.
};

#endif

//testing