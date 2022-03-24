#include <iostream>
#include <fstream>
#include <cstdlib>
#include "employee.h"
using namespace std;
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
ifstream in(argv[0], ios::binary);
ofstream out(argv[1]);
out<<"Отчет по файлу "<<argv[0]<<endl;
out<<"id, hours, salary per hour, salary"<<endl;
int salary = atoi(argv[2]);
employee e;
    while (in.read((char*)&e, sizeof(employee))) {
        out << e.num << " " << e.name << " " << e.hours<<" "<< e.hours * salary << endl;
    }
    in.close();
    out.close();
    return 0;
}