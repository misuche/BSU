#include <iostream>
#include <tchar.h>
#include <sstream>
#include <fstream>
#include <windows.h>
#include "employee.h"


void startCreator(std::string &binFileName, int amount) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::stringstream ss;
    ss << amount;
    std::string arguments = binFileName + ' ' + ss.str();

    bool flag = CreateProcess("Creator.exe", (char *) arguments.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL,
                              NULL, &si, &pi);
    if (!flag)
        GetLastError();
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}

void startReporter(std::string &binFileName, std::string &reportFileName, double salary) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::stringstream ss;
    ss << salary;
    std::string arguments = binFileName + " " + reportFileName + " " + ss.str();
    bool flag = CreateProcess("Reporter.exe", (char *) arguments.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL,
                              NULL, &si,
                              &pi);
    if (!flag) GetLastError();
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

}

int main() {
    std::string binFileName;
    int amount;
    std::cout << "enter binary file name\n";
    std::cin >> binFileName;
    std::cout << "enter amount of employees\n";
    std::cin >> amount;

    startCreator(binFileName, amount);

    std::ifstream in(binFileName.c_str(), std::ios::binary);
    employee e;
    while (in.read((char *) &e, sizeof(employee))) {
        std::cout << e.num << " " << e.name << " " << e.hours << std::endl;
    }

    in.close();

    std::string reportFileName;
    double salary;
    std::cout << "enter report file name\n";
    std::cin >> reportFileName;
    std::cout << "enter salary\n";
    std::cin >> salary;
    startReporter(binFileName, reportFileName, salary);
    std::string line;
    in.open(reportFileName.c_str());
    while (std::getline(in, line))
        std::cout << line << std::endl;
    system("pause");
    return 0;
}
