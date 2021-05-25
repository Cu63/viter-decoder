#ifndef VITER_HPP
#define VITER_HPP

#include <vector>
#include <ctime>
#include <cstdlib>

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
            4 * pi / 4,
            0
        };

        int k; //states count
        double T;
        double dt;
        int f;
        std::vector<short> signalIn;
        std::vector<short> signalOut;
        std::vector<short> codeIn;
        std::vector<std::vector<double>> signalVector;
        std::vector<std::pair<double, double>> signalCoefficients;
        std::vector<double> phi1;
        std::vector<double> phi2;

        short calcHammingDistance(short a, short b);
        void nextIndexAndState(codeMatrix &s, codeMatrix &ind, int x, int y);
        short findMin(codeMatrix &s);
    public:
        Viter(double T, double dt);
        void readSignal();
        void  generateSignal(int n = 16);
        short encode(short a);
        short encode(short a, short b);
        void decode();
        void printSignalOut();
        int cmpInOutSignals();
        std::vector<double> SNR(std::vector<double> r, int dB);

        double modeling(short signal, double t);
        std::pair<double, double> demodeling(short si);
//        ~Viter();
};

#endif
