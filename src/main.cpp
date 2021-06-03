#include "../header/viter.hpp"
#include <iostream>

int main() {
    double T;
    double dt;

    srand(time(0));
    T = 0.05;
    dt = T / 100;
    Viter system(T, dt);
    std::cout << "Start\n";
    while (1) {
//    for (int i = 0; i < 10; i++) {
        int s;
        int r;
        short si;
        std::pair<double, double> ri;

        s = rand() % 4;
//        std::cout << "Signal in: " << (s >> 1) << (s & 1)<< std::endl;
        si = system.encode(s >> 1, s & 1);
        //std::cout << "Code: " << si << std::endl;
        ri = system.demodeling(si);
        r = system.decode(ri);
//        std::cout << "Signal out: " << (r >> 1) << (r & 1) << std::endl;
        if (s != r) {
            std::cout << "Error!\n";
            return 1;
        }
    }

    return 0;
}
