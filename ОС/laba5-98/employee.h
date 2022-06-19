#include <ostream>
struct employee {
    int id;
    char name[10];
    double hours;
    void print(std::ostream &out){
        out << "id: " << id
            << "\tname: " << name
            << "\thours: " << hours << std::endl;
    }
};

int employeeComparator(const void* p1, const void* p2){
    return ((employee*)p1)->id - ((employee*)p2)->id;
}