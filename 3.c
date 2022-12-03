
#include <stdio.h> 
#include <assert.h> 
#include <string.h>
#include <ctype.h>


int get_commons_priority(char **groups, int *counts, int group_count) {
    int tab[256] = {0};
    for (int i = 0; i < group_count-1; i++) {
        char *group = groups[i];
        int count = counts[i];
        int group_tab[256] = {0};
        for (int i = 0; i < count; i++) {
            group_tab[group[i]] = 1;
        }
        for (int i = 0; i < 256; i++) tab[i] += group_tab[i];
    }
    char *group = groups[group_count-1];
    int count = counts[group_count-1];
    for (int i = 0; i < count; i++) {
        char c = group[i];
        if (tab[c] == group_count-1) {
            if (islower(c)) return c - 'a' + 1;
            if (isupper(c)) return c - 'A' + 27;
            assert(0);
        }
    }
    assert(0);
    return 0;
}

int main(void) {
    int line_count = 0;
    static char lines[512][512];
    static char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        char *c = line + strlen(line) - 1;
        if (isspace(*c)) *c = 0;
        strcpy(lines[line_count++], line);
    }
    
    int sum1 = 0;
    for (int i = 0; i < line_count; i++) {
        char *line = lines[i];
        int count = strlen(line) / 2;
        char *groups[] = {line, line + count};
        int counts[] = {count, count};
        sum1 += get_commons_priority(groups, counts, 2);
    }
    printf("%d\n", sum1);
    
    int sum2 = 0;
    for (int i = 0; i < line_count; i += 3) {
        char *groups[3];
        int counts[3];
        for (int j = 0; j < 3; j++) {
            groups[j] = lines[i+j];
            counts[j] = strlen(lines[i+j]);
        }
        sum2 += get_commons_priority(groups, counts, 3);
    }
    
    printf("%d\n", sum2);
}