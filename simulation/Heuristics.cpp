/**
 * @author Haorui Wang, LeTian Tang
 * @brief Using heuristic algorithm to find the best strategy of the order of gates for diffrent cases of data in quantum dueling algorithm.
 */

#include<bits/stdc++.h>
// #define SPECIAL_CASE 3  // Choose the specific case to run.
#define MANUAL_DEPTH  // Manually set the depth of the search tree.
// #define HADAMARD_GATE // Initialize the state vectors with Hadamard gate.
// If Hadamard gate is not used, the state vectors are initialized with random values.
// This can be used to test the performance of the algorithm with different initial states.
using namespace std;

constexpr int n = 10, lim_N = (1 << n), lim_M = 200, DEPTH_LIMIT = 50, lim_R = 120;
constexpr int PMAX[4] = {0,3,5,3};

int DEPTH = 7, CHANGE_LIMIT = 5;

float psi[2 * lim_M + 5][2 * lim_M + 5], lastProb, maxProb;
float psiRef[2 * lim_M + 5][2 * lim_M + 5];
int M, N, clusterNum, N_cnt[2 * lim_M + 5], P;
int phi[2 * lim_M + 5][2 * lim_M + 5], f[2 * lim_M + 5];
int gateArr[DEPTH_LIMIT + 1], gateBest[DEPTH_LIMIT + 1];
random_device rd;
mt19937 mt(rd());
vector<int> sol;

inline float sqr(float x) { return x * x; }

// Gate G1
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

// Gate G2
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

// Calculate the probability of the best solution.
float bestProb() {
    float prob = 0;
    for (int i = 1; i <= clusterNum; i++) {
        prob += sqr(psi[sol[0]][i]);
        if (sol[0] != i) prob += sqr(psi[i][sol[0]]);
    }
    return prob;
}

// Calculate the probability of the current state.
void nextStep(int dep, int remainChange) {
    if (dep == DEPTH) {
        memcpy(psi, psiRef, sizeof(psiRef));
// Deprecated code for temination of gate passing if probability starts to decrease.
//        float locBest = 0;
//        int pos = DEPTH;
//        for (int i = 0; i < DEPTH; i++) {
//            if (gateArr[i] == 1) G1(); else G2();
//            float prob = bestProb();
//            if (prob > locBest) {
//                locBest = prob;
//            } else {
//                pos = i;
//                break;
//            }
//        }
//        if (locBest > maxProb) {
//            maxProb = locBest;
//            memcpy(gateBest, gateArr, pos * sizeof(int));
//            if (pos != DEPTH) memset(gateBest + pos, 0, (DEPTH - pos) * sizeof(int));
//        }
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

struct dataPoint {
    int value;
    bool isSolution;
} randomData[lim_N];

// Initialize the problem with different special cases.
void initProblem() {
#if defined(SPECIAL_CASE)
    int K = ceil(double(N)/M);
    // Notice that the v[i] in the simulation is 0-indexed, but the v[i] in the paper is 1-indexed.
    for (int i = 0; i < N; i++) {
        randomData[i].value = i;
        randomData[i].isSolution = false;
    }
#if SPECIAL_CASE == 1
    switch (P) {
        case 1:
            for (int i = 0; i < N; i+=K)
                randomData[i].isSolution = true;
            break;
        case 2:
            for (int i = (K-1)/2; i < N; i+=K)
                randomData[i].isSolution = true;
            break;
        case 3:
            for (int i = (K-1); i < N; i+=K)
                randomData[i].isSolution = true;
            break;
    }
#elif SPECIAL_CASE == 2
    switch (P) {
        case 1:
            M = 4;
            break;
        case 2:
            M = 16;
            break;
        case 3:
            M = 32;
            break;
        case 4:
            M = 86;
            break;
        case 5:
            M = 256;
            break;
    }
    K = ceil(double(N)/M);
    for (int i = 0; i < N; i+=K)
        randomData[i].isSolution = true;
#elif SPECIAL_CASE == 3
    switch (P) {
        case 1:
            for (int i = 1; i <= M; i++)
                randomData[i*i-1].isSolution = true;
            break;
        case 2:
            for (int i=0; i < M; i++)
                randomData[i].isSolution = true;
            break;
        case 3:
            for (int i=1; i <= M; i++)
                randomData[N-i].isSolution = true;
            break;
    }
#endif
    int M_cnt = 0;
    for (int i = 0; i < N; i++)
        if (randomData[i].isSolution) M_cnt++;
    if (M_cnt!=M) exit(999);
#else
    //Generate data points
    uniform_int_distribution<int> F_dist(1, 1000000);
    for (auto& dp : randomData) {
        dp.value = F_dist(mt);
        dp.isSolution = false;
    }
    uniform_int_distribution<int> Sol_dist(0, N - 1);
    for (int i = 1; i <= M; i++) {
        int index = Sol_dist(mt);
        while (randomData[index].isSolution) index = Sol_dist(mt);
        randomData[index].isSolution = true;
    }
#endif
    sort(randomData, randomData + N, [](const dataPoint& a, const dataPoint& b) {
        if (a.value != b.value) return a.value < b.value;
        return a.isSolution;
    });
    //Record the solution related parameters
    memset(phi, 0, sizeof(phi));
    memset(f, 0, sizeof(f));
    memset(N_cnt, 0, sizeof(N_cnt));
    clusterNum = 1; bool nowSolution = false;
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
}

float resultAll[2 * lim_M + 5];

// Calculate the result of the current solution.
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

void printResult() {
    printf("All results: ");
    for (const auto& index : sol) {
        printf("%.4f ", resultAll[index]);
    }
    printf("\n");
}

void initStateVector() {
    #ifndef HADAMARD_GATE
        uniform_real_distribution<double> dist(0, 1);
        float sumSqr = 0;
    #endif
    for (int k = 1; k <= clusterNum; k++) {
        for (int l = 1; l <= clusterNum; l++) {
            #if defined(HADAMARD_GATE)
                psiRef[k][l] = sqrt(N_cnt[k] * N_cnt[l]) / N;
            #else
                psiRef[k][l] = sqrt(N_cnt[k] * N_cnt[l]) * dist(mt);
                sumSqr += sqr(psiRef[k][l]);
            #endif
        }
    }
    #ifndef HADAMARD_GATE
        for (int k = 1; k <= clusterNum; k++) {
            for (int l = 1; l <= clusterNum; l++) {
                psiRef[k][l] /= sqrt(sumSqr);
            }
        }
    #endif
}

// The data texts with filename starting with "Heuristics_Output" are produced here.
int main() {
    cerr << "Do you want output to file? (Y/N)";
    char YN = getchar(); while (YN != 'Y' && YN != 'N') YN = getchar();
    #if defined(SPECIAL_CASE)
        string fileName = "Heuristics_Output_" + to_string(SPECIAL_CASE) + ".txt";
    #else
        string fileName = "Heuristics_Output.txt";
    #endif
    if (YN == 'Y') freopen(fileName.c_str(), "w", stdout);
#ifdef MANUAL_DEPTH
    cerr<<"Please input depth:  ";
    scanf("%d",&DEPTH);
    CHANGE_LIMIT = DEPTH;
#endif
    uniform_int_distribution<int> M_dist(1, lim_M);
    for (P = 1; ; P++) {
#if defined(SPECIAL_CASE)
        M = 16; N = 256;
        if (P > PMAX[SPECIAL_CASE]) break;
#else
        N = 1 << n;
        M = M_dist(mt);
        printf("Problem #%d:\nDEPTH: %d\nCHANGE_LIMIT: %d\nN: %d, M: %d\n\nData Series:\n", P, DEPTH, CHANGE_LIMIT, N, M);
#endif
        initProblem();

        printf("Problem #%d:\nDEPTH: %d\nCHANGE_LIMIT: %d\nN: %d, M: %d\n\nData Series:\n", P, DEPTH, CHANGE_LIMIT, N, M);
        for (int i = 1; i <= clusterNum; i++) {
            printf("%d: %d %d\n", i, f[i], N_cnt[i]);
        }
        initStateVector();
        lastProb = 0;
        for (int Iter = 1;; Iter++) {
            maxProb = 0;
            nextStep(0, CHANGE_LIMIT);
#ifdef SPECIAL_CASE
            if ((Iter-1)*DEPTH > lim_R) break;
#else
            if (maxProb < lastProb) break;
#endif

            printf("\nIteration #%d:\n", Iter);
            memcpy(psi, psiRef, sizeof(psiRef));
            for (int gate : gateBest) {
                if (gate == 1) {
                    G1(); printf("G1 %.8f\n",bestProb());
                }
                if (gate == 2) {
                    G2(); printf("G2 %.8f\n",bestProb());
                }
                if (gate == 0) break;
                calculateResult();
                //printResult();
            }
            lastProb = maxProb;
            memcpy(psiRef, psi, sizeof(psi));
            //system("pause");
        }
        #if !defined(SPECIAL_CASE)
            system("pause");
        #endif
        printf("\n\n");
    }
    return 0;
}