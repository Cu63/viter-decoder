#include "../header/viter.hpp"
#include <iostream>

int main() {
    Viter signal(4);

    signal.readSignal();
    signal.decode();
    signal.printSignalOut();
    return 0;
}
