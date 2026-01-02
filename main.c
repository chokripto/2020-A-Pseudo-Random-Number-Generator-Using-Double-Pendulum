// main.c
#include <stdio.h>
#include "init_values.h"

int generate_sequence_to_file(const char* key, int F);

int main() {
    const char* key = "CHOKRI";  // ≥ 4 caractères
    int F = 1000000;             // 1 million de bits

    if (generate_sequence_to_file(key, F) == 0) {
        printf("Séquence de %d bits enregistrée dans final.txt\n", F);
    } else {
        fprintf(stderr, "Échec de la génération\n");
        return 1;
    }

    return 0;
}
