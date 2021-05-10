#ifndef VITER_HPP
#define VITER_HPP

#include <vector>

typedef std::vector<std::vector<short>> codeMatrix;

class Viter {
    private:
        const short A[4][4] = {
            { 0,  3, -1, -1},
            { -1,  -1,  1,  2},
            { 3,  0, -1, -1},
            {-1, -1,  2,  1}
        };


        int k; //states count
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
        short encode(short a);
        void decode();
        void printSignalOut();
//        ~Viter();
};

#endif
