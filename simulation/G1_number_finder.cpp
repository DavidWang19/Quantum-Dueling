#include<bits/stdc++.h>
#include "Dueling.h"
using namespace std;

int n, N, M, DEPTH, CHANGE_LIMIT, clusterNum;
dataPoint randomData[lim_N];

void initProblem() {
    int K = ceil(double(N)/M);
    // Notice that the v[i] in the simulation is 0-indexed, but the v[i] in the paper is 1-indexed.
    for (int i = 0; i < N; i++) {
        randomData[i].value = i;  // Set v(x) = x.
        randomData[i].isSolution = false;
    }
    // for (int i = K / 2; i < N; i += K) {
    //     randomData[i].isSolution = true;  // Pick M points as solutions.
    // }

    // int M_cnt = 0;
    // for (int i = 0; i < N; i++) {
    //     if (randomData[i].isSolution) M_cnt++;
    // }
    // if (M_cnt != M) exit(999);  // Ensure solution number is correct.
    srand(time(NULL));
    int M_cnt = M;
    while (M_cnt > 0) {
        int pos = rand() % N;
        if (randomData[pos].isSolution) continue;
        randomData[pos].isSolution = true;
        M_cnt--;
    }
}

bool executeGate(int target, double& lastProb, int& Iter) {
    string gateName;
    if (target == 1) {
        G1();
        gateName = "G1";
    } else {
        G2();
        gateName = "G2";
    }
    Iter++;
    double prob = bestProb();
    printf("%s, %.8lf\n", gateName.c_str(), prob);
    if (prob > lastProb) {
        lastProb = prob;
    } else {
        printf("Total Gate Passed = %d, Final prob = %.8lf\n", Iter, lastProb);
        return true;
    }
    return false;
}

int main() {
    freopen("G1_number_finder.txt", "w", stdout);
    N = 1 << 18;
    M = ceil(sqrt(N));
    initProblem();
    allocateMemory();
    for (int alpha = 1; alpha <= 10; alpha += 2) {
        double lastProb = 0;
        printf("N = %d, M = %d, alpha = %d\n", N, M, alpha);
        recordParameters();
        initStateVector(false);
        bool breakFlag = false;
        int Iter = 0;
        while (true) {
            for (int G1_cnt = 1; G1_cnt <= alpha; G1_cnt++) {
                breakFlag = executeGate(1, lastProb, Iter);
                if (breakFlag) break;
            }
            if (breakFlag) break;
            breakFlag = executeGate(2, lastProb, Iter);
            if (breakFlag) break;
            breakFlag = executeGate(1, lastProb, Iter);
            if (breakFlag) break;
            breakFlag = executeGate(2, lastProb, Iter);
            if (breakFlag) break;
        }
        printf("\n\n\n");
    }
    freeMemory();
    return 0;
}