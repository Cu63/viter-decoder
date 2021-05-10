#include "../header/viter.hpp"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <ctime>


Viter::Viter(int k, double errorProbability) {
    this->k = k;
    errP = errorProbability;
    srand(time(NULL));
}

void Viter::readSignal() {
    short signal;

    std::cout << "Enter signal using 0 and 1: ";
    while (std::cin >> signal && (0 == signal || 1 == signal)) {
        signalIn.push_back(signal);
        codeIn.push_back(encode(signal) ^ error());
    }
    signalOut.resize(signalIn.size());
    /*
    std::cout << "codeIn:\n";
    for (int i = 0; i < codeIn.size(); i++) 
        std::cout << codeIn[i] << " ";
    std::cout << std::endl;
    std::cout << "\ncodes:\n";
    for (int i = 0; i < codeIn.size(); i++)
        std::cout << (codeIn[i] >> 1) << (codeIn[i] & 1) << " ";
    std::cout << "\n";
    */
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
            /*
            short ind;
            ind = setIndex(state, index, i, j);
            index[j][i] = ind;
            state[j][i] = state[ind][i - 1]
                + calcHammingDistance(A[j][ind], codeIn[i]);
                */
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

short Viter::error() {
    int err;
    if ((err = abs(rand())) <= errP * RAND_MAX) {
        std::cout << "Error" << std::endl;
        return err % 3 + 1;
    }
    return 0;
}

void Viter::printSignalOut() {
    std::cout << "Signal out: ";
    for (int i = 0; i < signalOut.size(); ++i)
        std::cout << signalOut[i] << " ";
    std::cout << std::endl;
}

/*
Viter::~Viter() {
}
*/
