#include <iostream>
#include <thread>
#include <chrono>

template <typename T>
struct result
{
    T sum;                                    // use T type safe. Can use size_t as well.??
    std::chrono::duration<double> time_taken; // duration
};

// Part 1: C++ Parallel Reduction

// 1. Using std::atomic<T> type to ensure adding to it is safe.
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
        T current_sum;
        for (int i = start; i < end; i++)
        {
            // critical section
            current_sum += a[i];
        }
        sum_mutex.lock(); 
        global_sum += current_sum;
        sum_mutex.unlock(); 
    };

    // int section_size = N / num_threads; 
    // int remainder = N % num_threads; 
    size_t blockLengthPerThread = (N + num_threads - 1) / num_threads; //to ensure that the block lengths are evenly distributed among the threads


    for (int i = 0; i < num_threads; i++)
    {
        //int start = i * (N / num_threads);
        int start_index = i * blockLengthPerThread; 
        int end_index = std::min(start_index + blockLengthPerThread, N);  
        vec_of_threads[i] = std::thread(compute_sum, start_index, end_index); 
    }

    // std::cout << "vec_of_threads size: " << vec_of_threads.size() << "\n";

    for (auto &th : vec_of_threads)
    {
        th.join();
    }
    //1 1 1 1 1 
    // end
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

    // std::cout << "sum: " << sum << std::endl;
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

    int N = 5; // 1, 1, 1, 1, 1
    int a[N]; // Initialize array with sample values
    for (int i = 0; i < N; i++) {
        a[i] = 1;
    }
    result<int> resp = parallel_sum_std1(a, N, 10);
    std::cout << "Sum1: " << resp.sum << std::endl;
    std::cout << "Duration1 [seconds]: " << resp.time_taken.count() << std::endl;
    
    result<int> resp2 = parallel_sum_std2(a, N, 10); 
    std::cout << "Sum2: " << resp2.sum << std::endl;
    std::cout << "Duration2 [seconds]: " << resp2.time_taken.count() << std::endl;

    return 0;
}
