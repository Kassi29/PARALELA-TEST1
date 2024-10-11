#include <stdio.h>
#include <omp.h>

int main() {
    int n = 20;
    int a = 0, b = 1, c;
    printf("%d %d ", a, b);
    #pragma omp parallel shared(a, b) private(c)
    {
        #pragma omp for
        for (int i = 2; i < n; i++) {
            #pragma omp critical
            {
                c = a + b;
                a = b;
                b = c;
                printf("%d ", b);
            }
        }
    }

    printf("\n");
    return 0;
}
