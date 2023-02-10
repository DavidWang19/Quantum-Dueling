/**
 * @author Haorui Wang, Tony Tang
 * @brief To find the best strategy of a set of data in quantum dueling algorithm.
 */

#include <bits/stdc++.h>
#define OUTPUT_PROBABILITY
#include "Dueling.h"
using namespace std;

int n, N, M, DEPTH, CHANGE_LIMIT, clusterNum;
dataPoint randomData[lim_N];

void initProblem() {
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
}

// The data texts with filenames start with Heuristics_Output are produced here.
int main() {
    cout<<"This executable helps you find parameters for Quantum Dueling, please record parameters"<<endl;
    uniform_int_distribution<int> M_dist(1, lim_M);
    for (int P = 1; ; P++) {
        cout<<"Input n:";
        scanf("%d",&n);
        cout<<"Input test depth:";
        scanf("%d",&DEPTH);
        cout<<"Input test change limit:";
        scanf("%d",&CHANGE_LIMIT);
        N = 1 << n;
        M = ceil(sqrt(N));
        printf("Problem #%d\n", P);
        initProblem();
        recordParameters();
        initStateVector();
        for (int Iter = 1;; Iter++) {
            maxProb = 0;
            nextStep(0, CHANGE_LIMIT);
            printf("\nIteration #%d:\n", Iter);
            if (!printBest(Iter, true)) break;
        }
        system("pause");
        printf("\n\n");
    }
    return 0;
}