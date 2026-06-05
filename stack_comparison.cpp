#include <iostream>
#include <chrono>
#include <stack>
#include "Stack.hpp"
using namespace std;
using namespace std::chrono;

int main() {
  
    stack<int> s2;
    
    auto t2_push = high_resolution_clock::now();
    for (int i = 0; i < 5000; i++) s2.push(i);
    auto e2_push = high_resolution_clock::now();

    auto t2_pop = high_resolution_clock::now();
    for (int i = 0; i < 5000; i++) s2.pop();
    auto e2_pop = high_resolution_clock::now();

  
    Stack<int> s1;
    
    auto t1_push = high_resolution_clock::now();
    for (int i = 0; i < 5000; i++) s1.push(i);
    auto e1_push = high_resolution_clock::now();

    auto t1_pop = high_resolution_clock::now();
    for (int i = 0; i < 5000; i++) s1.pop();
    auto e1_pop = high_resolution_clock::now();

    duration<double, milli> cust_push = e1_push - t1_push, cust_pop = e1_pop - t1_pop;
    duration<double, milli> std_push = e2_push - t2_push, std_pop = e2_pop - t2_pop;

    cout << "BENCHMARK RESULTS (5000 Operations)\n";
 
    cout << "Custom Stack - Push: " << cust_push.count() << " ms\n";
    cout << "Custom Stack - Pop:  " << cust_pop.count() << " ms\n";
    
    cout << "Std Stack    - Push: " << std_push.count() << " ms\n";
    cout << "Std Stack    - Pop:  " << std_pop.count() << " ms\n";
}