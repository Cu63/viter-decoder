#include "../header/viter.hpp"
#include <iostream>

int main() {
    int errCounter;
    int errMissing;
    int signalsCount;
    double errP;
    int n;
    Viter signal(4, errP);

    errCounter = 0;
    errMissing = 0;
    signalsCount = 0;
    errP = 0.0001; 
    n = 1000;

    std::cout << signal.encode(1, 1) << std::endl;
    /*
//    signal.readSignal();
    while (errCounter < 10000) {
        int f;
        if ((f = signal.generateSignal(n)) != 0) {
            errCounter += f;
            std::cout << errCounter << std::endl;
        }
        signalsCount += n;
        signal.decode();
        errMissing += signal.cmpInOutSignals();
    }
        //signal.printSignalOut();

    std::cout << "Signal generated - " << signalsCount
        << ", errors prob " << errP
        << ", errors - " << errCounter 
        << ", missing - " << errMissing
        << ", missing/signal - " << (double) errMissing / signalsCount << std::endl;
        */
    return 0;
}
