
#include <stdio.h>
#include <assert.h>

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

int main(void) {
    int instruction_count = 0;
    static int instructions[4096];
    
    static char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        int n = 0;;
        sscanf(line, "addx %d", &n);
        instructions[instruction_count++] = n;
    }
    
    int checks[] = {20, 60, 100, 140, 180, 220};
    
    static char screen[6][40];
    
    int sum = 0;
    int X = 1;
    int current_instruction = 0;
    int current_instruction_cycle_offset = 0;
    for (int cycle = 1, i = 0; current_instruction < instruction_count; cycle++) {
        if (cycle == checks[i]) {
            i++;
            sum += cycle*X;
        }
        
        int x = (cycle-1) % ArrayCount(screen[0]);
        int y = ((cycle-1) / ArrayCount(screen[0]));
        
        if (X - 1 == x ||
            X     == x ||
            X + 1 == x) {
            screen[y][x] = 1;
        }
        
        int instruction = instructions[current_instruction];
        if (instruction) {
            if (current_instruction_cycle_offset) {
                current_instruction_cycle_offset=0;
                X += instruction;
                current_instruction++;
            } else {
                current_instruction_cycle_offset=1;
            }
        } else current_instruction++;
    }
    printf("%d\n", sum);
    for (int y = 0; y < ArrayCount(screen); y++) {
        for (int x = 0; x < ArrayCount(screen[0]); x++) {
            printf("%c", screen[y][x] ? '@' : ' ');
        }
        printf("\n");
    }
}