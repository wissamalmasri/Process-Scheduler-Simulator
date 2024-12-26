#include "scheduler.h"
#include "log.h"
#include <algorithm>
#include <vector>
#include <queue>
#include <iostream>

// Constructor: Initializes the scheduler with a given alpha value and processes
Scheduler::Scheduler(double alpha, const std::vector<Process*>& processes)
    : alpha(alpha), elapsed_time(0), completed(false) {

    // Create a deep copy of each process and store it in the ready queue
    for (const auto& process : processes) {
        ready_queue.push_back(new Process(*process)); // Ensure each process is a unique copy
    }
}

// Destructor: Cleans up the memory by deleting all processes in the queues
Scheduler::~Scheduler() {
    for (Process* p : ready_queue) delete p;
    while (!blocked_queue.empty()) {
        Process* blocked = blocked_queue.front();
        blocked_queue.pop();
        delete blocked;
    }
    for (Process* p : completed_processes) delete p;
}

// Function to sort the ready queue based on predicted CPU burst times
void Scheduler::sort_ready_queue() {
    std::stable_sort(ready_queue.begin(), ready_queue.end(),
        [this](Process* a, Process* b) {
            return estimate_next_cpu_burst(a) < estimate_next_cpu_burst(b);
        });
}

// Function to estimate the next CPU burst using exponential averaging
double Scheduler::estimate_next_cpu_burst(Process* process) const {
    if (process->predicted_burst == 0) {
        // Calculate the average CPU burst as the initial prediction
        double avg_cpu_burst = 0;
        int cpu_burst_count = 0;
        for (size_t i = 0; i < process->original_bursts.size(); i += 2) {
            avg_cpu_burst += process->original_bursts[i];
            cpu_burst_count++;
        }
        return avg_cpu_burst / cpu_burst_count;
    } else {
        // Use exponential averaging to estimate the next CPU burst
        int actual_burst = process->get_previous_cpu_burst();
        if (actual_burst == -1) return process->predicted_burst;
        return alpha * actual_burst + (1 - alpha) * process->predicted_burst;
    }
}



// Main function that simulates the CPU scheduler
void Scheduler::simulate() {
    while (!ready_queue.empty() || !blocked_queue.empty()) {
        handle_blocked_queue(0);

        if (!ready_queue.empty()) {
            sort_ready_queue();
            Process* current = ready_queue.front();
            ready_queue.erase(ready_queue.begin());

            int cpu_burst = current->get_next_cpu_burst();
            current->execute_cpu_burst();

            // Correctly increment elapsed time after the burst is executed
            elapsed_time += cpu_burst;

            // Check for I/O bursts and handle blocked processes
            handle_blocked_queue(cpu_burst);

            // Log the reason for stopping (either completed or going to I/O)
            ExecutionStopReasonType stop_reason = current->is_completed() ? COMPLETED : ENTER_IO;
            log_cpuburst_execution(current->id, current->executed_cpu_bursts, current->executed_io_bursts, elapsed_time, stop_reason);

            if (current->is_completed()) {
                // Turnaround time is captured when the process completes
                current->turnaround_time = elapsed_time;

                // Wait time is calculated as the difference between turnaround time and total bursts
                current->wait_time = current->turnaround_time - current->executed_cpu_bursts - current->executed_io_bursts;

                completed_processes.push_back(current);
            } else {
                // Process goes to blocked queue if not completed
                blocked_queue.push(current);
            }
        } else if (!blocked_queue.empty()) {
            // When ready queue is empty, execute the next I/O burst from the blocked queue
            int min_io_burst = blocked_queue.front()->get_next_io_burst();
            elapsed_time += min_io_burst;
            handle_blocked_queue(min_io_burst);
        }
    }

    // After simulation ends, print statistics
    print_stats();
    completed = true;
}




// Function to print process statistics
void Scheduler::print_stats() const {
    for (const Process* p : completed_processes) {
        int wait_time = p->turnaround_time - p->executed_cpu_bursts - p->executed_io_bursts;
        log_process_completion(p->id, p->turnaround_time, wait_time);
    }

    for (const Process* p : completed_processes) {
        Process temp_process = *p;
        bool is_cpu_burst = true;
        std::vector<double> estimated_bursts;
        double previous_burst = 0;
        bool first_cpu_burst = true;

        for (size_t i = 0; i < temp_process.original_bursts.size(); ++i) {
            if (is_cpu_burst) {
                if (first_cpu_burst) {
                    double avg_cpu_burst = 0;
                    int cpu_burst_count = 0;
                    for (size_t j = 0; j < temp_process.original_bursts.size(); j += 2) {
                        avg_cpu_burst += temp_process.original_bursts[j];
                        cpu_burst_count++;
                    }
                    double first_estimated_burst = avg_cpu_burst / cpu_burst_count;
                    estimated_bursts.push_back(first_estimated_burst);
                    previous_burst = first_estimated_burst;
                    first_cpu_burst = false;
                } else {
                    double estimated_burst = alpha * temp_process.original_bursts[i-2] + (1 - alpha) * previous_burst;
                    estimated_bursts.push_back(estimated_burst);
                    previous_burst = estimated_burst;
                }
                temp_process.execute_cpu_burst();
            } else {
                estimated_bursts.push_back(static_cast<double>(temp_process.original_bursts[i]));
            }
            is_cpu_burst = !is_cpu_burst;
        }

        // Convert double vector to float before passing to log function
        std::vector<float> float_estimated_bursts(estimated_bursts.begin(), estimated_bursts.end());
        log_process_estimated_bursts(p->id, float_estimated_bursts.data(), float_estimated_bursts.size());
    }
}


// Function to handle blocked processes
void Scheduler::handle_blocked_queue(int executed_time) {
    std::queue<Process*> temp_queue;
    while (!blocked_queue.empty()) {
        Process* blocked = blocked_queue.front();
        blocked_queue.pop();

        int io_burst = blocked->get_next_io_burst();
        if (io_burst <= executed_time) {
            blocked->execute_io_burst();
            if (!blocked->is_completed()) {
                ready_queue.push_back(blocked);
            } else {
                blocked->turnaround_time = elapsed_time;
                completed_processes.push_back(blocked);
            }
        } else {
            blocked->bursts[blocked->current_burst] -= executed_time;
            blocked->executed_io_bursts += executed_time;
            temp_queue.push(blocked);
        }
    }
    blocked_queue = temp_queue;
}

bool Scheduler::is_completed() const {
    return completed;
}



