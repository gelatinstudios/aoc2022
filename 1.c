
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int intcmp(const void *A, const void *B) {
    int a = *(int *)A;
    int b = *(int *)B;
    return a > b ? -1 : a < b;
}

int main(void) {
    int total_calories[4096];
    int elf_count = 0;
    
    
    int current_total_calories = 0;
    char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == '\n') {
            total_calories[elf_count++] = current_total_calories;
            current_total_calories = 0;
        } else {
            int calories = atoi(line);
            current_total_calories += calories;
        }
    }
    
    qsort(total_calories, elf_count, sizeof(total_calories[0]), intcmp);
    
    printf("%d\n", total_calories[0]);
    printf("%d\n", total_calories[0] + total_calories[1] + total_calories[2]);
}