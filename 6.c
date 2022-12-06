
#include <stdio.h>

int find_start_of_whatever(char *input, int length) {
    for (char *c = input; c[3]; c++) {
        int tab[32] = {0};
        for (int i=0;i<length;i++) tab[c[i] - 'a']++;
        int start_of_packet=1;
        for (int i = 0; i < 32; i++) {
            if (tab[i] > 1) {
                start_of_packet = 0;
                break;
            }
        }
        if (start_of_packet) return (int)(c - input + length);
    }
    return 0;
}

int main(void) {
    static char input[4096] = {0};
    fgets(input, sizeof(input), stdin);
    printf("%d\n", find_start_of_whatever(input, 4));
    printf("%d\n", find_start_of_whatever(input, 14));
}