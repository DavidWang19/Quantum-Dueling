/**
 * @author Haorui Wang, LeTian Tang
 * @brief Using heuristic algorithm to find the best strategy of the order of gates for a specific kind of data with different N in quantum dueling algorithm.
 */

#include<bits/stdc++.h>
#define MANUAL_DEPTH  // Manually set the depth of the search tree.
#define PROB_TH 0.4  // The target probability.
using namespace std;

constexpr int lim_N = 1<<15, lim_M = 256, DEPTH_LIMIT = 50, PMAX = 3, n_array[PMAX + 1] = {0,8,10,12};

int DEPTH = 7, CHANGE_LIMIT = 5;

float psi[2 * lim_M + 5][2 * lim_M + 5], maxProb;
float psiRef[2 * lim_M + 5][2 * lim_M + 5];
int n, N, M, clusterNum, N_cnt[2 * lim_M + 5], P;
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

// Calculate the probability of the best solution
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

void initProblem() {
    // The same as other simulation programs.
    int K = ceil(double(N)/M);
    for (int i = 0; i < N; i++) {
        randomData[i].value = i;
        randomData[i].isSolution = false;
    }
    for (int i = 0; i < N; i+=K)
        randomData[i].isSolution = true;

    int M_cnt = 0;
    for (int i = 0; i < N; i++)
        if (randomData[i].isSolution) M_cnt++;
    if (M_cnt!=M) exit(999);
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
    for (int k = 1; k <= clusterNum; k++) {
        for (int l = 1; l <= clusterNum; l++) {
            psiRef[k][l] = sqrt(N_cnt[k] * N_cnt[l]) / N;
        }
    }
}

// The data texts with filename "Heuristics_Output_for_all_N-18.txt" are produced here.
int main() {
    cerr << "Do you want output to file? (Y/N)";
    char YN = getchar();
    while (YN != 'Y' && YN != 'N') YN = getchar();
    if (YN == 'Y') {
        freopen("Heuristics_Output_for_all_N.txt", "w", stdout);
    }
#ifdef MANUAL_DEPTH
    cerr<<"Please input depth:  ";
    scanf("%d",&DEPTH);
    CHANGE_LIMIT = DEPTH;
#endif
    uniform_int_distribution<int> M_dist(1, lim_M);
    for (int P = 1; P <= PMAX; P++) {
        n = n_array[P];
        N = 1 << n;
        M = ceil(sqrt(N));
        initProblem();
        printf("Problem #%d:\nDEPTH: %d\nCHANGE_LIMIT: %d\nN: %d, M: %d\n\nData Series:\n", P, DEPTH, CHANGE_LIMIT, N, M);
        for (int i = 1; i <= clusterNum; i++) {
            printf("%d: %d %d\n", i, f[i], N_cnt[i]);
        }
        initStateVector();
        for (int Iter = 1;; Iter++) {
            bool flag = true;
            maxProb = 0;
            nextStep(0, CHANGE_LIMIT);
            printf("\nIteration #%d:\n", Iter);
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
                printf("%.8f\n",output);
                if(output > PROB_TH ) { printf("\n\n Target probability is achieved after %d Dueling gates!\n", (Iter-1)*DEPTH+i+1); flag = false; break; }
                calculateResult();
            }
            if (!flag) break;
            memcpy(psiRef, psi, sizeof(psi));
        }
        //system("pause");
        printf("\n\n");
    }
    return 0;
}