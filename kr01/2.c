#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        char *last = argv[i];
        char *newlast;
        unsigned count = 0;
        unsigned min = 0;
        unsigned max = UINT_MAX
        while (*last != '\0') {
            unsigned a = strtoul(last, &newlast, 10);
            if (a < min) {
                min = a;
            } else if (a > max) {
                max = a;
            }
            last = newlast;
            ++count;
        }
        if (count != 1) {
            
