
#include <stdio.h>
#include <ctype.h>

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

int main(void) {
    unsigned char grid[256][256];
    
    int y_count = 0;
    int x_count = 0;
    char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        x_count = 0;
        for (char *c = line; isdigit(*c); c++) {
            grid[y_count][x_count++] = *c - '0';
        }
        y_count++;
    }
    
    int ds[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    int highest_scenic_score = 0;
    int visible_tree_count = 2*(x_count+y_count) - 4;
    for (int y = 1; y < y_count-1; y++) {
        for (int x = 1; x < x_count-1; x++) {
            int scenic_score = 1;
            int is_visible = 0;
            for (int i = 0; i < ArrayCount(ds); i++) {
                int dx = ds[i][0];
                int dy = ds[i][1];
                
                int viewing_distance = 0;
                int all_shorter = 1;
                int x2 = x + dx;
                int y2 = y + dy;
                while (x2 >= 0 && x2 < x_count &&
                       y2 >= 0 && y2 < y_count) {
                    viewing_distance++;
                    int is_shorter = grid[y2][x2] < grid[y][x];
                    if (!is_shorter) {
                        all_shorter = 0;
                        break;
                    }
                    
                    x2 += dx;
                    y2 += dy;
                }
                
                scenic_score *= viewing_distance;
                is_visible |= all_shorter;
            }
            visible_tree_count += is_visible;
            if (scenic_score > highest_scenic_score) highest_scenic_score = scenic_score;
        }
    }
    printf("%d\n", visible_tree_count);
    printf("%d\n", highest_scenic_score);
}