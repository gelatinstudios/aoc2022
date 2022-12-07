
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

int starts_with(char *a, char *b) {
    return !strncmp(a, b, strlen(b));
}

typedef int64_t s64;

typedef struct node node;
struct node {
    char name[32];
    int is_dir;
    s64 size;
    
    node *parent;
    int children_count;
    node *children[256];
};

int node_count = 0;
node node_pool[4096] = {0};

s64 find_size(node *current) {
    s64 result = 0;
    if (current->is_dir) {
        for (int i = 0; i < current->children_count; i++) 
            result += find_size(current->children[i]);
    } else  result = current->size;
    return result;
}

void size_dirs(node *current) {
    if (current->is_dir) current->size = find_size(current);
    for (int i = 0; i < current->children_count; i++) 
        size_dirs(current->children[i]);
}

s64 part1(node *current) {
    s64 result = 0;
    if (current->is_dir && current->size <= 100000) 
        result = current->size;
    for (int i = 0; i < current->children_count; i++) 
        result += part1(current->children[i]);
    return result;
}

s64 part2(node *current, s64 need_to_delete) {
    s64 result = INT64_MAX;
    if (current->is_dir) {
        if (current->size >= need_to_delete) result = current->size;
        for (int i = 0; i < current->children_count; i++) {
            s64 n = part2(current->children[i], need_to_delete);
            if (n >= need_to_delete && n < result) result = n;
        }
    }
    return result;
}

void print_tree(node *current, int indent) {
    for (int i =0;i<indent;i++)putchar(' ');
    printf("%s %lld\n", current->name, current->size);
    for (int i = 0; i < current->children_count;i++) print_tree(current->children[i], indent+4);
}

int main(void) {
    node *root = &node_pool[node_count++];
    root->is_dir = 1;
    strcpy(root->name, "/");
    
    node *current = root;
    
    static char line[4096];
    fgets(line, sizeof(line), stdin); // $ cd /
    while (fgets(line, sizeof(line), stdin)) {
        assert(current);
        
        if (starts_with(line, "$")) {
            if (starts_with(line, "$ cd")) {
                char dir[32];
                sscanf(line, "$ cd %s", dir);
                if (!strcmp(dir, "..")) {
                    current = current->parent;
                } else {
                    for (int i = 0; i < current->children_count; i++ ){
                        node *child = current->children[i];
                        if (!strcmp(dir, child->name)) {
                            current = child;
                            assert(child->size == 0);
                            break;
                        }
                    }
                }
            }
        } else {
            assert(node_count < 4096);
            node *new_node = &node_pool[node_count++];
            
            char size_str[32];
            sscanf(line, "%s %s", size_str, new_node->name);
            s64 size = atoll(size_str);
            
            if (size) new_node->size = size;
            else new_node->is_dir = 1;
            
            new_node->parent = current;
            current->children[current->children_count++] = new_node;
        }
    }
    
    size_dirs(root);
    
    //print_tree(root, 0);
    
    printf("%lld\n", part1(root));
    
    s64 free_space = 70000000 - root->size;
    s64 need_to_delete = 30000000 - free_space;
    
    printf("%lld\n", part2(root, need_to_delete));
}