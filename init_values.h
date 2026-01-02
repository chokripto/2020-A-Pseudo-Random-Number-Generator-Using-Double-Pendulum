// init_values.h
#ifndef INIT_VALUES_H
#define INIT_VALUES_H

#include <stdio.h>

// Structure pour les paramètres initialisés à partir de la clé
typedef struct {
    double l1, l2;     // Longueurs des tiges
    double m1, m2;     // Masses
    double theta1, theta2; // Angles initiaux (en radians)
    double r;          // Paramètre utilisé pour la période transitoire
} InitParams;

// Déclaration de la fonction d'initialisation
InitParams initialize(const char* key);

#endif // INIT_VALUES_H
