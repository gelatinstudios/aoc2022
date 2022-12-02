
#include <stdio.h>

int main(void) {
    int total_score1 = 0;
    int total_score2 = 0;
    
    char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        char A,B;
        sscanf(line, "%c %c", &A, &B);
        int a = A - 'A' + 1;
        int b = B - 'X' + 1;
        
        int outcomes1[] = {0, 6, 3, 0, 6};
        total_score1 += b + outcomes1[a-b+2];
        
        int outcomes2[] = {0, 3, 6};
        int shape2 = (a + b) % 3 + 1;
        total_score2 += shape2 + outcomes2[b-1];
    }
    printf("%d\n", total_score1);
    printf("%d\n", total_score2);
}