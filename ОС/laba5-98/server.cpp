#include <iostream>
#include <conio.h>
#include <fstream>
#include <time.h>
#include <algorithm>
#include <process.h>
#include <windows.h>
#include "employee.h"

int employeeCount;
employee* employees;
HANDLE* hReady;
bool *modified;
const char pipeName[30] = "\\\\.\\pipe\\name";

void sortEmployees(){
    qsort(employees, employeeCount, sizeof(employee), employeeComparator);
}

void writeToFile(char filename[50]){
    std::fstream fin(filename, std::ios::binary | std::ios::out);
    fin.write(reinterpret_cast<char*>(employees), sizeof(employee) * employeeCount);
    fin.close();
}

void readData(){
    employees = new employee[employeeCount];
    std::cout << "enter employee id, name and working hours:" << std::endl;
    for(int i = 1; i <= employeeCount; ++i){
        std::cout  << i << ")";
        std::cin >> employees[i - 1].id >> employees[i - 1].name >> employees[i - 1].hours;
    }
}

employee* findEmployee(int id){
    employee key;
    key.id = id;
    return (employee*) bsearch((const char *) (&key), (const char *) (employees), employeeCount, sizeof(employee),
                               employeeComparator);
}

void startPocesses(int count){
    char buff[10];
    for(int i = 0; i < count; i++) {
        char cmdargs[80] = "..\\cmake-build-debug\\Client.exe ";
        char eventName[50] = "READY_EVENT_";
        itoa(i + 1, buff, 10);
        strcat(eventName, buff);
        strcat(cmdargs, eventName);
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        hReady[i] = CreateEvent(NULL, TRUE, FALSE, eventName);
        if (!CreateProcess(NULL, cmdargs, NULL, NULL, FALSE, CREATE_NEW_CONSOLE,
                           NULL, NULL, &si, &pi)) {
            printf("creation error.\n");
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
}
CRITICAL_SECTION cs;
DWORD WINAPI connect(LPVOID handle){
    HANDLE hPipe = (HANDLE)hPipe;
    employee* errorE = new employee;
    errorE->id = -1;
    while(true){
        DWORD rBytes;
        char message[10];
        bool isRead = ReadFile(handle, message, 10, &rBytes, NULL);
        if(!isRead){
            if(ERROR_BROKEN_PIPE == GetLastError()){
                std::cout << "client disconnected." << std::endl;
                break;
            }
            else {
                std::cerr << "failed to read message." << std::endl;
                break;
            }
        }

        if(strlen(message) > 0) {
            char command = message[0];
            message[0] = ' ';
            int id = atoi(message);
            DWORD wBytes;
            EnterCriticalSection(&cs);
            employee* sendE = findEmployee(id);
            LeaveCriticalSection(&cs);
            if(NULL == sendE){
                sendE = errorE;
            }
            else{
                int ind = sendE - employees;
                if(modified[ind])
                    sendE = errorE;
                else{
                    switch (command) {
                        case 'w':
                            printf("requested to modify id %d", id);
                            modified[ind] = true;
                            break;
                        case 'r':
                            printf("requested to read id %d", id);
                            break;
                        default:
                            std::cout << "unknown request";
                            sendE = errorE;
                    }
                }
            }
            bool isSent = WriteFile(handle, sendE, sizeof(employee), &wBytes, NULL);
            if(isSent) std::cout << "sent answer" << std::endl;
            else std::cout << "failed to send answer" << std::endl;

            if('w' == command && sendE != errorE){
                isRead = ReadFile(handle, sendE, sizeof(employee), &rBytes, NULL);
                if(isRead){
                    std::cout << "modified record" << std::endl;
                    modified[sendE - employees] = false;
                    EnterCriticalSection(&cs);
                    sortEmployees();
                    LeaveCriticalSection(&cs);
                }
                else{
                    std::cerr << "failed to read message" << std::endl;
                    break;
                }
            }
        }
    }
    FlushFileBuffers(handle);
    DisconnectNamedPipe(handle);
    CloseHandle(handle);
    delete errorE;
    return 0;
}

void openPipes(int clientCount){
    HANDLE hPipe;
    HANDLE* hThreads = new HANDLE[clientCount];
    for(int i = 0; i < clientCount; i++){
        hPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX,
                                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                PIPE_UNLIMITED_INSTANCES,0, 0,INFINITE, NULL);
        if (INVALID_HANDLE_VALUE == hPipe){
            std::cerr << "failed to create pipe" << std::endl;
            getch();
            return;
        }
        if(!ConnectNamedPipe(hPipe, NULL)){
            std::cout << "no connected clients" << std::endl;
            break;
        }
        hThreads[i] = CreateThread(NULL, 0, connect, (LPVOID) hPipe, 0, NULL);
    }
    std::cout << "clients connected to pipe" << std::endl;
    WaitForMultipleObjects(clientCount, hThreads, TRUE, INFINITE);
    std::cout << "all clients disconnected" << std::endl;
    delete[] hThreads;
}

int main() {
    char fileName[50];
    std::cout << "enter file name and employee count\n>";
    std::cin >> fileName >> employeeCount;
    readData();
    writeToFile(fileName);
    sortEmployees();

    InitializeCriticalSection(&cs);
    srand(time(0));
    int clientCount = 2 + rand() % 3; //from 2 to 4
    HANDLE hstartALL = CreateEvent(NULL, TRUE, FALSE, "START_ALL");
    modified = new bool[employeeCount];
    for(int i = 0; i < employeeCount; ++i)
        modified[i] = false;
    hReady = new HANDLE[clientCount];
    startPocesses(clientCount);
    WaitForMultipleObjects(clientCount, hReady, TRUE, INFINITE);
    std::cout << "all processes are ready" << std::endl;
    SetEvent(hstartALL);

    openPipes(clientCount);
    for(int i = 0; i < employeeCount; i++)
        employees[i].print(std::cout);
    std::cout << "press any key to exit ..." << std::endl;
    getch();
    DeleteCriticalSection(&cs);
    delete[] modified;
    delete[] hReady;
    delete[] employees;
    return 0;
}