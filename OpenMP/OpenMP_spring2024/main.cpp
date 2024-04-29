#include <iostream>
#include <vector>
#include <atomic>
#include "mutex"
#include <thread>
#include <chrono>
#include <omp.h>


/**
 * To make #include <omp.h> work. Make sure you are including the path for omp.h file in the make file.
 * If the red sgriggle continues. Upate the c__pp_properties.json file. 
 * updat the compilerPath to include where to find g++ and intelliSenseMode 
 * in my case this is how it looks like.
 *  "compilerPath": "/opt/homebrew/bin/g++-13",
 * and  "intelliSenseMode": "macos-clang-arm64"
 * in most case the g++ is located in the /usr/bin/g++. 
 * 
 */


// #include "/opt/homebrew/opt/libomp/include/omp.h"

template <typename T>
struct result_summary
{
    T sum;                                    // use T type safe. Can use size_t as well.??
    std::chrono::duration<double> time_taken; // duration
};

// Part 1: C++ Parallel Reduction
/*
// 1. Using std::atomic<T> type to ensure adding to it is safe. Each thread is computing the sum of the array
// and add the sums together. Not what we are looking for.
template <typename T>
result_summary<T> parallel_sum_std1(T a[], size_t N, size_t num_threads)
{
    std::vector<std::thread> vec_of_threads(num_threads);

    // start
    const auto start = std::chrono::system_clock::now();
    std::atomic<T> total_sum = 0; // atomic variable
    auto f = [&]()
    {
        for (int i = 0; i < N; i++)
        {
            // critical section
            total_sum += a[i];
        }
    };

    for (int i = 0; i < num_threads; i++)
    {
        vec_of_threads[i] = std::thread(f);
    }
    for (auto &th : vec_of_threads)
    {
        th.join();
    }
    // end
    const auto end = std::chrono::system_clock::now();
    result_summary<T> sol;
    sol.sum = total_sum;
    sol.time_taken = end - start;
    return sol;
}
// 1. Using std::atomic<T> type to ensure adding to it is safe.
// 2. Use a critical section when computing the sum.
// Allow threads to compute partial sums in parallel
// and use one master thread to compute the global sum. */

template <typename T>
result_summary<T> parallel_sum_std(T a[], size_t N, size_t num_threads)
{
    // std::vector<std::thread> vec_of_threads(num_threads);
    std::mutex sum_mutex;
    // start
    const auto start = std::chrono::system_clock::now();
    std::atomic<T> global_sum = 0;
    auto compute_sum = [&](int start, int end)
    {
        std::atomic<T> current_sum;
        for (int i = start; i < end; i++)
        {
            // critical section
            current_sum += a[i];
        }
        sum_mutex.lock();
        global_sum += current_sum;
        sum_mutex.unlock();
    };

    // int remainder = N % num_threads;
    size_t chunkSize = (N + num_threads - 1) / num_threads; // to ensure that the block lengths are evenly distributed among the threads.
    // divide by threads rounded up.
    // size_t chunkSize = N / num_threads; // won't envely distrube chuncks among the array,

    std::vector<std::thread> vec_of_threads(num_threads);

    for (int i = 0; i < num_threads; i++)
    {
        int start_index = i * chunkSize;
        int end_index = std::min(start_index + chunkSize, N); // in case end_index jumps out of the array size.
        vec_of_threads[i] = std::thread(compute_sum, start_index, end_index);
    }

    for (auto &th : vec_of_threads)
    {
        th.join();
    }

    const auto end = std::chrono::system_clock::now();
    result_summary<T> sol;
    sol.sum = global_sum;
    sol.time_taken = end - start;
    return sol;
}


/*
void atomic_behavior_test()
{
    std::atomic<int> sum = 0;
    auto f = [&sum]()
    {
        for (int i = 0; i < 5; i++)
        {
            sum += 1;
        }
    };

    std::thread t1(f);
    std::thread t2(f);

    t1.join();
    t2.join();

    std::cout << "atomic_behavior_test  SUM: " << sum << std::endl;
} */

/* Part 2: OpenMP Custom Parallel Reduction
The goal is to learn to write multithreaded code with OpenMP in a fashion analogous to C++ threads,
and to build your own OpenMP code to do reductions.
 */
template <typename T>
result_summary<T> parallel_sum_omp1(T a[], size_t N, size_t num_threads)
{
    //   std::atomic<T> global_sum = 0;
    T global_sum = 0;
    size_t chunkSize = (N + num_threads - 1) / num_threads; // chunch size for each thread
    const auto start = std::chrono::system_clock::now();

// num_threads number of threads to create
#pragma omp parallel num_threads(num_threads)
    {
        int threadId = omp_get_thread_num();
        // std::atomic<T> local_sum = 0;
        T local_sum = 0;
        size_t startIndex = threadId * chunkSize;
        size_t endIndex = std::min(startIndex + chunkSize, N);

        for (size_t k = startIndex; k < endIndex; k++)
        {
            local_sum += a[k];
        }

    #pragma omp critical
        global_sum += local_sum;
    }

    const auto end = std::chrono::system_clock::now();
    result_summary<T> result;
    result.sum = global_sum;
    result.time_taken = end - start;
    return result;
}

//  Part 3
// Part 3: OpenMP Built-in Reduction
/* Now write a third function, parallel_sum_omp_builtin, with the same signature,
inputs and outputs as above. Use the OpenMP built-in reduction to compute the sum. */
template <typename T>
result_summary<T> parallel_sum_omp_builtin(T a[], size_t N, size_t num_threads)
{
    T global_Sum = 0;
    auto start_time = std::chrono::high_resolution_clock::now();
    // + initialize globalsum to 0 for every thread
#pragma omp parallel for reduction(+ : global_Sum) num_threads(num_threads)
    for (int k = 0; k < N; k++)
    {
        global_Sum += a[k];
    }
    // at the end all
    // Local copies are reduced into a single value and combined
    // with the original global value when returns to the master  thread.

    auto end_time = std::chrono::high_resolution_clock::now();
    result_summary<T> sol;
    sol.sum = global_Sum;
    sol.time_taken = end_time - start_time;
    return sol;
}

void strongScaling()
{
    int N_values[3] = {1000, 2000, 4000};
    // million elemnt of size
    // make the size array really big to see the scaling

    for (int N : N_values)
    {
        int a[N];
        // putting numbers in the array
        for (int i = 0; i < N; i++)
        {
            a[i] = 1;
        }

        // actual testing fixed N with num_threads changing from 1 - 16
        for (int num_threads = 1; num_threads < 17; num_threads++)
        {
            result_summary<int> sum_part1 = parallel_sum_std(a, N, num_threads);
            result_summary<int> sum_part2 = parallel_sum_omp1(a, N, num_threads);
            result_summary<int> sum_part3 = parallel_sum_omp_builtin(a, N, num_threads);
        }
    }
}
// TODO Weak scaling test

int main()
{
    strongScaling();

    // atomic_behavior_test();

    int N = 100; // 1, 1, 1, 1, 1
    int a[N];    // Initialize array with sample values
    for (int i = 0; i < N; i++)
    {
        a[i] = 2;
    }
    size_t num_threads = 20;

    /*  //  Exercies
     result<int> resp = parallel_sum_std1(a, N, 5);
     std::cout << "Sum: " << resp.sum << std::endl;
     std::cout << "Duration1 [seconds]: " << resp.time_taken.count() << std::endl; */

    // PART 1
    result_summary<int> sum_part1 = parallel_sum_std(a, N, num_threads);
    std::cout << "sum_part1: " << sum_part1.sum << std::endl;
    std::cout << "Duration2 [seconds]: " << sum_part1.time_taken.count() << std::endl;

    // PART 2

    result_summary<int> sum_part2 = parallel_sum_omp1(a, N, num_threads);
    std::cout << "sum_part2: " << sum_part2.sum << std::endl;
    std::cout << "Duration2 [seconds]: " << sum_part2.time_taken.count() << std::endl;

    // PART 2
    result_summary<int> sum_part3 = parallel_sum_omp_builtin(a, N, num_threads);
    std::cout << "sum_part3: " << sum_part3.sum << std::endl;
    std::cout << "Duration2 [seconds]: " << sum_part3.time_taken.count() << std::endl;

    return 0;
}
