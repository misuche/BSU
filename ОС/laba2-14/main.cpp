#include <iostream>
#include <thread>
#include<vector>
#include <chrono>

struct result {
    double min;
    double max;
    double average;
};

void findMinMax(std::vector<int> v, result &r) {

    double min = INT_MAX;
    double max = INT_MIN;
    for (int i :v)
    {
        if (max < i) max = i;
        if (min > i) min = i;
        std::this_thread::sleep_for(std::chrono::milliseconds (7));
    }
    std::cout << "max value: " << max << "\n";
    std::cout << "min value: " << min << "\n";

    r.max=max; r.min=min;
}
void findAverage(std::vector<int> v, result &r) {
    for (int i : v) {
        r.average += i;
        std::this_thread::sleep_for(std::chrono::milliseconds (12));
    }
    if(v.size()!=0)r.average = r.average / v.size();
    std::cout << "average value: " << r.average << "\n";
}
void createMinMax(std::vector<int> v, result &r) {
    std::thread thread(findMinMax,std::ref(v),std::ref(r));
    thread.join();
}
void createAverage(std::vector<int> v, result &r) {
    std::thread thread(findAverage,std::ref(v),std::ref(r));
    thread.join();
}
void printArray(std::vector<int> v) {
    for (int i:v) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}
int main() {
    std::cout << "enter array size\n";
    int n;
    std::cin >> n;
    std::vector<int> v(n);
    std::cout<<"enter array elements\n";
    for(int i=0;i<n;i++)std::cin>>v[i];
    printArray(v);
    result res;
    createMinMax(v,res);
    createAverage(v,res);
    for(int i=0;i<n;i++)
        if(v[i]==res.max||v[i]==res.min)
            v[i]=res.average;
    printArray(v);
    return 0;
}
