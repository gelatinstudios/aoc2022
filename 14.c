
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

typedef struct { int x,y; } iv2;

#define IV2Index(g, v) g[(v).y][(v).x]

int intcmp(int a, int b) {
    return (a > b) - (a < b);
}

iv2 IV2(int x, int y) {
    iv2 result = {x,y};
    return result;
}

iv2 iv2_add(iv2 a, iv2 b) { return IV2(a.x + b.x, a.y + b.y); }
int iv2_eq(iv2 a, iv2 b) { return a.x == b.x && a.y == b.y; }

int main(void) {
    static char grid[256][1024] = {0};
    
    int highest_y = 0;
    static char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        iv2 vertices[32];
        int vertex_count = 0;
        
        int count = 0;
        char *sep = "->, \n";
        char *tok = strtok(line, sep);
        while (tok) {
            vertices[vertex_count].x = atoi(tok);
            tok = strtok(0, sep);
            vertices[vertex_count++].y = atoi(tok);
            tok = strtok(0, sep);
        }
        
        for (int i = 0; i < vertex_count; i++) {
            int y = vertices[i].y;
            if (y > highest_y) highest_y = y;
        }
        
        for (int i = 0; i < vertex_count-1; i++) {
            iv2 a = vertices[i];
            iv2 b = vertices[i+1];
            iv2 d = {intcmp(b.x, a.x), intcmp(b.y, a.y)};
            for (iv2 v = a; !iv2_eq(v, iv2_add(b, d)); v = iv2_add(v, d)) {
                IV2Index(grid, v) = '#';
            }
        }
    }
    
    int units_of_sand1 = 0;
    int units_of_sand2 = 0;
    for (int part = 1; part <= 2; part++) {
        if (part == 2) {
            for (int y = 0; y < ArrayCount(grid); y++) {
                for (int x = 0; x < ArrayCount(grid[0]); x++) {
                    if (grid[y][x] == 'o') grid[y][x] = 0;
                }
            }
            for (int x = 0; x < ArrayCount(grid[0]); x++) {
                grid[highest_y+2][x] = '#';
            }
        }
        iv2 source = {500, 0};
        for (;;) {
            iv2 sand = source;
            
            int is_in_source = 0;
            int is_in_endless_void = 0;
            int is_falling = 1;
            while (is_falling) {
                is_falling = 0;
                iv2 deltas[] = {{0,1}, {-1,1}, {1,1}};
                for (int i = 0; i < ArrayCount(deltas); i++) {
                    iv2 dest = iv2_add(sand, deltas[i]);
                    if (dest.y == ArrayCount(grid)) {
                        is_in_endless_void = 1;
                        break;
                    }
                    if (!IV2Index(grid, dest)) {
                        is_falling = 1;
                        sand = dest;
                        break;
                    }
                }
            }
            
            if (is_in_endless_void) break;
            
            IV2Index(grid, sand) = 'o';
            
            if (part == 1) units_of_sand1++;
            if (part == 2) units_of_sand2++;
            
            if (iv2_eq(source, sand)) break;
#if 0
            if (part == 2) {
                for (int y = 0; y < 16; y++) {
                    for (int x = 480; x < 520; x++) {
                        putchar(grid[y][x]);
                    }
                    putchar('\n');
                }
                putchar('\n');
            }
#endif
        }
    }
    printf("%d\n%d\n", units_of_sand1, units_of_sand2);
}