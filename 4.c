
#include <stdio.h> 

int main(void) {
    int contained_count = 0;
    int overlap_count = 0;
    char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        int x1, y1, x2, y2;
        sscanf(line, "%d-%d,%d-%d", &x1, &y1, &x2, &y2);
        contained_count += (x1 >= x2 && y1 <= y2) || (x1 <= x2 && y1 >= y2);
        overlap_count += (x2 >= x1 && x2 <= y1) || (x1 >= x2 && x1 <= y2);
    }
    printf("%d\n", contained_count);
    printf("%d\n", overlap_count);
}