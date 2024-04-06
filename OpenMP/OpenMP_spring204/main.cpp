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
result<T> parallel_sum_std1(T a[], size_t N, size_t num_threads) {
    std::vector<std::thread> vec_of_threads(num_threads); 
    // start
    const auto start = std::chrono::system_clock::now(); 
    std::atomic<T> total_sum = 0; // atomic variable
    auto f = [&] () {
        for(int i = 0; i < N; i++) {
            // critical section 
            total_sum += a[i]; 
        }
    };

    for(int i = 0; i < num_threads; i++) {
        vec_of_threads[i] = std::thread(f); 
    }
    for( auto &th: vec_of_threads) {
        th.join(); 
    }
    // end
    const auto end = std::chrono::system_clock::now(); 
    result<T> sol; 
    sol.sum = total_sum; 
    sol.time_taken = end - start; 
    return sol; 
}

//2. Use a critical section when computing the sum.
//Allow threads to compute partial sums in parallel
//and use one master thread to compute the global sum.

template <typename T>
result<T> parallel_sum_std2 (T a[], size_t N, size_t num_threads) {
    std::vector<std::thread> vec_of_threads(num_threads); 




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

    std::cout << "sum: " << sum << std::endl;
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

    int N = 5; 
    int a[N];  // Initialize array with sample values
    for(int i = 0; i < N; i++) {
        a[i] = 1; 
    }
    result<int> resp = parallel_sum_std1(a, N, 2); 
    std::cout << "Sum: " << resp.sum << std::endl; 
    std::cout << "Duration [seconds]: " << resp.time_taken.count() << std::endl; 
    return 0; 
}
