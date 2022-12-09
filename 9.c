
#include <stdio.h>
#include <stdlib.h>

int sign(int n) { return (n > 0) - (n < 0); }

#define N 1024

typedef struct { int x, y; } iv2;

void sim_knots(char *grid, iv2 *knots, int knot_count, iv2 delta, int n) {
    for (int i = 0; i < n; i++) {
        knots[0].x += delta.x;
        knots[0].y += delta.y;
        
        for (int i = 1; i < knot_count; i++) {
            int diffx = knots[i].x - knots[i-1].x;
            int diffy = knots[i].y - knots[i-1].y;
            if (abs(diffx) > 1 || abs(diffy) > 1) {
                knots[i].x += -sign(diffx);
                knots[i].y += -sign(diffy);
            }
        }
        iv2 tail = knots[knot_count-1];
        grid[N*tail.y + tail.x] = 1;
    }
}

int main(void) {
    static char grid1[N][N] = {0};
    static char grid2[N][N] = {0};
    
    int x = N/2;
    int y = N/2;
    
    iv2 part1[2] = {{x,y},{x,y}};
    iv2 part2[10];
    for (int i = 0; i < 10; i++) {
        part2[i].x = x;
        part2[i].y = y;
    }
    
    char d = 0;
    int n = 0;
    while (scanf("%c %d\n", &d, &n) != EOF) {
        int dx, dy;
        switch(d) {
            case 'U': dx =  0; dy = -1; break;
            case 'D': dx =  0; dy =  1; break;
            case 'L': dx = -1; dy =  0; break;
            case 'R': dx =  1; dy =  0; break;
        }
        iv2 delta = {dx, dy};
        sim_knots((char*)grid1, part1, 2, delta, n);
        sim_knots((char*)grid2, part2, 10, delta, n);
    }
    
    int n1 = 0;
    int n2 = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            n1 += grid1[i][j];
            n2 += grid2[i][j];
        }
    }
    printf("%d\n%d\n", n1, n2);
}