#include <stdio.h>
#include <stdlib.h>

int main() {
    int n = 1000; // muda pra 1000 depois
    printf("%d %d\n", n, n*(n-1)/2);

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            int peso = rand() % 100 + 1;
            printf("%d %d %d\n", i, j, peso);
        }
    }
    return 0;
}