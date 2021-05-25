#include "../header/viter.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>


Viter::Viter(double T, double dt) {
    k = 4;
    this->T = T;
    this->dt = dt;
    f = 1200;

//    srand(time(NULL));
    //generate signals from 0 to T
    signalVector.resize(8);;
    for (int i = 0; i < signalVector.size(); ++i) {
        for (double t = 0; t < T; t += dt) {
            signalVector[i].push_back(modeling(i, t));
        }
    }

    //calculate phi1 and phi2
    for (double t = 0; t < T; t += dt) {
        phi1.push_back(sqrt(2.0 / T) * cos(2.0 * pi * f * t));
        phi2.push_back(sqrt(2.0 / T) * sin(2.0 * pi * f * t));
    }

    //calculate signals coefficients
    for (int i = 0; i < signalVector.size(); ++i) {
        std::pair<double, double> tmp {0, 0};

        for (int j = 0; j < signalVector[0].size(); ++j) {
            tmp.first += signalVector[i][j] * phi1[j];
            tmp.second += signalVector[i][j] * phi2[j];
        }
        signalCoefficients.push_back(tmp);
    }
}

void Viter::readSignal() {
    short signal;

    std::cout << "Enter signal using 0 and 1: ";
    while (std::cin >> signal && (0 == signal || 1 == signal)) {
        signalIn.push_back(signal);
        codeIn.push_back(encode(signal));
    }
    signalOut.resize(signalIn.size());
}

void Viter::generateSignal(int n) {

    signalIn.resize(n);
    signalOut.resize(n);
    codeIn.resize(n);

    for (int i = 0; i < n; i++) {
        signalIn[i] = abs(rand() % 4);
        std::cout << "Encoder input: " 
            << (signalIn[i] >> 1) << (signalIn[i] & 1) << std::endl;
        codeIn.push_back(encode((signalIn[i] >> 1), (signalIn[i] & 1)));
        std::cout << "Encoder output: " 
            << (codeIn.back() >> 1) << (codeIn.back() & 1) << std::endl;
    }
}

short Viter::calcHammingDistance(short a, short b) {
    if ((a < 0) || (b < 0))
        return -1;
    return ((a >> 1) ^ (b >> 1)) + ((a & 1) ^ (b & 1));
}

//encode: generate 2 bits from 1
short Viter::encode(short a) {
    static short b = 0;
    static short c = 0;
    short tmp;

    tmp = (a ^ c) << 1;
    tmp |= (a ^ b) ^ c;
    c = b;
    b = a;
    return tmp;
}

//generate 3 bits from 2
short Viter::encode(short a, short b) {
    static short c = 0;
    static short d = 0;
    short tmp;

    tmp = (c << 2) | ((a ^ d) << 1) | b;
    d = c;
    c = a;
    return tmp;
}

short Viter::findMin(codeMatrix &s) {
    short min;
    short index;

    min = 127;
    index = -1;
    for (int i = 0; i < k; ++i) {
        if (s[i][s[0].size() - 1] < min) {
            min = s[i][s[0].size() - 1];
            index = i;
        }
    }
    return index;
}

void Viter::decode() {
    codeMatrix state(k, std::vector<short> (codeIn.size()));
    codeMatrix index(k, std::vector<short> (codeIn.size()));


    for (int i = 0; i < k; ++i) {
        state[i][0] = calcHammingDistance(A[i][0], codeIn[0]);
        index[i][0] = 0;
    }

    for (int i = 1; i < state[0].size(); ++i) {
        for (int j = 0; j < k; ++j) {
            nextIndexAndState(state, index, i, j);
        }
    }
    short X;

    X = findMin(state);
    signalOut[signalOut.size() - 1] = X >> 1;
    for (int i = signalOut.size() - 1; i > 0; --i) {
        signalOut[i - 1] = index[X][i] >> 1;
        X = index[X][i];
    }
    /*

    std::cout << "Index:\n";
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < state[i].size(); ++j)
            std::cout << std::setw(2) << index[i][j] << " ";
        std::cout << std::endl;
    }

    std::cout << "\nState:\n";
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < state[i].size(); ++j)
            std::cout << std::setw(2) << state[i][j] << " ";
        std::cout << std::endl;
    }
*/
}

void Viter::nextIndexAndState(codeMatrix &s, codeMatrix &ind, int x, int y) {
    short index;
    short min;

    index = -1;
    min = 127;
    for (int i = 0; i < k; ++i) {
        /*
         std::cout << "From " << i << " to " << y
            << " distance is " << calcHammingDistance(A[y][i], codeIn[x]);
            */
        short tmp;
        if (A[y][i] != -1 && s[i][x - 1] != -1) {
           tmp = s[i][x - 1] + calcHammingDistance(A[y][i], codeIn[x]);
           if (tmp < min) {
               min = tmp;
               index = i;
           }
//           std::cout << ", summary distance is " << tmp;
        }
 //       std::cout << std::endl;
    }
  //  std::cout << "\n";
    ind[y][x] = index;
    s[y][x] = min;
}

void Viter::printSignalOut() {
    std::cout << "Signal out: ";
    for (int i = 0; i < signalOut.size(); ++i)
        std::cout << signalOut[i] << " ";
    std::cout << std::endl;
}

int Viter::cmpInOutSignals() {
    int err;

    err = 0;
    for (int i = 0; i < signalIn.size(); ++i)
        if (signalIn[i] != signalOut[i])
            err++;
    return err;
}

double Viter::modeling(short s, double t) {
    return sqrt(2.0 / T) * cos(2.0 * pi * f * t - signal[s]);
}

std::pair<double, double> Viter::demodeling(short si) {
    std::pair<double, double> tmp {0, 0};

    for (int i = 0; i < signalVector[0].size(); ++i) {
        tmp.first += signalVector[si][i] * phi1[i];
        tmp.second += signalVector[si][i] * phi2[i];
    }
    std::cout << "r1: " << tmp.first << std::endl;
    std::cout << "r2: " << tmp.second << std::endl;
}

/*
std::vector<double> Viter::SNR(std::pair<double> r, int dB) {
    double sigma;

}
*/
