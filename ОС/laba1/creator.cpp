#include <iostream>
#include <fstream>
#include <sstream>
#include "employee.h"
using namespace std;
int main(int argc, char* argv[]) {
    fstream out(argv[0], ios::out | ios::binary);//создаем бинарный файл
    istringstream is(argv[1]);
    int amount;
    is>>amount;
    for (int i = 0; i < amount; ++i) {
        employee e;
        cout<<"enter id\n";
        cin>>e.num;
        cout<<"enter name\n";
        cin>>e.name;
        cout<<"enter hours\n";
        cin>>e.hours;
        out.write((char*)&e,sizeof(employee));
    }
    out.close();
    return 0;
}