#include <iostream>
#include <fstream>
#include <cstdlib>
#include "employee.h"



int main(int argc, char *argv[]) {

    std::ifstream in(argv[0], std::ios::binary);
    std::ofstream out(argv[1]);
    out << "Report on file " << argv[0] << ':' << std::endl;
    out << "id, hours, salary per hour, salary" << std::endl;

    double salary = atof(argv[2]);

    employee e;
    while (in.read((char *) &e, sizeof(employee))) {
        out << e.num << " " << e.name << " " << e.hours << " " << e.hours * salary << std::endl;
    }
    in.close();
    out.close();
    return 0;
}