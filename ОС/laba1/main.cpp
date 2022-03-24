#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <sstream>
#include <fstream>
#include "employee.h"

using namespace std;
void startCreator(string binFileName, int amount)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    const char* app_name = "Creator.exe";
    stringstream ss;
    ss<<amount;
    string arguments = binFileName + " " + ss.str();
    CreateProcess("Creator.exe", (char*)arguments.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
}
void startReporter(string binFileName, string reportFileName, double salary)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    const char* app_name = "Reporter.exe";
    stringstream ss;
    ss<<salary;
    string arguments = binFileName + " " +reportFileName+" " + ss.str();
    CreateProcess("Reporter.exe", (char*)arguments.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

}
int main() {
    string binFileName; int amount;
    cout << "enter binary file name\n";
    cin >> binFileName;
    cout << "enter amount of employees\n";
    cin >> amount;
    startCreator(binFileName,amount);
    ifstream in(binFileName.c_str(), ios::binary);
    employee e;
    while (in.read((char*)&e, sizeof(employee))) {
        cout << e.num << " " << e.name << " " << e.hours << endl;
    }
    in.close();
    string reportFileName; int salary;
    cout<<"enter report file name\n";
    cin>>reportFileName;
    cout<<"enter salary\n";
    cin>>salary;
    startReporter(binFileName, reportFileName, salary);
    string line;
    in.open(reportFileName.c_str());
    while(getline(in, line))
        cout<<line<<endl;
    system("pause");
    return 0;
}
