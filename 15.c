
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ArrayCount(arr) (sizeof(arr)/sizeof(arr[0]))
#define Minimum(a, b) ((a) < (b) ? (a) : (b))
#define Maximum(a, b) ((a) > (b) ? (a) : (b))

typedef struct { int min, max; } range;
typedef struct { int x, y, d; } sensor;

typedef struct { int x, y; } iv2;

iv2 IV2(int x, int y) {
    iv2 result = {x, y};
    return result;
}
int iv2_eq(iv2 a, iv2 b) { return a.x == b.x && a.y == b.y; }
iv2 iv2_add(iv2 a, iv2 b) { return IV2(a.x + b.x, a.y + b.y); }
iv2 iv2_sub(iv2 a, iv2 b) { return IV2(a.x - b.x, a.y - b.y); }
int iv2_dot(iv2 a, iv2 b) { return a.x*b.x + a.y*b.y; }
int iv2_cross(iv2 a, iv2 b) { return a.x*b.y - a.y*b.x; }
iv2 iv2_clamp(iv2 p, int min, int max) { return IV2(Minimum(Maximum(p.x, min), max), Minimum(Maximum(p.x, min), max)); }

int manhattan_distance(iv2 a, iv2 b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int comp(int a, int b) {
    return (a > b) - (a < b);
}

#define TEST 0

typedef struct { iv2 p; int count; } intersection;

static intersection intersections[4096*32];
int intersection_count = 0;

void add_intersection(iv2 ip, int add_if_not_found) {
    int found = 0;
    for (int i = 0; i < intersection_count; i++) {
        intersection *in = &intersections[i];
        if (in->p.x == ip.x && in->p.y == ip.y) {
            in->count++;
            found = 1;
            break;
        }
    }
    if (add_if_not_found && !found) {
        intersection in = {ip, 1};
        intersections[intersection_count++] = in;
    }
}

int main(void) {
    static sensor sensors[256];
    int sensor_count = 0;
    
    static range ranges_a[4096];
    static range ranges_b[4096];
    int range_count = 0;
    
    range *ranges = ranges_a;
    
    int part1_y = TEST ? 10 : 2000000;
    
    static int beacons_on_part1_y[4096];
    int beacons_on_part1_y_count = 0;
    
    int sx, sy, bx, by;
    while (scanf("Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d\n", &sx, &sy, &bx, &by) != EOF) {
        if (by == part1_y) {
            int found = 0;
            for (int i = 0; i < beacons_on_part1_y_count; i++) {
                if (beacons_on_part1_y[i] == bx) {
                    found = 1;
                    break;
                }
            }
            if (!found) beacons_on_part1_y[beacons_on_part1_y_count++] = bx;
        }
        
        int manhattan_distance = abs(bx - sx) + abs(by - sy);
        
        sensor s = {sx, sy, manhattan_distance};
        sensors[sensor_count++] = s;
        
        if (part1_y >= sy - manhattan_distance && part1_y <= sy + manhattan_distance) {
            int cx = abs(manhattan_distance - abs(part1_y - sy));
            int min = sx - cx;
            int max = sx + cx;
            range a = {min, max};
            
            //printf("(%d, %d)\n\n", min, max);
            
            int does_intersect = 0;
            char intersect_tab[4096] = {0};
            for (int i = 0; i < range_count; i++) {
                range b = ranges[i];
                if ((a.min >= b.min && a.min <= b.max) ||
                    (b.min >= a.min && b.min <= a.max)) {
                    intersect_tab[i] = 1;
                    does_intersect = 1;
                    a.min = Minimum(a.min, b.min);
                    a.max = Maximum(a.max, b.max);
                }
            }
            if (does_intersect) {
                range *new_ranges = ranges == ranges_a ? ranges_b : ranges_a;
                int new_range_count = 0;
                for (int i = 0; i < range_count; i++) {
                    if (!intersect_tab[i]) {
                        new_ranges[new_range_count++] = ranges[i];
                    }
                }
                new_ranges[new_range_count++] = a;
                ranges = new_ranges;
                range_count = new_range_count;
            } else {
                ranges[range_count++] = a;
            }
            
#if 0 
            for (int i = 0; i < range_count; i++) {
                range r = ranges[i];
                printf("[%d, %d]\n", r.min, r.max);
            }
            printf("\n");
#endif
        }
    }
    
    int sum = 0;
    for (int i = 0; i < range_count; i++) {
        range r = ranges[i];
        //printf("(%d, %d)\n", r.min, r.max);
        sum += r.max - r.min + 1;
    }
    printf("%d\n", sum - beacons_on_part1_y_count);
    
    int min = 0;
    int max = TEST ? 20 : 4000000;
    
    typedef struct { iv2 a, b; } line_seg;
    
    line_seg segs[256];
    int seg_count = 0;
    
    for (int i = 0; i < sensor_count; i++) {
        sensor s = sensors[i];
        
        int d = s.d + 1;
        iv2 points[] = {
            { s.x - d, s.y },
            { s.x, s.y - d },
            { s.x + d, s.y },
            { s.x, s.y + d},
            { s.x - d, s.y },
        };
        
        for (int i = 0; i < ArrayCount(points)-1; i++) {
            iv2 a = points[i];
            iv2 b = points[i+1];
            line_seg seg = {a,b};
            assert(seg_count < ArrayCount(segs));
            segs[seg_count++] = seg;
        }
    }
    
    int count = 0;
    for (int i = 0; i < seg_count; i++) {
        line_seg seg = segs[i];
        
        int dx = comp(seg.b.x, seg.a.x); 
        int dy = comp(seg.b.y, seg.a.y);
        iv2 d = {dx, dy};
        
        for (iv2 v = seg.a; !iv2_eq(v, iv2_add(seg.b, d)); v = iv2_add(v, d)) {
            if (v.x >= min && v.x <= max && v.y >= min && v.y <= max) {
                int found = 0;
                for (int i = 0; i < sensor_count; i++) {
                    sensor s = sensors[i];
                    if (manhattan_distance(v, IV2(s.x, s.y)) <= s.d) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("%lld\n", (long long)v.x*4000000 + (long long)v.y);
                    return 0;
                }
            }
        }
    }
    
    printf(":(\n", count);
}
