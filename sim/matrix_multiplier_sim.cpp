#include "VMatrixMultiplier.h"
#include <verilated.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <atomic>

const int SIZE = 16;  // Increased matrix size
const int NUM_TESTS = 100;  // Number of random tests to run

void tick(VMatrixMultiplier* dut) {
    dut->clock = 0;
    dut->eval();
    dut->clock = 1;
    dut->eval();
}

std::vector<uint16_t> softwareMatrixMultiply(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    std::vector<uint16_t> result(SIZE * SIZE, 0);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                result[i*SIZE + j] += static_cast<uint16_t>(a[i*SIZE + k]) * b[k*SIZE + j];
            }
        }
    }
    return result;
}

bool runTest(VMatrixMultiplier* dut, const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    // Set inputs
    for (int i = 0; i < SIZE*SIZE; i++) {
        dut->io_a_0 = a[i];
        dut->io_b_0 = b[i];
        tick(dut);
    }

    dut->reset = 1;
    tick(dut);
    dut->reset = 0;

    dut->io_start = 1;
    tick(dut);
    dut->io_start = 0;

    int cycles = 0;
    while (!dut->io_done) {
        tick(dut);
        cycles++;
    }

    std::vector<uint16_t> c(SIZE*SIZE);
    for (int i = 0; i < SIZE*SIZE; i++) {
        c[i] = dut->io_c_0;
        tick(dut);
    }

    std::vector<uint16_t> expected = softwareMatrixMultiply(a, b);

    bool correct = (c == expected);
    if (!correct) {
        std::cout << "Matrix multiplication failed. Cycles: " << cycles << std::endl;
    } else {
        std::cout << "Matrix multiplication successful. Cycles: " << cycles << std::endl;
    }

    return correct;
}

std::vector<uint8_t> generateRandomMatrix() {
    std::vector<uint8_t> matrix(SIZE * SIZE);
    for (int i = 0; i < SIZE * SIZE; i++) {
        matrix[i] = rand() % 256;
    }
    return matrix;
}

std::vector<uint8_t> generateEdgeMatrix(uint8_t value) {
    return std::vector<uint8_t>(SIZE * SIZE, value);
}

void runParallelTests(int num_threads) {
    std::vector<std::thread> threads;
    std::atomic<int> successful_tests(0);

    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back([&successful_tests, num_threads]() {
            VMatrixMultiplier* dut = new VMatrixMultiplier{"TOP"};
            for (int j = 0; j < NUM_TESTS / num_threads; j++) {
                auto a = generateRandomMatrix();
                auto b = generateRandomMatrix();
                if (runTest(dut, a, b)) {
                    successful_tests++;
                }
            }
            delete dut;
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Parallel tests completed. " << successful_tests << " out of " << NUM_TESTS << " tests passed." << std::endl;
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    srand(time(NULL));

    VMatrixMultiplier* dut = new VMatrixMultiplier{"TOP"};

    // Random tests
    int successful_tests = 0;
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_TESTS; i++) {
        auto a = generateRandomMatrix();
        auto b = generateRandomMatrix();
        if (runTest(dut, a, b)) {
            successful_tests++;
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << successful_tests << " out of " << NUM_TESTS << " tests passed." << std::endl;
    std::cout << "Total time: " << duration.count() << " ms" << std::endl;

    // Edge case tests
    std::cout << "Running edge case tests..." << std::endl;
    runTest(dut, generateEdgeMatrix(0), generateEdgeMatrix(0));  // All zeros
    runTest(dut, generateEdgeMatrix(255), generateEdgeMatrix(255));  // All max values
    runTest(dut, generateEdgeMatrix(1), generateEdgeMatrix(1));  // All ones

    // Parallel tests
    std::cout << "Running parallel tests..." << std::endl;
    runParallelTests(4);  // Run tests using 4 threads

    dut->final();
    delete dut;
    return 0;
}