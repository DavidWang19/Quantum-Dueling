/**
 * @author Haorui Wang, Chi Zhang
 * @brief The original simulation code to run quantum dueling. Now a faster and neater one has been developed: Dueling.cpp. This program is aiming at finding the relationship between maximum probabilities and values of M.
 */

#include <bits/stdc++.h>
#define ITER_MAX 80
using namespace std;
constexpr int n = 8, N = (1 << n);
int M = 8;
struct matrix {
    int r, c;
    float** mat;

    // Constructor
    matrix(int R, int C) {
        r = R;
        c = C;
        mat = new float* [r];
        for (int i = 0; i < r; i++) {
            mat[i] = new float[c];
        }
        for (int i = 0; i < r; i++) {
            memset(mat[i], 0, c * sizeof(float));
        }
    }

    // Matrix multiplication
    matrix operator*(const matrix& b) {
        matrix ans(r, b.c);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < b.c; j++) {
                for (int k = 0; k < c; k++) {
                    ans.mat[i][j] = (ans.mat[i][j] + mat[i][k] * b.mat[k][j]);
                }
            }
        }
        return ans;
    }

    // Assign operator
    matrix& operator=(const matrix& b) {
        c = b.c;
        r = b.r;
        for (int i = 0; i < r; i++) {
            memcpy(mat[i], b.mat[i], c * sizeof(float));
        }
        return *this;
    }

    // Print the matrix
    void display() {
        cout << r << " " << c << endl;
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Destructor
    ~matrix() {
        for (int i = 0; i < r; i++) {
            delete[] mat[i];
            mat[i] = nullptr;
        }
        delete[] mat;
        mat = nullptr;
    }
};

struct node {
    int F, id;
    bool isSol;
    // Comparison operator to perform sorting. The order is the order of function value. If a, b have same function value, the one is solution will be put in the first.
    bool operator< (const node& b) {
        if (isSol && b.isSol) {
            return F < b.F;
        }
        if (isSol) return true;
        if (b.isSol) return false;
        return F < b.F;
    }
} sorted[N];

// Dealing with float error.
bool Equal(float a, float b) {
    return fabs(a - b) < 1e-6;
}

// Tensor product of two matrix
matrix prod(matrix a, matrix b) {
    matrix ans(a.r * b.r, a.c * b.c);
    for (size_t i1 = 0; i1 < a.r; i1++) {
        for (size_t i2 = 0; i2 < b.r; i2++) {
            for (size_t j1 = 0; j1 < a.c; j1++) {
                for (size_t j2 = 0; j2 < b.c; j2++) {
                    ans.mat[i1 * b.r + i2][j1 * b.c + j2] = a.mat[i1][j1] * b.mat[i2][j2];
                }
            }
        }
    }
    return ans;
}
//Variables:
int v[N], best;
bool isSolution[N];
matrix A(N* N, N* N), Psi(N* N, 1);

//Print arrays, for solution of the problem a # is printed before the data to help with visualization.
void print_float_array(int Size, float* Array) {
    for (int i = 0; i < Size; i++) {
        printf("%.4f%c", Array[i], " \n"[i + 1 == Size]);
    }
}

void print_int_array(int Size, int* Array) {
    for (int i = 0; i < Size - 1; i++) cout << (isSolution[i] ? "#" : "") << Array[i] << " ";
    cout << (isSolution[Size - 1] ? "#" : "") << Array[Size - 1] << endl;
}

void print_bool_array(int Size, bool* Array) {
    for (int i = 0; i < Size - 1; i++) cout << Array[i] << " ";
    cout << Array[Size - 1] << endl;
}

//Initialize gates other than the oracles
auto bit_dot = [](int i, int j) {
    int count = 0;
    for (int t = n; t; t--, i >>= 1, j >>= 1)
        if ((i & j) & 1) count++;
    if (count & 1) return -1;
        else return 1;
};

void init_gates() {
    for (int i1 = 0; i1 < N; i1++) {
        for (int i2 = 0; i2 < N; i2++) {
            for (int j1 = 0; j1 < N; j1++) {
                for (int j2 = 0; j2 < N; j2++) {
                    A.mat[i1 * N + i2][j1 * N + j2] = (2.0 / N - (i2 == j2)) * (2.0 / N - (i1 == j1)) * (((v[j2] < v[i1] && isSolution[j2]) ^ (v[j1] < v[j2] && isSolution[j1])) * (-2) + 1);
                }
            }
        }
    }
}

//Initialize the state vector.
void init_statevector() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Psi.mat[i * N + j][0] = 1. / N;
        }
    }
}

/* Set up the problem.
 * The problem is described using a bool function isSolution to denote the solution to the problem.
 * The v[i] is a function that we wish to optimize.
 * In the end, the goal is to find a solution to the problem while minimizing the v values.
 *      We hope that it can increase the probability amplitudes of solutions with small v values.
 * This construction is only a rough investigation, not the one that would be needed in actual research.
 * find_best is a helper function to find the best solution, in this implementation, where v(x) = x, it is not required
 */

int find_best() {
    int Answer;
    bool flag = false;
    for (int i = 0; i < N; i++) {
        if (isSolution[i]) {
            if (!flag) Answer = i, flag = true;
            if (v[i] < v[Answer]) Answer = i;
        }
    }
    return Answer;
}

bool init_problem() {
    memset(isSolution, 0, sizeof(isSolution));
    srand(time(0));
    for (int i = 0; i < N; i++) v[i] = i;
    int k = ceil(double(N) / M);
    if (M > 1 && k > double(N) / (M - 1) - 1e-10) return false;
    for (int i = 0; i < N; i++)
        isSolution[i] = !((bool)(i % k));

    int M_cnt = 0;
    for (int i = 0; i < N; i++)
        if (isSolution[i]) M_cnt++;
    if (M_cnt != M) exit(999);

    best = find_best();
    for (int i = 0; i < N; i++) {
        sorted[i].id = i;
        sorted[i].isSol = isSolution[i];
        sorted[i].F = v[i];
    }
    sort(sorted, sorted + N);
    return true;
}

float Results_all[N], Results_1[N], Results_2[N];
float last_prob_c, max_prob_c, last_prob_f, max_prob_f;
int max_loc_c, max_loc_f;

float sqr(float x) { return x * x; }

// The data text with filename "Output_For_All_M" are produced here.
int main() {
    freopen("Output_For_All_M.txt","w",stdout);
    printf("%4s %8s %4s %8s %4s\n", "M","P","r","P'","r'");
    for (M = 1; M <= N; M++) {
        init_statevector();
        if (!init_problem()) continue;
        init_gates();
        last_prob_f = last_prob_c = 0;
        max_loc_c = max_loc_f = 0;

        for (int Iter = 0; Iter <= ITER_MAX; Iter++) {
            // Calculate the probabilities in every iteration.
            memset(Results_all, 0, sizeof(Results_all));
            memset(Results_1, 0, sizeof(Results_1));
            memset(Results_2, 0, sizeof(Results_2));
            for (int i = 0; i < N * N; i++) Results_1[i / N] += Psi.mat[i][0] * Psi.mat[i][0];
            for (int i = 0; i < N * N; i++) Results_2[i % N] += Psi.mat[i][0] * Psi.mat[i][0];
            for (int i = 0; i < N * N; i++) {
                int R1 = i / N, R2 = i % N;
                if (isSolution[R1]==isSolution[R2]) {
                    if (v[R1] < v[R2]) {
                        Results_all[R1] += Psi.mat[i][0] * Psi.mat[i][0];
                    } else {
                        Results_all[R2] += Psi.mat[i][0] * Psi.mat[i][0];
                    }
                }
                else {
                    if (isSolution[R1]) Results_all[R1] += Psi.mat[i][0] * Psi.mat[i][0];
                    if (isSolution[R2]) Results_all[R2] += Psi.mat[i][0] * Psi.mat[i][0];
                }
            }
            if (Results_1[best] < last_prob_f + 1e-6 && !max_loc_f) {
                max_loc_f = Iter - 1;
                max_prob_f = last_prob_f;
            }
            last_prob_f = Results_1[best];
            if (Results_all[best] < last_prob_c + 1e-6 && !max_loc_c) {
                max_loc_c = Iter - 1;
                max_prob_c = last_prob_c;
            }
            if (max_loc_c && max_loc_f) break;
            last_prob_c = Results_all[best];
            Psi = A * Psi;
        }
        printf("%4d %8.5f %4d %8.5f %4d\n", M, max_prob_c, max_loc_c, max_prob_f, max_loc_f);
    }
    return 0;
}
