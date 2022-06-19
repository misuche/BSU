#include <iostream>
#include <conio.h>
#include <windows.h>
#include "employee.h"

const std::string pipeName = "\\\\.\\pipe\\name";


void connect(HANDLE hPipe){

    while(true){
        char command[10];
        std::cout << "enter r(to read) or w(to write) command and employee id: \n>";
        std::cin.getline(command, 10, '\n');
        if(std::cin.eof()) {
            return;
        }
        DWORD wBytes;

        if(!WriteFile(hPipe, command, 10, &wBytes, NULL)){
            std::cerr << "failed to send message" << std::endl;
            getch();
            return;
        }

        employee e;
        DWORD rBytes;

        if(ReadFile(hPipe, &e, sizeof(e), &rBytes, NULL))
            std::cerr << "failed to receive answer" << std::endl;

        else{
            if(e.id < 0) {
                std::cerr << "employee not found" << std::endl;
                continue;
            }
            else {
                e.print(std::cout);
                if('w' == command[0]) {
                    std::cout << "enter employee id, name and working hours:\n>" << std::flush;
                    std::cin >> e.id >> e.name >> e.hours;
                    std::cin.ignore(2, '\n');
                    if (WriteFile(hPipe, &e, sizeof (e), &wBytes, NULL))
                        std::cout << "record has been sent" << std::endl;

                    else {
                        std::cerr << "failed to send" << std::endl;
                        getch();
                        break;
                    }
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    HANDLE hReadyEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, argv[1]);
    HANDLE hStartEvent = OpenEvent(SYNCHRONIZE, FALSE, "START_ALL");

    if(NULL == hStartEvent || NULL == hReadyEvent){
        std::cerr << "event error"  << std::endl;
        getch();
        return GetLastError();
    }
    SetEvent(hReadyEvent);
    WaitForSingleObject(hStartEvent, INFINITE);
    std::cout << "process started" << std::endl;

    HANDLE hPipe;
    while (true)
    {
        hPipe = CreateFile(pipeName.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL,
                           OPEN_EXISTING, 0, NULL);
        if (INVALID_HANDLE_VALUE != hPipe) {
            break;
        }
        if(!WaitNamedPipe(pipeName.c_str(), 2000)){
            std::cout << "out of time" << std::endl;
            getch();
            return 0;
        }
    }
    std::cout << "connected to pipe" << std::endl;
    connect(hPipe);
    return 0;
}