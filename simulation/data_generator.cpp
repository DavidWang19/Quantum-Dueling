/**
 * @author Haorui Wang, Tony Tang
 * @brief To generate complexity data for analysis.
 */

#include <bits/stdc++.h>
#include "Dueling.h"
using namespace std;

int n, N, M, DEPTH, CHANGE_LIMIT, clusterNum;
int nMin, nMax, depth[lim_n + 1], change_limit[lim_n + 1];
dataPoint randomData[lim_N];

void initProblem() {
    int K = ceil(double(N) / M);
    for (int i = 0; i < N; i++) {
        randomData[i].value = i;
        randomData[i].isSolution = false;
    }
    for (int i = 0; i < N; i += K) {
        randomData[i].isSolution = true;
    }
    int M_cnt = 0;
    for (int i = 0; i < N; i++) {
        if (randomData[i].isSolution) M_cnt++;
    }
    if (M_cnt != M) exit(999);
    sort(randomData, randomData + N, [](const dataPoint &a, const dataPoint &b) {
        if (a.value != b.value) return a.value < b.value;
        return a.isSolution;
    });
}

// The data texts with filenames start with Complexity_data are produced here.
int main() {
    cerr << "Do you want output to file? (Y/N) \n";
    char YN = getchar();
    while (YN != 'Y' && YN != 'N') YN = getchar();
    if (YN == 'Y') {
        freopen("Complexity_data.txt", "w", stdout);
    }
    cerr << "Please input the range of n from lower to upper bound:\n";
    scanf("%d%d", &nMin, &nMax);
    cerr << "Please input depth array:\n";
    for (int i = nMin; i <= nMax; i++) {
        scanf("%d", &depth[i]);
    }
    cerr << "Please input change limit array:\n";
    for (int i = nMin; i <= nMax; i++) {
        scanf("%d", &change_limit[i]);
    }
    uniform_int_distribution<int> M_dist(1, lim_M);
    for (n = nMin; n <= nMax; n++) {
        N = 1 << n;
        M = ceil(sqrt(N));
        DEPTH = depth[n];
        CHANGE_LIMIT = change_limit[n];
        printf("Problem #%d\n", n - nMin + 1);
        initProblem();
        recordParameters();
        initStateVector();
        for (int Iter = 1;; Iter++) {
            maxProb = 0;
            nextStep(0, CHANGE_LIMIT);
            printf("\nIteration #%d:\n", Iter);
            if (!printBest(Iter, false)) break;
        }
        printf("\n\n");
    }
    return 0;
}