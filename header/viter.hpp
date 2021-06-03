#ifndef VITER_HPP
#define VITER_HPP

#include <vector>
#include <ctime>
#include <cstdlib>
#include <utility>

const double pi = 3.14159265358979323846;
typedef std::vector<std::vector<short>> codeMatrix;

class Viter {
    private:
        //possible ways between states
        const short A[4][4] = {
            { 0,  2, -1, -1},
            {-1, -1,  4,  6},
            { 2,  0, -1, -1},
            {-1, -1,  6,  4}
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
        double E; //energy of signal

        std::vector<std::vector<double>> signalVector;
        std::vector<std::pair<double, double>> signalCoefficients;
        std::vector<double> phi1;
        std::vector<double> phi2;

        double calcEuclidDistance(std::pair<double, double> s,
                std::pair<double, double> r);
        std::pair<double, short> nextState(std::pair<double, double> r,
                short to, short from);
    public:
        Viter(double T, double dt);
        short encode(short a, short b);
        short decode(std::pair<double, double> r);
        std::vector<double> SNR(std::vector<double> r, int dB);

        double modeling(short signal, double t);
        std::pair<double, double> demodeling(short si);
//        ~Viter();
};

#endif
