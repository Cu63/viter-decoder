#include "../header/viter.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <random>
#include <cmath>

Viter::Viter(double T, double dt, int dB) {
    k = 4;
    f = 1200;
    this->dB = dB;
    this->T = T;
    this->dt = dt;

    //generate signals from 0 to T
    signalVector.resize(8);;
    for (int i = 0; i < signalVector.size(); ++i) {
        for (double t = 0; t < T; t += dt) {
            signalVector[i].push_back(modeling(i, t));
        }
    }

    //calculate signal energy
    E = 0;
    for (int i = 0; i < signalVector[0].size(); ++i) {
        E += pow(signalVector[0][i], 2);
    }

    //calculate phi1 and phi2
    for (double t = 0; t < T; t += dt) {
        phi1.push_back(sqrt(2.0 / T) * cos(2.0 * pi * f * t));
        phi2.push_back(sqrt(2.0 / T) * sin(2.0 * pi * f * t));
    }

    //calculate signals coefficients
    for (int i = 0; i < signalVector.size(); ++i) {
        std::pair<double, double> tmp {0, 0};

        signalCoefficients.push_back(tmp);
        tmp.first = trapz(signalVector[i], phi1);
        tmp.second = trapz(signalVector[i], phi2);
    }
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

short Viter::decode(std::pair<double, double> r) {
    static short lastState = 0;
    double min;
    int index;
    //save Euclid distance and chosen code
    std::vector<std::pair<double, short>> state(k);

    min = INT_MAX;
    for (int i = 0; i < k; ++i) {
        state[i] = nextState(r, i, lastState);
        if (min > state[i].first && state[i].first >= 0) {
            min = state[i].first;
            index = i;
        }
    }

    lastState = index;
    return (lastState & 0X02) + (state[lastState].second & 1);
}

std::pair<double, short> Viter::nextState(std::pair<double, double> r, 
    short to, short from) {
    std::pair<double, double> s1;
    std::pair<double, double> s2;
    std::pair<double, short> res {-1, -1};

    if (A[to][from] >= 0) {
     //   std::cout << from << "->" << to << std::endl;
        std::pair<double, short> tmp1 {-1, A[to][from]};
        std::pair<double, short> tmp2 {-1, A[to][from] + 1};

        s1 = signalCoefficients[A[to][from]];
        s2 = signalCoefficients[A[to][from] + 1];
        tmp1.first = calcEuclidDistance(s1, r);
        tmp2.first = calcEuclidDistance(s2, r);
//        std::cout << "tmp1: " << tmp1.first << " " << tmp1.second << std::endl;
//        std::cout << "tmp2: " << tmp2.first << " " << tmp2.second << std::endl;

        if (tmp1.first < res.first || res.second == -1)
            res = tmp1;
        if (tmp2.first < res.first || res.second == -1)
            res = tmp2;
    }
    /*
    if (res.second != -1) {
        std::cout << "res: " << res.first << " " << res.second << std::endl;
        std::cout << "Signal: " << (to & 0X02) + (res.second & 1) << std::endl;
    }
    */
    return res;
}

double Viter::modeling(short s, double t) {
    return sqrt(2.0 / T) * cos(2.0 * pi * f * t - signal[s]);
}

std::pair<double, double> Viter::demodeling(short si) {
    std::pair<double, double> tmp {0, 0};
    std::vector<double> r (signalVector[si]);

//    r = SNR(signalVector[si]);
    tmp.first = trapz(r, phi1);
    tmp.second = trapz(r, phi2);

    return tmp;
}

double Viter::calcEuclidDistance(std::pair<double, double> s,
        std::pair<double, double> r) {
    return sqrt(pow((s.first - r.first), 2) + pow((s.second - r.second), 2));
}

std::vector<double> Viter::SNR(std::vector<double> r) {
    double sigma;
    double SNR;
    double N0;
    std::vector<double> tmp (r);
    std::default_random_engine generator;

    SNR = pow(10, (dB / 10));
    N0 = E / SNR; 
    sigma = sqrt(N0 / 2);

    std::normal_distribution<double> distribution(0.0, sigma);
    for (int i = 0; i < r.size(); ++i) {
        double n;
        n = distribution(generator);
        tmp[i] += n;
    }
    return tmp;
}

double Viter::trapz(std::vector<double> s) {
    double res = 0;

    for (int i = 0; i < s.size() - 1; ++i) {
        res += 0.5 * dt * (s[i] + s[i+1]);
    }

    return res;
}

double Viter::trapz(std::vector<double> s, std::vector<double> phi) {
    double res = 0;

    for (int i = 0; i < s.size() - 1; ++i) {
        res += 0.5 * dt * (s[i] * phi[i] + s[i+1] * phi[i+1]);
    }

    return res;
}
