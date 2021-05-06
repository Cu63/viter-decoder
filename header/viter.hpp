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
        std::vector<short> signalIn;
        std::vector<short> signalOut;
        std::vector<short> codeIn;
        std::vector<short> codeOut;

        short calcHammingDistance(short a, short b);
        short setIndex(codeMatrix &s, codeMatrix &ind, int x, int y);
    public:
        Viter(int k = 4);
        void readSignal();
        short encode(short a);
        void decode();
//        ~Viter();
};

#endif
