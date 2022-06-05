#include <iostream>
#include "windows.h"

struct parameter {
    parameter() : id(-1), arr(NULL), n(-1), termStatus(NULL) {}

    parameter(int threadId, int *arr, int n, HANDLE *termStatus) : id(threadId), arr(arr), n(n),
                                                                   termStatus(termStatus) {}

    int id;
    int *arr;
    int n;
    HANDLE *termStatus;
};

HANDLE *hThreads;
HANDLE hStart;
HANDLE *hStopped;
parameter *params;
CRITICAL_SECTION cs;


DWORD WINAPI marker(void *args) {
    parameter *param = static_cast<parameter *>(args);
    int *arr = param->arr;
    int n = param->n;
    int id = param->id;
    int nMarkedElements = 0;
    int num;
    srand(id);

    WaitForSingleObject(hStart, INFINITE);

    while (true) {
        num = rand() % n;
        EnterCriticalSection(&cs);
        if (arr[num] == 0) {
            Sleep(5);
            arr[num] = id;
            Sleep(5);
            LeaveCriticalSection(&cs);
            ++nMarkedElements;
        } else {
            std::cout << "thread: " << id << ", marked count: " << nMarkedElements
                      << ", failed to mark " << num << std::endl;
            LeaveCriticalSection(&cs);

            SetEvent(hStopped[id - 1]);

            DWORD event = WaitForMultipleObjects(2, param->termStatus, FALSE, INFINITE);
            if (event == WAIT_FAILED)
                GetLastError();
            else if (event - WAIT_OBJECT_0 == 1) break;

        }
    }
    for (int i = 0; i < n; ++i)
        if (arr[i] == id)
            arr[i] = 0;

    return 0;
}

int main() {

    int currentCount;
    int *arr;
    bool *isTerminated;
    std::cout << "enter array size: ";
    int arrSize;
    std::cin >> arrSize;
    std::cout << "enter thread count: ";
    int threadCount;
    std::cin >> threadCount;

    arr = new int[arrSize];

    hThreads = new HANDLE[threadCount];
    params = new parameter[threadCount];
    isTerminated = new bool[threadCount];

    for (int i = 0; i < arrSize; ++i) arr[i] = 0;
    for (int i = 0; i < threadCount; ++i)isTerminated[i] = false;

    if (!hThreads)GetLastError();

    hStart = CreateEvent(NULL, TRUE, FALSE, NULL);
    hStopped = new HANDLE[threadCount];
    for (int i = 0; i < threadCount; i++) {

        params[i] = parameter(i + 1, arr, arrSize, new HANDLE[2]);
        params[i].termStatus[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
        params[i].termStatus[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
        hStopped[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        hThreads[i] = CreateThread(NULL, 0, marker, &params[i], 0, NULL);
        if (!hThreads[i]) {
            GetLastError();
            return -1;
        }
    }
    InitializeCriticalSection(&cs);

    SetEvent(hStart);
    currentCount = threadCount;
    while (currentCount) {
        WaitForMultipleObjects(threadCount, hStopped, TRUE, INFINITE);
        for (int i = 0; i < arrSize; i++)
            std::cout << arr[i] << ' ';
        std::cout << std::endl;

        int terminationID;
        bool termFlag = false;

        while (!termFlag) {
            std::cout << "thread to be terminated: ";
            std::cin >> terminationID;
            if (terminationID < 1 || terminationID > threadCount) {
                std::cout << "wrong index!\n";
                continue;
            }
            terminationID--;
            if (isTerminated[terminationID])
                std::cout << "thread is already terminated!\n";
            else {
                SetEvent(params[terminationID].termStatus[1]);
                WaitForSingleObject(hThreads[terminationID], INFINITE);
                termFlag = true;
                isTerminated[terminationID] = true;
                --currentCount;

            }
        }
        for (int i = 0; i < threadCount; ++i)
            if (!isTerminated[i]) {
                ResetEvent(hStopped[i]);
                SetEvent(params[i].termStatus[0]);
            }

    }

    std::cout << "all threads were terminated" << std::endl;

    DeleteCriticalSection(&cs);
    CloseHandle(hStart);
    for (int i = 0; i < threadCount; i++) {
        CloseHandle(hThreads[i]);
        CloseHandle(hStopped[i]);
    }

    for (int i = 0; i < threadCount; i++)
        delete params[i].termStatus;
    delete[] hThreads;
    delete[] isTerminated;
    delete[] hStopped;
    delete[] params;
    delete[] arr;

    return 0;
}