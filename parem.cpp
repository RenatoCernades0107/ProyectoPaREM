#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <omp.h>
#include "DFA.hpp"
#include "matplotlibcpp.hpp"

namespace plt = matplotlibcpp;

void plot_execution_time(const std::vector<size_t> &num_of_processes,
                         const std::vector<double> &input_lengths,
                         std::vector<std::vector<double>> &Tp)
{
    // Ensure the NUMBER_OF_PROCESSES is a vector of doubles for plotting
    std::vector<double> processes(num_of_processes.begin(), num_of_processes.end());

    // Plotting: Each input length's measurements are plotted with the same x-axis (processes)
    for (size_t i = 0; i < Tp.size(); ++i)
    {
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(2) << input_lengths[i];
        // Create a label with the input length for each plot
        std::string label = "Length: " + oss.str();

        // Pass the label as part of the plot's keyword arguments
        std::map<std::string, std::string> plot_args;
        plot_args["label"] = label;
        plot_args["marker"] = "o";

        // Plot the data for this input length with the label
        plt::plot(processes, Tp[i], plot_args);
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
    // plt::show();
}

void plot_speedup(const std::vector<size_t> &num_of_processes,
                  const std::vector<double> &input_lengths,
                  std::vector<std::vector<double>> Tp,
                  std::vector<double> &Ts)
{
    // Ensure the NUMBER_OF_PROCESSES is a vector of doubles for plotting
    std::vector<double> processes(num_of_processes.begin(), num_of_processes.end());
    std::vector<double> ideal_speedup;

    for (int i = 0; i < num_of_processes.size(); ++i)
        ideal_speedup.push_back(num_of_processes[i]);

    std::string label = "Ideal Speedup";

    // Pass the label as part of the plot's keyword arguments
    std::map<std::string, std::string> plot_args;
    plot_args["label"] = label;
    plot_args["marker"] = "o";

    // Plot the data for this input length with the label
    plt::plot(processes, ideal_speedup, plot_args);

    for (int i = 0; i < Tp.size(); ++i)
        for (int j = 0; j < num_of_processes.size(); ++j)
            Tp[i][j] = Ts[i] / Tp[i][j];

    // Plotting: Each input length's measurements are plotted with the same x-axis (processes)
    for (size_t i = 0; i < Tp.size(); ++i)
    {
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(2) << input_lengths[i];
        // Create a label with the input length for each plot
        std::string label = "Length: " + oss.str();

        // Pass the label as part of the plot's keyword arguments
        std::map<std::string, std::string> plot_args;
        plot_args["label"] = label;
        plot_args["marker"] = "o";

        // Plot the data for this input length with the label
        plt::plot(processes, Tp[i], plot_args);
    }

    // Add labels, title, and legend
    plt::xlabel("Number of Processes");
    plt::ylabel("Speedup");
    plt::title("Speedup vs Number of Processes");
    plt::legend();
    plt::grid(1);

    // Save the image
    plt::save("parem_speedup.png");

    // Show the plot
    // plt::show();
}

int main()
{
    // Initialize the DFA object
    DFA dfa;
    dfa.read();
    dfa.print();

    // Language: a*bc?
    const std::vector<size_t> NUMBER_OF_PROCESSES = { 1, 2, 4, 8, 16 };  // Number of processes to test
    const std::vector<double> INPUT_LENGTHS = { 6.69e+07, 1.34e+08, 2.68e+08, 5.36e+08, 1.07e+09 };  // Multiple input lengths

    // Create a vector to store the parallel time measurements for each input length
    std::vector<std::vector<double>> Tp;
    // Sequential time
    std::vector<double> Ts;

    // Iterate over input lengths
    for (const auto n : INPUT_LENGTHS)
    {
        std::vector<double> measurements; // Vector to store measurements for each input length
        double t1, t2;

        // Create a string of length 'n' (a*bc?) for testing
        std::string *s = new std::string(n, 'a'); // Initialize the string with 'a's
        s->push_back('b');

        cout << "Sequential (n=" << n << "):" << endl;
        t1 = omp_get_wtime(); // Start timing
        bool b = dfa.exec(s);
        t2 = omp_get_wtime(); // End timing

        if (b) {
            cout << "accepted" << endl;            
        } else {
            cout << "rejected" << endl;
        }

        Ts.push_back(t2 - t1); // Store the sequential time

        // Iterate over the different numbers of processes
        
        cout << "Parallel (n=" << n << "):" << endl;
        for (const auto p : NUMBER_OF_PROCESSES)
        {
            cout << "For p=" << p << ":" <<  endl; 
#ifdef _OPENMP
            t1 = omp_get_wtime(); // Start timing
#endif

            // Run the DFA algorithm with the current number of processes
            bool b = dfa.parallel_rem(s, p);

#ifdef _OPENMP
            t2 = omp_get_wtime(); // End timing
#endif

            if (b) {
                cout << "accepted" << endl;            
            } else {
                cout << "rejected" << endl;
            }

            measurements.push_back(t2 - t1); // Store the elapsed time for this configuration
        }

        Tp.push_back(measurements); // Store measurements for the current input length
    }

    plot_execution_time(NUMBER_OF_PROCESSES, INPUT_LENGTHS, Tp);
    plot_speedup(NUMBER_OF_PROCESSES, INPUT_LENGTHS, Tp, Ts);

    return 0;
}
