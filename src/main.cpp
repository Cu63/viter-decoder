#include "../header/viter.hpp"
#include <iostream>

int main() {
    double T;
    double dt;
    int errMax;

    srand(time(0));
    T = 0.05;
    dt = T / 100;
    errMax = 1;
    std::cout << "Start\n";
    for (int i = 1; i <= 10; i++) {
        Viter system(T, dt, i);
        unsigned long long testN;
        double errCount;

        testN = errCount = 0;
    while (errCount < errMax) {
            int s;
            int r;
            short si;
            std::pair<double, double> ri;

            s = rand() % 4;
//            std::cout << "Signal in: " << s << std::endl;
            si = system.encode(s >> 1, s & 1);
            ri = system.demodeling(si);
            r = system.decode(ri);
//            std::cout << "Code in: " << si;
//            std::cout << "Signal out: " << r << std::endl;
            std::cout << "s = " << s << ", r = " << r << std::endl;
            if (r != s) {
                errCount++;
                std::cout << "Error's num: " << errCount << std::endl;
            }
            testN++;
        }
        std::cout << "For dB " << i << " error probability is "
            << errCount / testN << std::endl;
    }
    return 0;
}
