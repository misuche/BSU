#include<vector>
#include <sstream>
#include "processMessage.h"


HANDLE startSingleProcess(std::string &binFileName, int i) {

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    std::ostringstream ss;
    ss << binFileName << ' ' << i;
    if (!CreateProcess("sender.exe",
                       &ss.str()[0],
                       NULL,
                       NULL,
                       FALSE,
                       CREATE_NEW_CONSOLE,
                       NULL,
                       NULL,
                       &si,
                       &pi))
        GetLastError();

    CloseHandle(pi.hThread);
    return pi.hProcess;
}

std::vector<HANDLE> processesHandles;
HANDLE *startEvents;

std::vector<HANDLE> startSender(int processesCount, std::string &binaryFileName) {
    startEvents = new HANDLE[processesCount];
    for (int i = 0; i < processesCount; i++) {
        std::ostringstream handleName;
        handleName << "_ready_" << i;

        processesHandles.push_back(startSingleProcess(binaryFileName, i));
        startEvents[i] = CreateEvent(
                NULL,
                TRUE,
                FALSE, &handleName.str()[0]);
    }

    return processesHandles;
}

int main() {
    std::cout << "enter binary file name:\n";
    std::string binFileName;
    std::cin >> binFileName;
    std::cout << "\nenter message count:\n";
    int messageCount;
    std::cin >> messageCount;
    std::cout << "\nenter sender count:\n";
    int senderCount;
    std::cin >> senderCount;

    std::fstream fs(binFileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    fs.close();

    startSender(senderCount, binFileName);
    HANDLE hMutex = CreateMutex(NULL, FALSE, "file mutex");
    HANDLE msgSemaphore = CreateSemaphore(NULL, 0, messageCount, "msg emaphore");
    HANDLE fileWriteSemaphore = CreateSemaphore(NULL, messageCount, messageCount, "Write Semaphore");

    WaitForMultipleObjects(senderCount, startEvents, TRUE, INFINITE);

    while (true) {
        std::cout << "Options\n 1) read message \n 2) exit\n";
        int option;
        std::cin >> option;
        if (option == 2) break;

        WaitForSingleObject(msgSemaphore, INFINITE);
        WaitForSingleObject(hMutex, INFINITE);

        fs.open(binFileName.c_str(), std::ios::binary | std::ios::in);
        Message message;
        fs >> message;

        std::cout << "New message : " << message.toString() << std::endl;

        std::vector<Message> allFileMessages;
        while (fs >> message) {
            allFileMessages.push_back(message);
        }
        fs.close();

        fs.open(binFileName.c_str(), std::ios::binary | std::ios::out);
        for (int i = 0; i < allFileMessages.size(); ++i) {
            fs << allFileMessages[i];
        }
        fs.close();

        ReleaseMutex(hMutex);
        ReleaseSemaphore(fileWriteSemaphore, 1, NULL);
    }

    for (int i = 0; i < processesHandles.size(); i++) {
        CloseHandle(processesHandles[i]);
        CloseHandle(startEvents[i]);
    }

    delete[] startEvents;
    CloseHandle(fileWriteSemaphore);
    CloseHandle(msgSemaphore);
    CloseHandle(hMutex);
}