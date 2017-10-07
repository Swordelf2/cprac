#include <stdio.h>
#include <stdlib.h>

enum
{
    BUFF_LEN = 1022, // 1020 + '\n' + '\0'
    MAX_GIDS_SIZE = 32,
    DEC_BASE = 10,
    OCT_BASE = 8
};

int
u_compare(const void *arg1, const void *arg2);

int
main(void)
{
    char buff[BUFF_LEN];
    unsigned my_uid;
    unsigned my_gids[MAX_GIDS_SIZE];
    size_t my_gids_size;
    unsigned op; // operation to be done on files
    // Read my_uid
    fgets(buff, sizeof(buff), stdin);
    my_uid = strtoul(buff, NULL, DEC_BASE);

    // Read my_gids
    fgets(buff, sizeof(buff), stdin);
    char *last_end = buff;
    size_t i;
    for (i = 0; i < MAX_GIDS_SIZE; ++i) {
        char *new_last_end;
        my_gids[i] = strtoul(last_end, &new_last_end, DEC_BASE);
        if (new_last_end == last_end) {
            // List ended
            break;
        }
        last_end = new_last_end;
    }
    my_gids_size = i;
    qsort(my_gids, my_gids_size, sizeof(my_gids[0]), u_compare);

    // Read operation
    fgets(buff, sizeof(buff), stdin);
    op = strtoul(buff, NULL, OCT_BASE);
    
    // Read files
    while (fgets(buff, sizeof(buff), stdin)) {
        unsigned f_uid, f_gid, f_perm;
        char *f_name;
        f_uid = strtoul(buff, &last_end, DEC_BASE);
        f_gid = strtoul(last_end, &last_end, DEC_BASE);
        f_perm = strtoul(last_end, &last_end, OCT_BASE);
        f_name = last_end;

        if (f_uid == my_uid) {
            f_perm >>= 6; // use user permission bits
        } else if (bsearch(&f_gid, my_gids, my_gids_size, sizeof(my_gids[0]), u_compare)) {
            f_perm >>= 3; // use group permission bits
        }
        // otherwise use others permission bits
        if ((f_perm & op) == op) {
            while (*f_name == ' ') {
                ++f_name;
            }
            fputs(f_name, stdout);
        }
    }
            
    return 0;
}

int
u_compare(const void *arg1, const void *arg2)
{
    unsigned c_arg1 = *((unsigned *) arg1);
    unsigned c_arg2 = *((unsigned *) arg2);
    if (c_arg1 < c_arg2) {
        return -1;
    }
    if (c_arg1 == c_arg2) {
        return 0;
    }
    return 1;
}
