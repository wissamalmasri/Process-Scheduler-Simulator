//Wissam Almasri
//RedID: 130183310
#include "scheduler.h"
#include "process.h"
#include "log.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <pthread.h>

// Function to read the input file and parse process bursts
// The file contains the CPU and I/O bursts for multiple processes.
std::vector<Process*> read_input(const std::string& file) {
    std::ifstream infile(file);
    if (!infile.is_open()) {
        std::cerr << "Unable to open " << file << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<Process*> processes;
    int id = 0;

    // Read each line of the input file
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::vector<int> bursts;  // Temporarily store bursts as 'int' to handle negative values
        int burst;  // Temporary variable to hold burst time

        // Parse each burst value from the line
        while (iss >> burst) {
            // Check if burst value is negative or zero
            if (burst <= 0) {
                std::cerr << "A burst number must be bigger than 0" << std::endl;
                exit(1);  // Exit if any burst is invalid check while debugging
            }
            // Add valid burst to the vector
            bursts.push_back(burst);
        }

        // Ensure each process has an odd number of bursts (CPU + I/O bursts, ending with CPU)
        if (bursts.size() % 2 == 0) {
            std::cerr << "There must be an odd number of bursts for each process" << std::endl;
            exit(1);
        }

        // Convert bursts from 'int' to 'unsigned int' for the Process constructor
        std::vector<unsigned int> unsigned_bursts(bursts.begin(), bursts.end());

        // Create a new Process object with 'unsigned int' bursts and add it to the processes vector
        processes.push_back(new Process(id++, unsigned_bursts));
    }
	// Return the vector of processes
    return processes;
}


// Function to be run as the scheduler thread
void* scheduler_thread_function(void* arg) {
    Scheduler* scheduler = static_cast<Scheduler*>(arg);  // Cast argument to Scheduler pointer
    scheduler->simulate();  // Call the scheduler's simulate function to start scheduling
    return nullptr;  // Return nullptr when the thread finishes
}
// make sure that the equation is correct, wissam check your calcuator
// everthing looks correct on paper too
int main(int argc, char* argv[]) {
    double alpha = 0.5;  // Default alpha value for exponential averaging
    std::string input_file;  // Variable to store the input file name
    int opt;  // Variable to store the option parsed from command line

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "a:")) != -1) {  // Handle -a option for alpha
        switch (opt) {
        case 'a':  // If -a option is found
            alpha = std::stod(optarg);  // Convert the argument to a double
            // Ensure alpha is within the range (0.0, 1.0)
            if (alpha <= 0.0 || alpha >= 1.0) {
                std::cerr << "Alpha for exponential averaging must be within (0.0, 1.0)" << std::endl;
                return 1;  // Return an error if the value is out of range
            }
            break;
        default:  // If an invalid option is provided
            std::cerr << "Usage: " << argv[0] << " [-a <alpha>] <input_file>" << std::endl;
            return 1;  // Return an error message and exit
        }
    }

    // Check if an input file is provided after options
    if (optind >= argc) {
        std::cerr << "Expected input file after options" << std::endl;
        return 1;  // Exit with an error if no input file is provided
    }

    // Set the input file name from the command-line argument
    input_file = argv[optind];

    // Read processes from the input file
    std::vector<Process*> processes = read_input(input_file);

    // Print process bursts for debugging purposes
    for (const Process* p : processes) {
        p->print_bursts();  // Call print_bursts method for each process
    }

    // Create the Scheduler object with the specified alpha and list of processes
    Scheduler scheduler(alpha, processes);

    // Create a separate thread to run the scheduler
    pthread_t thread;
    // Create the scheduler thread and check for any error during thread creation
    if (pthread_create(&thread, nullptr, scheduler_thread_function, &scheduler) != 0) {
        std::cerr << "Error creating scheduler thread" << std::endl;
        return 1;  // Exit with an error if thread creation fails
    }

    // Main thread waits for the scheduler to complete using busy waiting (no pthread_join)
    while (!scheduler.is_completed()) {
        // Busy waiting loop, checking if the scheduler has completed
    }

    // Clean up dynamically allocated memory for the processes
    for (Process* p : processes) {
        delete p;  // Free the memory allocated for each process
    }

    return 0;  // Exit the program successfully
}



// testing
//last testing