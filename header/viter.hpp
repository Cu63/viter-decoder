#ifndef VITER_HPP
#define VITER_HPP

#include <vector>

const double pi = 3.14159265358979323846;
typedef std::vector<std::vector<short>> codeMatrix;

class Viter {
    private:
        const short A[4][4] = {
            { 0,  3, -1, -1},
            { -1,  -1,  1,  2},
            { 3,  0, -1, -1},
            {-1, -1,  2,  1}
        };

        const double signal[8] = {
            3 * pi / 4,
            7 * pi / 4,
            5 * pi / 4,
            1 * pi / 4,
            6 * pi / 4,
            2 * pi / 4,
            0
        };

        int k; //states count
        int T;
        int f;
        double errP;
        std::vector<short> signalIn;
        std::vector<short> signalOut;
        std::vector<short> codeIn;

        short calcHammingDistance(short a, short b);
        void nextIndexAndState(codeMatrix &s, codeMatrix &ind, int x, int y);
        short findMin(codeMatrix &s);
        short error();
    public:
        Viter(int k = 4, double errP = 10e-4);
        void readSignal();
        int generateSignal(int n = 16);
        short encode(short a);
        short encode(short a, short b);
        void decode();
        void printSignalOut();
        int cmpInOutSignals();

        double modeling(short signal, double t);
//        ~Viter();
};

#endif
