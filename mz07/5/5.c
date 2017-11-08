#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum
{
    RR_MUL = 1103515245u,
    RR_INCR = 12345u,
    RR_MOD = 0x80000000u,
    CHAR_COUNT = 62,
    BUF_SIZE = 35,
    NUM_COUNT = RR_MOD / CHAR_COUNT
};

unsigned rr_cur;

void
NextRR(void);

char
MakeChar(unsigned a);

unsigned
GetRange(char a);
// returns the bottom of the range
    
int
main(void)
{
    char str[BUF_SIZE];
    fgets(str, sizeof(str), stdin);
    size_t str_len = strlen(str);
    if (str[str_len - 1] == '\n') {
        str[--str_len] = '\0';
    }
    if (str_len == 1) {
        putchar(str[0]);
        putchar('\n');
        return 0;
    }
    unsigned try_seed = GetRange(str[0]);
    unsigned good_seed = 0;
    int found_seed_flag = 0;
    for (unsigned i = 0; i < NUM_COUNT; ++i, ++try_seed) {
        int good_seed_flag = 1;
        rr_cur = try_seed;
        for (unsigned j = 1; j < str_len; ++j) {
            NextRR();
            if (str[j] != '.') {
                if (MakeChar(rr_cur) != str[j]) {
                    good_seed_flag = 0;
                    break;
                }
            }
        }
        if (good_seed_flag) {
            if (found_seed_flag) {
                found_seed_flag = 2; // indicating that we found two different seeds
                break;
            } else {
                good_seed = try_seed;
                found_seed_flag = 1;
            }
        }
    }
    if (found_seed_flag == 1) {
        putchar(str[0]);
        rr_cur = good_seed;
        for (unsigned j = 1; j < str_len; ++j) {
            NextRR();
            putchar(MakeChar(rr_cur));
        }
    } else {
        putchar('#');
    }
    putchar('\n');
    return 0;
}

void
NextRR(void)
{
    rr_cur = (rr_cur * RR_MUL + RR_INCR) % RR_MOD;
}

char
MakeChar(unsigned a)
{
    double val = (double) a / RR_MOD;
    unsigned int_val = val * CHAR_COUNT;
    if (int_val <= '9' - '0') {
        return '0' + int_val;
    } else {
        int_val -= ('9' - '0' + 1);
        if (int_val <= 'Z' - 'A') {
            return 'A' + int_val;
        } else {
            int_val -= ('Z' - 'A' + 1);
            return 'a' + int_val;
        }
    }
}

unsigned
GetRange(char a)
{
    unsigned dist;
    if (isdigit(a)) {
        dist = a - '0';
    } else {
        dist = '9' - '0' + 1;
        if (isupper(a)) {
            dist += a - 'A';
        } else {
            dist += ('Z' - 'A' + 1) + a - 'a';
        }
    }
    return (double) dist / CHAR_COUNT * RR_MOD;
}
