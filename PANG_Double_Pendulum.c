// prng_double_pendulum.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>  // pour uint32_t
#include "init_values.h"

// Équations d'accélération du pendule double (équations 9 et 10)
void compute_accelerations(double theta1, double theta2,
                           double dtheta1, double dtheta2,
                           double l1, double l2,
                           double m1, double m2,
                           double g,
                           double* d2theta1, double* d2theta2) {
    double delta = theta1 - theta2;
    double den1 = 2 * m1 + m2 - m2 * cos(2 * delta);
    double den = l1 * den1;

    double term1 = -g * (2 * m1 + m2) * sin(theta1);
    double term2 = -m2 * g * sin(theta1 - 2 * theta2);
    double term3 = -2 * sin(delta) * m2 * (dtheta2 * dtheta2 * l2 + dtheta1 * dtheta1 * l1 * cos(delta));

    *d2theta1 = (term1 + term2 + term3) / den;

    double den2 = l2 * den1;
    double term4 = 2 * sin(delta);
    double term5 = dtheta1 * dtheta1 * l1 * (m1 + m2);
    double term6 = g * (m1 + m2) * cos(theta1);
    double term7 = dtheta2 * dtheta2 * l2 * m2 * cos(delta);

    *d2theta2 = term4 * (term5 + term6 + term7) / den2;
}

// Génération de la séquence (Algorithme 2) → écriture directe dans final.txt
int generate_sequence_to_file(const char* key, int F) {
    InitParams p = initialize(key);
    double l1 = p.l1, l2 = p.l2;
    double m1 = p.m1, m2 = p.m2;
    double theta1 = p.theta1, theta2 = p.theta2;
    double r = p.r;

    double dtheta1 = 0.0, dtheta2 = 0.0;
    double g = 9.81;
    double h = 0.002;

    int L = strlen(key) * 8; // Longueur de la clé en bits
    long t0 = (long)(r * L);

    FILE *fout = fopen("final.txt", "w+");
    if (!fout) {
        perror("Impossible d'ouvrir final.txt");
        return -1;
    }

    long i = 0;
    int bits_written = 0;

    while (bits_written < F) {
        double d2theta1, d2theta2;
        compute_accelerations(theta1, theta2, dtheta1, dtheta2, l1, l2, m1, m2, g, &d2theta1, &d2theta2);

        dtheta1 += h * d2theta1;
        dtheta2 += h * d2theta2;
        theta1  += h * dtheta1;
        theta2  += h * dtheta2;

        if (i >= t0) {
            double x2 = l1 * sin(theta1) + l2 * sin(theta2);
            double y2 = -l1 * cos(theta1) - l2 * cos(theta2);

            if ((i % L) != 0) {
                // Normalisation : extraire la partie fractionnaire après ×1000
                double x_frac = x2 * 1000.0;
                x_frac = x_frac - floor(x_frac); // [0, 1)

                double y_frac = y2 * 1000.0;
                y_frac = y_frac - floor(y_frac); // [0, 1)

                uint32_t X = (uint32_t)(x_frac * 4294967296.0); // 2^32
                uint32_t Y = (uint32_t)(y_frac * 4294967296.0);

                // Intercaler les bits de X et Y (X31, Y31, X30, Y30, ...)
                for (int b = 31; b >= 0 && bits_written < F; --b) {
                    fputc('0' + ((X >> b) & 1), fout);
                    bits_written++;
                    if (bits_written >= F) break;
                    fputc('0' + ((Y >> b) & 1), fout);
                    bits_written++;
                }
            } else {
                // Mise à jour dynamique de r
                r = fmod((r + 1.0) * (r + 1.0), 2.0) + 2.0;
            }
        }
        i++;
    }

    fclose(fout);
    return 0; // succès
}
