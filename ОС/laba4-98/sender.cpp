#include<stdlib.h>
#include <sstream>
#include "processMessage.h"


int main(int argc, char *argv[]) {
    std::string fileName = argv[1];
    int id = atoi(argv[2]);
    std::fstream fs;
    std::ostringstream name;
    name << "_ready_" << id;
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "fs mutex");
    HANDLE msgSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "msg semaphore");
    HANDLE fileWriteSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "write semaphore");

    HANDLE senderStartEvent = OpenEvent(EVENT_MODIFY_STATE,
                                        FALSE,
                                        &name.str()[0]);
    SetEvent(senderStartEvent);

    while (true) {
        std::cout << "\n\n\nSender " << id << " \n Options: \n" << "1) send message \n 2) exit \n";
        int option;
        std::cin >> option;
        if (option == 2) {
            ReleaseMutex(hMutex);
            break;
        }

        WaitForSingleObject(fileWriteSemaphore, INFINITE);
        WaitForSingleObject(hMutex, INFINITE);

        std::cout << "enter message\n";
        std::string message;
        std::cin >> message;

        fs.open(fileName.c_str(), std::ios::out | std::ios::binary | std::ios::app);
        fs << Message(message);
        fs.close();

        ReleaseMutex(hMutex);
        ReleaseSemaphore(msgSemaphore, 1, NULL);
    }

    CloseHandle(hMutex);
    CloseHandle(msgSemaphore);
    CloseHandle(fileWriteSemaphore);
    CloseHandle(senderStartEvent);
}