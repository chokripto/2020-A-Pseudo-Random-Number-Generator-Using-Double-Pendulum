// init_values.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Structure pour retourner les valeurs initiales
typedef struct {
    double l1, l2, m1, m2, theta1, theta2, r;
} InitParams;

// Fonction utilitaire : conversion d'une chaîne en binaire (ASCII)
char* string_to_binary(const char* str, int* len_out) {
    int n = strlen(str);
    int total_bits = n * 8;
    char* bin = (char*)malloc(total_bits);
    for (int i = 0; i < n; ++i) {
        unsigned char c = str[i];
        for (int j = 7; j >= 0; --j) {
            bin[i * 8 + (7 - j)] = (c >> j) & 1;
        }
    }
    *len_out = total_bits;
    return bin;
}

// Fonction d'initialisation (Algorithme 1)
InitParams initialize(const char* key) {
    int n_bin;
    char* K = string_to_binary(key, &n_bin);

    // Assure qu'on a au moins 32 bits (>=4 caractères)
    if (n_bin < 32) {
        fprintf(stderr, "Error: Key too short (<4 chars)\n");
        exit(EXIT_FAILURE);
    }

    int L = n_bin;
    int quarter = L / 4;

    // Extraire k1, k2, k3, k4 (chacun de taille quarter)
    char* k1 = &K[0];
    char* k2 = &K[quarter];
    char* k3 = &K[2 * quarter];
    char* k4 = &K[3 * quarter];

    // Allouer les tableaux de 64 bits
    char L1[64] = {0}, L2[64] = {0}, M1[64] = {0}, M2[64] = {0};
    char T1[64] = {0}, T2[64] = {0}, R[64] = {0};

    // Remplissage cyclique
    int idx = 0;
    for (int i = 0; idx < 64; ++i) {
        int pos = i % quarter;
        L1[idx] = k1[pos] ^ k2[pos];
        L2[idx] = k1[pos] ^ k3[pos];
        M1[idx] = k1[pos] ^ k4[pos];
        M2[idx] = k2[pos] ^ k3[pos];
        T1[idx] = k2[pos] ^ k4[pos];
        T2[idx] = k3[pos] ^ k4[pos];
        R[idx]  = k1[pos]; // Selon ligne 20 de l'algo 1
        idx++;
    }

    // Conversion binaire → entier → réel
    double l1_val = 0, l2_val = 0, m1_val = 0, m2_val = 0;
    double t1_val = 0, t2_val = 0, r_val = 0;

    for (int i = 0; i < 64; ++i) {
        double bit_power = pow(2.0, 63 - i);
        l1_val += L1[i] * bit_power;
        l2_val += L2[i] * bit_power;
        m1_val += M1[i] * bit_power;
        m2_val += M2[i] * bit_power;
        t1_val += T1[i] * bit_power;
        t2_val += T2[i] * bit_power;
        r_val  += R[i]  * bit_power;
    }

    double denom64 = pow(2.0, 63);
    double denom59 = pow(2.0, 59);

    InitParams params;
    params.l1 = l1_val / denom64;
    params.l2 = l2_val / denom64;
    params.m1 = m1_val / denom64;
    params.m2 = m2_val / denom64;
    params.theta1 = (t1_val / denom64) * M_PI;  // [0, π]
    params.theta2 = (t2_val / denom64) * M_PI;  // [0, π]
    params.r = r_val / denom59;                 // [0, ~10^4]

    free(K);
    return params;
}
