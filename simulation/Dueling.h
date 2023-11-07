#ifndef MAIN_CPP_DUELING_H
#define MAIN_CPP_DUELING_H

struct dataPoint {
    int value;
    bool isSolution;
};

inline constexpr int lim_n = 18, lim_N = 1 << lim_n, lim_M = 9000, DEPTH_LIMIT = 50;
inline float maxProb;

inline float sqr(float x);
void allocateMemory();
void freeMemory();
void G1();
void G2();
float bestProb();
void nextStep(int dep, int remainChange) ;
void recordParameters();
void calculateResult();
void printResult();
void printAllResult();
void initStateVector(bool useRef = true);
bool printBest(int Iter, bool print_prob);

#endif //MAIN_CPP_DUELING_H
