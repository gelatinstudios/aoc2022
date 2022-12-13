
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#define ArrayCount(arr) (sizeof(arr)/sizeof((arr)[0]))

const int LIST    = 0;
const int INTEGER = 1;

typedef struct node node;
struct node {
    int type;
    
    int value;
    
    int count;
    node *children[32];
};

int node_pool_count = 0;
node node_pool[16384];

node *new_node(void) {
    assert(node_pool_count < ArrayCount(node_pool));
    return &node_pool[node_pool_count++];
}

typedef struct {
    node *result;
    char *offset;
} parse_list_result;

parse_list_result parse_list(char *s) {
    assert(s[0] == '[');
    s++;
    
    node *r = new_node();
    r->type = LIST;
    r->count = 0;
    
    while (s[0] != ']') {
        if (isdigit(s[0])) {
            int n = 0;
            while (isdigit(*s)) {
                n *= 10;
                n += *s++ - '0';
            }
            
            node *i = new_node();
            i->type = INTEGER;
            i->value = n;
            
            assert(r->count < ArrayCount(r->children));
            r->children[r->count++] = i;
        } else if (s[0] == '[') {
            parse_list_result lr = parse_list(s);
            assert(r->count < ArrayCount(r->children));
            r->children[r->count++] = lr.result;
            s = lr.offset;
        } else {
            fprintf(stderr, "%d found\n", s[0]);
            assert(0);
        }
        if (s[0] == ',') s++;
    }
    
    parse_list_result result = {r, s+1};
    return result;
}

node *as_list(node *i) {
    if (i->type == INTEGER) {
        node *result = new_node();
        result->type = LIST;
        result->count = 1;
        result->children[0] = i;
        return result;
    }
    return i;
}

int compare(node *left, node *right) {
    int result = 0;
    if (left->type == INTEGER && right->type == INTEGER) {
        result = (left->value > right->value) - (left->value < right->value);
    } else if (left->type == LIST && right->type == LIST) {
        for (int i =0 ;; i++) {
            if (i >= left->count || i >= right->count) {
                if      (i >= left->count && i >= right->count) result = 0;
                else if (i >= left->count)                      result = -1;
                else                                            result = 1;
                break;
            }
            int c = compare(left->children[i], right->children[i]);
            if (c) {
                result = c;
                break;
            }
        }
    } else {
        result = compare(as_list(left), as_list(right));
    }
    return result;
}

int packet_cmp(const void *A, const void *B) {
    node *a = *(node **)A;
    node *b = *(node **)B;
    return compare(a, b);
}

int main(void) {
    static node *packets[4096];
    int packet_count = 0;
    
    int sum = 0;
    int pair = 1;
    
    static char line[4096];
    while (fgets(line, sizeof(line), stdin)) {
        node *left = parse_list(line).result;
        fgets(line, sizeof(line), stdin);
        node *right = parse_list(line).result;
        fgets(line, sizeof(line), stdin);
        
        sum += (compare(left, right)==-1) * pair++;
        
        packets[packet_count++] = left;
        packets[packet_count++] = right;
    }
    printf("%d\n", sum);
    
    node *key1 = parse_list("[[2]]").result;;
    node *key2 = parse_list("[[6]]").result;;
    
    packets[packet_count++] = key1;
    packets[packet_count++] = key2;
    
    qsort(packets, packet_count, sizeof(packets[0]), packet_cmp);
    
    int prod = 1;
    for (int i = 0; i < packet_count; i++) {
        if (packets[i] == key1 || packets[i] == key2) {
            prod *= i + 1;
        }
    }
    printf("%d\n", prod);
}