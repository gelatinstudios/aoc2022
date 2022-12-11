
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

typedef int64_t s64;

int s64cmp(const void *A, const void *B) {
    s64 a = *(s64 *)A;
    s64 b = *(s64 *)B;
    return (a < b) - (a > b);
}

typedef struct {
    char op; int op_end; // if op_end == 0 use `old`
    
    int test;
    int if_true;
    int if_false;
    
    int item_count;
    s64 items[128];
    
    s64 items_inspected;
} monkey;

int main(void) {
    int monkey_count = 0;
    static monkey monkies[16];
    
    static char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        monkey *m = &monkies[monkey_count++];
        
        fgets(line, sizeof(line), stdin);
        char *c = line + 18;
        char *sep = ", ";
        char *tok = strtok(c, sep);
        while (tok) {
            m->items[m->item_count++] = atoi(tok);
            tok = strtok(0, sep);
        }
        
        fgets(line, sizeof(line), stdin);
        char end_str[32];
        sscanf(line, " Operation: new = old %c %s", &m->op, end_str);
        m->op_end = atoi(end_str);
        
        fgets(line, sizeof(line), stdin);
        sscanf(line, " Test: divisible by %d", &m->test);
        
        fgets(line, sizeof(line), stdin);
        sscanf(line, " If true: throw to monkey %d", &m->if_true);
        
        fgets(line, sizeof(line), stdin);
        sscanf(line, " If false: throw to monkey %d", &m->if_false);
        
        fgets(line, sizeof(line), stdin);
    }
    
#if 0
    for (int i = 0; i < monkey_count; i++) {
        monkey m = monkies[i];
        printf("Monkey %d:\n", i);
        printf("  Starting items: ");
        for (int i = 0; i < m.item_count; i++) printf("%lld, ", m.items[i]);
        printf("\n");
        printf("  Operation: new = old %c %d\n", m.op, m.op_end);
        printf("  Test: divisible by %d\n", m.test);
        printf("    If true: throw to monkey %d\n", m.if_true);
        printf("    If false: throw to monkey %d\n", m.if_false);
        printf("\n");
    }
#endif
    
    s64 modulo = 1;
    for (int i = 0; i < monkey_count; i++) 
        modulo *= monkies[i].test;
    
    static monkey copy[ArrayCount(monkies)];
    memcpy(copy, monkies, sizeof(monkies));
    
    int rounds[] = {20, 10000};
    for (int part = 0; part < 2; part++) {
        memcpy(monkies, copy, sizeof(monkies));
        for (int round = 0; round < rounds[part]; round++) {
            for (int i = 0; i < monkey_count; i++) {
                monkey *m = &monkies[i];
                for (int i = 0; i < m->item_count; i++) {
                    s64 item = m->items[i];
                    s64 op_end = m->op_end ? m->op_end : item;
                    switch (m->op) {
                        case '*': item *= op_end; break;
                        case '+': item += op_end; break;
                        default: assert(0);
                    }
                    if (part == 0) item /= 3;
                    item %= modulo;
                    monkey *dest = &monkies[item % m->test == 0 ? m->if_true : m->if_false];
                    dest->items[dest->item_count++] = item;
                }
                m->items_inspected += m->item_count;
                m->item_count = 0;
            }
        }
        
        s64 items_inspecteds[ArrayCount(monkies)] = {0};
        for (int i = 0; i < monkey_count; i++) {
            items_inspecteds[i] = monkies[i].items_inspected;
            //printf("%lld\n", items_inspecteds[i]);
        }
        //printf("\n");
        
        qsort(items_inspecteds, monkey_count, sizeof(items_inspecteds[0]), s64cmp);
        printf("%lld\n", items_inspecteds[0] * items_inspecteds[1]);
        
        //printf("\n");
    }
}