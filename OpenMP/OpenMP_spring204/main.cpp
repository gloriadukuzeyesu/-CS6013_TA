#include <iostream>
#include <thread>
#include <chrono>
#include "/opt/homebrew/opt/libomp/include/omp.h"
// #include <omp.h>





template <typename T>
struct result
{
    T sum;                                    // use T type safe. Can use size_t as well.??
    std::chrono::duration<double> time_taken; // duration
};

// Part 1: C++ Parallel Reduction

// 1. Using std::atomic<T> type to ensure adding to it is safe. Each thread is computing the sum of the array
// and add the sums together. Not what we are looking for.
template <typename T>
result<T> parallel_sum_std1(T a[], size_t N, size_t num_threads)
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
    result<T> sol;
    sol.sum = total_sum;
    sol.time_taken = end - start;
    return sol;
}
// 1. Using std::atomic<T> type to ensure adding to it is safe.
// 2. Use a critical section when computing the sum.
// Allow threads to compute partial sums in parallel
// and use one master thread to compute the global sum.

template <typename T>
result<T> parallel_sum_std2(T a[], size_t N, size_t num_threads)
{
    std::vector<std::thread> vec_of_threads(num_threads);
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
    result<T> sol;
    sol.sum = global_sum;
    sol.time_taken = end - start;
    return sol;
}

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
}

/* Part 2: OpenMP Custom Parallel Reduction
The goal is to learn to write multithreaded code with OpenMP in a fashion analogous to C++ threads,
and to build your own OpenMP code to do reductions.
 */
template <typename T>
void parallel_sum_omp1(Ta[], size_t N, size_t num_threads)
{
}

int main()
{
    atomic_behavior_test();

    /*  //    int sum = 0;
     int N = 5;
     int a[N];
     result<int> resp = parallel_sum_std(a, N, 2);
     std::cout << "Sum: " << resp.sum << std::endl;
     std::cout << "Duration [seconds]: " << resp.time_taken.count() << std::endl;
    */

    int N = 2; // 1, 1, 1, 1, 1
    int a[N];  // Initialize array with sample values
    for (int i = 0; i < N; i++)
    {
        a[i] = 1;
    }
    result<int> resp = parallel_sum_std1(a, N, 5);
    std::cout << "Sum1: " << resp.sum << std::endl;
    std::cout << "Duration1 [seconds]: " << resp.time_taken.count() << std::endl;

    result<int> resp2 = parallel_sum_std2(a, N, 5);
    std::cout << "Sum2: " << resp2.sum << std::endl;
    std::cout << "Duration2 [seconds]: " << resp2.time_taken.count() << std::endl;

    return 0;
}
