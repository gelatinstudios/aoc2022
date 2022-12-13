
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

// since day 13 didn't take me very long, i had the idea to revist day 12 to improve its runtime and clean it up a bit

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

typedef struct { int x,y; } iv2;

#define N 256

#define IV2Index(v) v.y][v.x
//                     ^^^ silliest thing ever fr

#define IV2Arg(v) v.x, v.y

int w = 0;
int h = 0;

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

int heuristic(iv2 v, iv2 *ends, int end_count) {
    int lowest = N*N;
    for (int i = 0; i < end_count; i++) {
        int d = manhattan_distance(v, ends[i]);
        if (d < lowest) lowest = d;
    }
    return lowest;
}

int f(iv2 v) {
    return f_scores[IV2Index(v)];
}

int normalize_s_and_e(int c) {
    if (c == 'S') return 'a';
    if (c == 'E') return 'z';
    return c;
}

int a_star(iv2 start, int end_letter, iv2 *ends, int end_count, int backwards) {
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
    f_scores[IV2Index(start)] = heuristic(start, ends, end_count);
    
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
        if (grid[IV2Index(current)] == end_letter) {
            int count = 0;
            while (!iv2_eq(current, start)) {
                current = came_from[IV2Index(current)];
                count++;
            }
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
            if (neighbor.x >= 0 && neighbor.x < w &&
                neighbor.y >= 0 && neighbor.y < h) {
                int a = normalize_s_and_e(grid[IV2Index(current)]);
                int b = normalize_s_and_e(grid[IV2Index(neighbor)]);
                if ((!backwards && b <= a + 1) ||
                    (backwards && a <= b + 1)) {
                    int tentative_g_score = g(current) + 1;
                    if (tentative_g_score < g(neighbor)) {
                        came_from[IV2Index(neighbor)] = current;
                        g_scores[IV2Index(neighbor)] = tentative_g_score;
                        f_scores[IV2Index(neighbor)] = tentative_g_score + heuristic(neighbor, ends, end_count);
                        
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
        }
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
    
    iv2 start, end;
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
    
    iv2 ends1[] = {end};
    printf("%d\n", a_star(start, 'E', ends1, 1, 0));
    
    static iv2 ends2[N*N];
    int end_count = 0;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (grid[y][x] == 'a') {
                iv2 a = {x,y};
                ends2[end_count++] = a;
            }
        }
    }
    
    printf("%d\n", a_star(end, 'a', ends2, end_count, 1));
}