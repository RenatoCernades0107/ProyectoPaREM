#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <omp.h>
#include "DFA.hpp"
#include "matplotlibcpp.hpp"

namespace plt = matplotlibcpp;

#define DEBUG

void plot_execution_time(const std::vector<size_t>& num_of_processes, 
                         const std::vector<double>& input_lengths,
                         std::vector<std::vector<double>>& measurements) {
    // Ensure the NUMBER_OF_PROCESSES is a vector of doubles for plotting
    std::vector<double> processes(num_of_processes.begin(), num_of_processes.end());

    // Plotting: Each input length's measurements are plotted with the same x-axis (processes)
    for (size_t i = 0; i < measurements.size(); ++i) {
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(2) << input_lengths[i];
        // Create a label with the input length for each plot
        std::string label = "Length: " + oss.str();

        // Pass the label as part of the plot's keyword arguments
        std::map<std::string, std::string> plot_args;
        plot_args["label"] = label;
        plot_args["marker"] = "o";

        // Plot the data for this input length with the label
        plt::plot(processes, measurements[i], plot_args);
    }

    // Add labels, title, and legend
    plt::xlabel("Number of Processes");
    plt::ylabel("Time (seconds)");
    plt::title("Parallel Execution Time vs Number of Processes");
    plt::legend();
    plt::grid(1);

    // Save the image
    plt::save("parem_exec_time.png");

    // Show the plot
    plt::show();
}

int main() {
    // Initialize the DFA object
    DFA dfa;    
    dfa.read();
    dfa.print();

    // Language: a*bc?
    const std::vector<size_t> NUMBER_OF_PROCESSES = {1, 2, 4, 8, 16 };  // Number of processes to test
    const std::vector<double> INPUT_LENGTHS = { 6.69e+07, 1.34e+08, 2.68e+08, 5.36e+08, 1.07e+09 };  // Multiple input lengths

    // Create a vector to store the time measurements for each input length
    std::vector<std::vector<double>> all_measurements;

    // Iterate over input lengths
    for (const auto n : INPUT_LENGTHS) {
        std::vector<double> measurements;  // Vector to store measurements for each input length

        // Create a string of length 'n' (a*bc?) for testing
        std::string s(n, 'a');  // Initialize the string with 'a's
        s.append("bc");         // Append "bc" to the string

        // Iterate over the different numbers of processes
        for (const auto p : NUMBER_OF_PROCESSES) {
            double t1, t2;
            
            #ifdef _OPENMP
            t1 = omp_get_wtime();  // Start timing
            #endif

            // Run the DFA algorithm with the current number of processes
            bool b = dfa.parallel_rem(&s, p); 

            #ifdef _OPENMP
            t2 = omp_get_wtime();  // End timing
            #endif

            measurements.push_back(t2 - t1); // Store the elapsed time for this configuration
        }

        all_measurements.push_back(measurements);  // Store measurements for the current input length
    }

    plot_execution_time(NUMBER_OF_PROCESSES, INPUT_LENGTHS, all_measurements);
    return 0;
}
