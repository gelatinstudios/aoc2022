
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int count;
    char crates[512];
} stack;

void print_tops(stack *stacks) {
    for (int i = 0; i < 10; i++) {
        stack *s = &stacks[i];
        if (s->count) printf("%c", s->crates[s->count-1]);
    }
    printf("\n");
}

int main(void) {
    int line_count = 0;
    static char lines[32][4096] = {0};
    static char line[4096] = {0};
    while (fgets(line, sizeof(line), stdin)) {
        if (!strcmp(line, "\n")) break;
        strcpy(lines[line_count++], line);
    }
    
    static stack stacks[10] = {0};
    
    char *index_line = lines[line_count-1];
    int stack_index = 1;
    for (int i = 0; index_line[i]; i++) {
        if (isdigit(index_line[i])) {
            for (int line = line_count-2; line >= 0; line--) {
                char c = lines[line][i];
                if (isalpha(c)) {
                    stack *s = &stacks[stack_index];
                    s->crates[s->count++] = c;
                }
            }
            stack_index++;
        }
    }
    
    static stack part1[10];
    memcpy(part1, stacks, sizeof(stacks));
    stack *part2 = stacks;
    for (int count, a_index, b_index; scanf("move %d from %d to %d\n", &count, &a_index, &b_index) != EOF; ) {
        for (int i = 0; i < count; i++) {
            stack *a = &part1[a_index];
            stack *b = &part1[b_index];
            char c = a->crates[--a->count];
            b->crates[b->count++] = c;
        }
        
        stack *a = &part2[a_index];
        stack *b = &part2[b_index];
        memcpy(b->crates+b->count, a->crates+a->count-count, count);
        b->count += count;
        a->count -= count;
    }
    
    print_tops(part1);
    print_tops(part2);
}
