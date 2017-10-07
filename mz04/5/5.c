#include <stdio.h>

int
bsearch(unsigned *arr, size_t nmemb, size_t key);

int
main(void)
{
    unsigned arr[] = {0, 1, 2, 5, 5, 10, 11, 24, 100, 101};
    printf("Array: ");
    for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) {
        printf("%u ", arr[i]);
    }
    putchar('\n');
    while (1) {
        unsigned key;
        printf("Enter the key: ");
        if (scanf("%u", &key) == 0) {
            break;
        }
        printf("%d\n", bsearch(arr, sizeof(arr) / sizeof(arr[0]), key));
    }
    return 0;
}

int
bsearch(unsigned *arr, size_t nmemb, size_t key)
{
    size_t left = 0;
    size_t right = nmemb;
    size_t mid = 0;
    while (left != right) {
        mid = (left + right) / 2;
        if (key < arr[mid]) {
            right = mid;
        } else if (key == arr[mid]) {
            return 1;
        } else {
            left = mid + 1;
        }
    }
    if (key == arr[left]) {
        return 1;
    }
    return 0;
}
