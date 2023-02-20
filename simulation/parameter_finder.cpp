/**
 * @author Haorui Wang, Letian Tang
 * @brief An assistant tool to find appropriate parameters, such as CHANGE_LIMIT, for producing data in the program "data_generator.cpp".
 */

#include <bits/stdc++.h>
#define OUTPUT_PROBABILITY
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
    for (int i = 0; i < N; i += K) {
        randomData[i].isSolution = true;  // Pick M points as solutions.
    }

    int M_cnt = 0;
    for (int i = 0; i < N; i++) {
        if (randomData[i].isSolution) M_cnt++;
    }
    if (M_cnt!=M) exit(999);  // Ensure solution number is correct.
    
    sort(randomData, randomData + N, [](const dataPoint& a, const dataPoint& b) {
        if (a.value != b.value) return a.value < b.value;
        return a.isSolution;
    });
}

// No data texts are produced here. This is only an assistant tool to find appropriate parameters for other programs.
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