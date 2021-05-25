#include "../header/viter.hpp"
#include <iostream>

int main() {
    double T;
    double dt;

    srand(time(0));
    T = 1;
    dt = T / 10;
    Viter system(T, dt);
    for (int i = 0; i < 8; i++) {
        int s;
        short si;

        s = rand() % 4;
        std::cout << "Signal: " << (s >> 1) << (s & 1)<< std::endl;
        si = system.encode(s >> 1, s & 1);
        std::cout << "Code: " << si << std::endl;
        system.demodeling(si);
    }

    return 0;
}
