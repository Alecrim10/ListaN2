#include <stdio.h>
#include <stdlib.h>

double* cria_vetor(int n) {
    if (n <= 0) return NULL;
    double* v = (double*)malloc(sizeof(double) * n);
    if (!v) {
        perror("malloc");
        return NULL;
    }
    return v;
}

void imprime_vetor(double* v, int n) {
    if (!v) {
        printf("Vetor vazio.\n");
        return;
    }
    printf("Elementos do vetor:\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] = %.6g\n", i, v[i]);
    }
}

void libera_vetor(double* v) {
    free(v);
}

int main() {
    int n;
    printf("Digite o tamanho n do vetor: ");
    if (scanf("%d", &n) != 1) {
        printf("Entrada invalida.\n");
        return 1;
    }
    double* v = cria_vetor(n);
    if (!v) return 1;

    printf("Digite %d valores (separados por espaco ou Enter):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%lf", &v[i]) != 1) {
            printf("Entrada invalida. Abortando.\n");
            libera_vetor(v);
            return 1;
        }
    }

    imprime_vetor(v, n);
    libera_vetor(v);
    printf("Memoria liberada.\n");
    return 0;
}
