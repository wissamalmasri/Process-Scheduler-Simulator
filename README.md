Author
- Wissam Almasri

Process Scheduling Simulation
This project simulates a process scheduling system using exponential averaging for CPU burst prediction.

Features
- Implements exponential averaging to estimate CPU bursts.
- Handles CPU and I/O burst execution with detailed logging.
- Simulates process completion and calculates turnaround and wait times.

Files
- `main.cpp`: Entry point for the program, reads process data, and manages scheduling.
- `process.h` and `process.cpp`: Defines and implements the Process class, including methods for handling
- `scheduler.h` and `scheduler.cpp`: Implements the Scheduler class, managing process queues and sched
- `log.h` and `log.cpp`: Provides utilities for logging process execution, burst estimation, and completion sta
- `Makefile`: Automates the build process.
Usage

Prerequisites
- A C++ compiler supporting C++11 or later.
- An input file containing process bursts.

Compilation
Run the following command to compile the program:
```bash
make
```

Running the Program
Execute the program using:
```bash
./scheduler_simulation [-a <alpha>] <input_file>
```

Options:
- `-a <alpha>`: Sets the alpha value for exponential averaging (default: 0.5). Must be in the range (0.0, 1.0)
- `<input_file>`: Path to the input file containing process burst data.
### Input File Format
The input file should have one line per process, with burst times separated by spaces. Each process must
### Example
```bash
./scheduler_simulation -a 0.7 input.txt
```

Output
The program logs:
1. CPU burst execution details.
2. Estimated bursts using exponential averaging.
3. Turnaround and wait times for each process.
Code Overview

Key Components
- **Process**: Represents a process with CPU and I/O bursts, tracks execution progress.
- **Scheduler**: Manages the ready and blocked queues, handles process scheduling and logging.
- **Logger**: Outputs execution details, burst estimations, and completion statistics.

Logging
Logs include:
- Execution details for CPU bursts.
- Burst estimations based on exponential averaging.
- Turnaround and wait times for each process.
