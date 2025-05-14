#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <cstdlib>
#include <climits>
#include <vector>

using namespace std;
using namespace std::chrono;

string generate_postcode(mt19937& rng) {
    uniform_int_distribution<int> digit_dist(0, 9);
    string postcode;
    for (int i = 0; i < 6; ++i) {
        postcode += '0' + digit_dist(rng);
    }
    return postcode;
}

unsigned long long generate_value(mt19937& rng) {
    uniform_int_distribution<unsigned long long> value_dist(0, ULLONG_MAX);
    return value_dist(rng);
}

void run_benchmark(int N, ofstream& log) {
    random_device rd;
    mt19937 rng(rd());
    
    ofstream data_file("test_data.txt");
    for (int i = 0; i < N; ++i) {
        data_file << generate_postcode(rng) << '\t' 
                 << generate_value(rng) << '\n';
    }
    data_file.close();

    auto start = high_resolution_clock::now();
    system("./main < test_data.txt > /dev/null 2>&1");
    auto end = high_resolution_clock::now();
    double time = duration_cast<microseconds>(end - start).count() / 1e6;

    remove("test_data.txt");
    log << N << "," << time << "\n";
    cout << "N = " << N << " \tTime = " << time << " sec\n";
}

int main() {
    vector<int> test_sizes = {10, 100, 1000, 10000, 100000, 1000000};
    const int RUNS_PER_TEST = 5;
    
    ofstream result_log("benchmark_results.csv");
    result_log << "N,Time\n";

    for (int N : test_sizes) {
        double total_time = 0;
        cout << "Running benchmark for N = " << N << "..." << endl;
        
        for (int run = 0; run < RUNS_PER_TEST; ++run) {
            ofstream tmp_log("tmp_log");
            auto start = high_resolution_clock::now();
            run_benchmark(N, tmp_log);
            auto end = high_resolution_clock::now();
            total_time += duration_cast<microseconds>(end - start).count() / 1e6;
        }
        
        double avg_time = total_time / RUNS_PER_TEST;
        result_log << N << "," << avg_time << "\n";
    }

    result_log.close();
    cout << "\nBenchmark complete. Results saved to benchmark_results.csv\n";
    return 0;
}