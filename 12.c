
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// a LOT of printf debugging simply because i did not read early on that E has an elevation of z
// once again i spent a lot of time re-reading and worrying about my code when i should have just re-read the problem
// anyways, here's a bad A*
// and yes, i DID brute-force part 2. doesn't take very long on my machine :)

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

typedef struct { int x,y; } iv2;

#define N 256

#define IV2Index(v) v.y][v.x
//                     ^^^ silliest thing ever fr

#define IV2Arg(v) v.x, v.y

int w = 0;
int h = 0;
iv2 end;

static char grid[N][N];

static iv2 came_from[N][N] = {0};

static int g_scores[N][N];
static int f_scores[N][N];

int iv2_eq(iv2 a, iv2 b) { return a.x == b.x && a.y == b.y; }

int g(iv2 v) {
    return g_scores[IV2Index(v)];
}

int manhattan_distance(iv2 a, iv2 b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int heuristic(iv2 v) {
    return manhattan_distance(v, end);
}

int f(iv2 v) {
    return f_scores[IV2Index(v)];
}

int a_star(iv2 start) {
    static iv2 open_set[N*N] = {0};
    open_set[0] = start;
    int open_set_count = 1;
    
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            g_scores[y][x] = N*N;
            f_scores[y][x] = N*N;
            came_from[y][x].x = N;
            came_from[y][x].y = N;
        }
    }
    g_scores[IV2Index(start)] = 0;
    f_scores[IV2Index(start)] = heuristic(start);
    
    while (open_set_count > 0) {
        int lowest_index;
        int lowest_f_score = N*N;
        for (int i = 0; i < open_set_count; i++) {
            if (f(open_set[i]) < lowest_f_score) {
                lowest_f_score = f(open_set[i]);
                lowest_index = i;
            }
        }
        
        iv2 current = open_set[lowest_index];
        //printf("current = (%d, %d)\n", IV2Arg(current));
        if (grid[IV2Index(current)] == 'E') {
            int count = 0;
            while (came_from[IV2Index(current)].x < 255) {
                current = came_from[IV2Index(current)];
                count++;
                //grid[IV2Index(current)] = count; // DELETE!!
            }
#if 0
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    int n = grid[y][x];
                    printf("%03d ", n);
                }
                printf("\n");
            }
#endif
            return count;
        }
        
        open_set_count--;
        for (int i = lowest_index; i < open_set_count; i++) {
            open_set[i] = open_set[i+1];
        }
        
        iv2 deltas[] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        for (int i = 0; i < ArrayCount(deltas); i++) {
            iv2 d = deltas[i];
            iv2 neighbor = {current.x + d.x, current.y + d.y};
            int a = grid[IV2Index(current)];
            int b = grid[IV2Index(neighbor)];
            if (neighbor.x >= 0 && neighbor.x < w &&
                neighbor.y >= 0 && neighbor.y < h &&
                ((islower(a) && islower(b) && b <= a + 1) ||
                 a == 'S' ||
                 ((a == 'y' || a == 'z') && b == 'E'))) {
                //printf("neighbor = (%d, %d) %c -> %c\n", IV2Arg(neighbor), grid[IV2Index(current)], grid[IV2Index(neighbor)]);
                int tentative_g_score = g(current) + 1;
                if (tentative_g_score < g(neighbor)) {
                    came_from[IV2Index(neighbor)] = current;
                    g_scores[IV2Index(neighbor)] = tentative_g_score;
                    f_scores[IV2Index(neighbor)] = tentative_g_score + heuristic(neighbor);
                    
                    int found = 0;
                    for (int i = 0; i < open_set_count; i++) {
                        if (iv2_eq(open_set[i], neighbor)) {
                            found = 1;
                            break;
                        }
                    }
                    if (!found) open_set[open_set_count++] = neighbor;
                }
            }
        }
# if 0
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                printf("(%3d %3d) ", IV2Arg(came_from[y][x]));
            }
            printf("\n");
        }
        printf("\n");
#endif
    }
    
    return 0;
}

int main(void) {
    while (fgets(grid[h], sizeof(grid[h]), stdin)) {
        if (isalpha(grid[h][0])) {
            w = 0;
            while (isalpha(grid[h][w])) w++;
            h++;
        }
    }
    
    iv2 start;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (grid[y][x] == 'S') {
                start.x = x;
                start.y = y;
            }
            if (grid[y][x] == 'E') {
                end.x = x;
                end.y = y;
            }
        }
    }
#if 0
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            printf("%c", grid[y][x]);
        }
        printf("\n");
    }
    printf("\n");
#endif
    
    printf("%d\n", a_star(start));
    
    int lowest = N*N;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (grid[y][x] == 'a') {
                iv2 start = {x,y};
                int n = a_star(start);
                if (n && n < lowest) lowest = n;
            }
        }
    }
    printf("%d\n", lowest);
}