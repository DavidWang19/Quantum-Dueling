/**
 * @author Haorui Wang, Tony Tang
 * @brief Implementation of central simulation of the quantum dueling algorithm.
 */

#include <bits/stdc++.h>
#include "Dueling.h"

// Target probability to achieve.
#define PROB_TH 0.4

using namespace std;

extern int N, M, CHANGE_LIMIT, DEPTH, clusterNum;
extern float maxProb;
static float psi[2 * lim_M + 5][2 * lim_M + 5];
static float psiRef[2 * lim_M + 5][2 * lim_M + 5];
static int N_cnt[2 * lim_M + 5], P;
static int phi[2 * lim_M + 5][2 * lim_M + 5], f[2 * lim_M + 5];
static int gateArr[DEPTH_LIMIT + 1], gateBest[DEPTH_LIMIT + 1];
static random_device rd;
static mt19937 mt(rd());
static vector<int> sol;

extern dataPoint randomData[lim_N];

inline float sqr(float x) { return x * x; }

// Gate G1, implementation based on equations deduced in the paper.
void G1() {
    for (int l = 1; l <= clusterNum; l++) {
        float h = 0;
        for (int s = 1; s <= clusterNum; s++) {
            h += (sqrt(N_cnt[s] * 1.0 / N) * phi[s][l] * psi[s][l]);
        }
        for (int k = 1; k <= clusterNum; k++) {
            psi[k][l] = 2 * sqrt(N_cnt[k] * 1.0 / N) * h - phi[k][l] * psi[k][l];
        }
    }
}

// Gate G2, implementation based on equations deduced in the paper.
void G2() {
    for (int k = 1; k <= clusterNum; k++) {
        float h = 0;
        for (int s = 1; s <= clusterNum; s++) {
            h += (sqrt(N_cnt[s] * 1.0 / N) * phi[s][k] * psi[k][s]);
        }
        for (int l = 1; l <= clusterNum; l++) {
            psi[k][l] = 2 * sqrt(N_cnt[l] * 1.0 / N) * h - phi[l][k] * psi[k][l];
        }
    }
}

// Find the measurement probability of the best solution.
float bestProb() {
    float prob = 0;
    for (int i = 1; i <= clusterNum; i++) {
        prob += sqr(psi[sol[0]][i]);
        if (sol[0] != i) prob += sqr(psi[i][sol[0]]);
    }
    return prob;
}

// Recursive method to generate a series of gates under limitations.
void nextStep(int dep, int remainChange) {
    // Reaches maximum depth for one recursion.
    if (dep == DEPTH) {
        memcpy(psi, psiRef, sizeof(psiRef));
        for (int i = 0; i < DEPTH; i++) {
            if (gateArr[i] == 1) G1(); else G2();
        }
        float prob = bestProb();
        if (prob > maxProb) {
            maxProb = prob;
            memcpy(gateBest, gateArr, sizeof(gateArr));
        }
        return;
    }
    // Limitation of the number of changing the gate from 1 to 2 / 2 to 1.
    if (dep == 0) {
        gateArr[dep] = 1;
        nextStep(dep + 1, remainChange);
        gateArr[dep] = 2;
        nextStep(dep + 1, remainChange);
        return;
    }
    if (gateArr[dep - 1] == 2) {
        if (remainChange > 0) {
            gateArr[dep] = 1;
            nextStep(dep + 1, remainChange - 1);
        }
        gateArr[dep] = 2;
        nextStep(dep + 1, remainChange);
    } else {
        gateArr[dep] = 1;
        nextStep(dep + 1, remainChange);
        if (remainChange > 0) {
            gateArr[dep] = 2;
            nextStep(dep + 1, remainChange - 1);
        }
    }
}

// Initialization of parameters used in the simulation.
void recordParameters() {
    memset(phi, 0, sizeof(phi));
    memset(f, 0, sizeof(f));
    memset(N_cnt, 0, sizeof(N_cnt));
    clusterNum = 1; bool nowSolution = false;
    // Calculation of the number of solution in one cluster.
    N_cnt[1] = 1;
    if (randomData[0].isSolution) {
        nowSolution = true;
        f[1] = 1;
    }
    for (int i = 1; i < N; i++) {
        if (randomData[i].isSolution) {
            if (nowSolution) {
                if (randomData[i].value != randomData[i - 1].value) {
                    clusterNum++;
                    f[clusterNum] = 1;
                }
            } else {
                clusterNum++;
                f[clusterNum] = 1;
            }
            nowSolution = true;
        } else {
            if (nowSolution) {
                nowSolution = false;
                clusterNum++;
            }
        }
        N_cnt[clusterNum]++;
    }
    for (int k = 1; k <= clusterNum; k++) {
        for (int l = 1; l <= clusterNum; l++) {
            if (k < l && f[k] == 1) {
                phi[k][l] = -1;
            } else {
                phi[k][l] = 1;
            }
        }
    }
    sol.clear();
    for (int i = 1; i <= clusterNum; i++) {
        if (f[i]) sol.emplace_back(i);
    }
    printf("DEPTH: %d\nCHANGE_LIMIT: %d\nN: %d, M: %d\n\nData Series:\n", DEPTH, CHANGE_LIMIT, N, M);
    /*for (int i = 1; i <= clusterNum; i++) {
        printf("%d: %d %d\n", i, f[i], N_cnt[i]);
    }*/
}

float resultAll[2 * lim_M + 5];

// Calculate the measurement probabilities of every solution.
void calculateResult() {
    memset(resultAll, 0, sizeof(resultAll));
    for (int k = 1; k <= clusterNum; k++) {
        for (int l = 1; l <= clusterNum; l++) {
            if (f[k] == 1) {
                if (f[l] == 1) {
                    resultAll[min(k, l)] += sqr(psi[k][l]);
                } else {
                    resultAll[k] += sqr(psi[k][l]);
                }
            } else {
                if (f[l] == 1) {
                    resultAll[l] += sqr(psi[k][l]);
                } else {
                    resultAll[min(k, l)] += sqr(psi[k][l]);
                }
            }
        }
    }
}

// Print the probability array.
void printResult() {
    printf("All results: ");
    for (const auto& index : sol) {
        printf("%.4f ", resultAll[index]);
    }
    printf("\n");
}

// Print the best strategies and total gates passed.
bool printBest(int Iter, bool print_prob) {
    bool flag = true;
    memcpy(psi, psiRef, sizeof(psiRef));
    for (int i = 0; i < DEPTH; i++) {
        if (gateBest[i] == 1) {
            G1(); printf("G1 ");
        }
        if (gateBest[i] == 2) {
            G2(); printf("G2 ");
        }
        if (gateBest[i] == 0) break;
        float output = bestProb();
        if (print_prob) printf("%.8f\n",output);
        if (output > PROB_TH) { 
            printf("\n\nTarget probability is achieved after %d Dueling gates!\n", (Iter-1)*DEPTH+i+1);
            flag = false; break;
        }
        calculateResult();
    }
    memcpy(psiRef, psi, sizeof(psi));
    return flag;
}

// Initialization of the state vector.
void initStateVector() {
    for (int k = 1; k <= clusterNum; k++) {
        for (int l = 1; l <= clusterNum; l++) {
            psiRef[k][l] = sqrt(N_cnt[k] * N_cnt[l]) / N;
        }
    }
}