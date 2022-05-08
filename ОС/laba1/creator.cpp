#include <iostream>
#include <fstream>
#include <sstream>
#include "employee.h"


int main(int argc, char *argv[]) {
    std::fstream out(argv[0], std::ios::out | std::ios::binary);
    std::istringstream is(argv[1]);
    int amount;
    is >> amount;
    for (int i = 0; i < amount; ++i) {
        employee e;
        std::cout << "enter id\n";
        std::cin >> e.num;
        std::cout << "enter name\n";
        std::cin >> e.name;
        std::cout << "enter hours\n";
        std::cin >> e.hours;
        out.write((char *) &e, sizeof(employee));
    }
    out.close();
    return 0;
}