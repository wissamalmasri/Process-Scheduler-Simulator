//Wissam Almasri
//RedID: 130183310
#ifndef PROCESS_H
#define PROCESS_H

#include <vector>

class Process {
public:
    Process(int id, const std::vector<unsigned int>& bursts);  // deubbging here
    void execute_cpu_burst();
    void execute_io_burst();
    bool is_completed() const;
    int get_next_cpu_burst() const;
    int get_next_original_cpu_burst() const;  // Declared
    int get_next_io_burst() const;
    int get_previous_cpu_burst() const; // use this for the previous
    void print_bursts() const;

    int id;
    std::vector<unsigned int> bursts;  // check if int or unsigned for debugging
    std::vector<unsigned int> original_bursts;
    int executed_cpu_bursts;
    int executed_io_bursts;
    int current_burst;
    double predicted_burst;
    int turnaround_time;
    int wait_time;
};

#endif