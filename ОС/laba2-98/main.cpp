#include <iostream>
#include <vector>
#include <Windows.h>

struct array {
    explicit array(const std::vector<int>& arr) : v(arr) {};
    std::vector<int> v;
    int min = 0;
    int max = 0;
    int average = 0;

};

DWORD WINAPI findMinMax(void* arguments) {
    array* arr = (array*)arguments;
    double min = INT_MAX;
    double max = INT_MIN;
    for (int i = 0; i < arr->v.size(); ++i)
    {
        if (max < arr->v[i]) max = arr->v[i];
        if (min > arr->v[i]) min = arr->v[i];
        Sleep(7);
    }
    std::cout << "max value: " << max << "\n";
    std::cout << "min value: " << min << "\n";

    arr->max=max; arr->min=min;
    return 0;
}
DWORD WINAPI average(void* arguments) {
    array* arr = (array*)arguments;

    for (int i = 0; i < arr->v.size(); ++i) {
        arr->average += arr->v[i];
        Sleep(12);
    }
    if(arr->v.size()!=0)arr->average = arr->average / arr->v.size();
    std::cout << "average value: " << arr->average << "\n";
    return 0;
}
void printArray(std::vector<int> v) {
    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << ' ';
    }
    std::cout << std::endl;
}
void createMinMax(array *arr)
{
    HANDLE hThread= CreateThread(NULL, 0, findMinMax, LPVOID(arr), 0, NULL);
    if (hThread == NULL) GetLastError();
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
}
void createAverage(array *arr)
{
    HANDLE hThread=CreateThread(NULL, 0, average, LPVOID(arr), 0, NULL);
    if (hThread == NULL) GetLastError();
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
}
int main() {
    std::cout<<"enter array size\n";
    int n; std::cin>>n;
    std::cout<<"enter array elements\n";
    std::vector<int> v(n);
    for(int i=0;i<n;i++)std::cin>>v[i];

    array* arr= new array(v);
    createMinMax(arr);
    createAverage(arr);

    for (int i = 0; i <v.size(); ++i)
        if (v[i] == arr->max || v[i] == arr->min)
            v[i] = arr->average;

    printArray(v);

    delete arr;
    return 0;
}