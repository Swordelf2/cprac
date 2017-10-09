#include <stdio.h>
#include <time.h>

enum
{
    STR_MAX_LEN = 1002
};

int read_time(FILE *inp_file, time_t *p_res);

int
main(void)
{
    tzset();
    FILE *inp_file = fopen("input.txt", "r");
    time_t prev;
    if (read_time(inp_file, &prev) == -1) {
        fclose(inp_file);
        return 0;
    }

    time_t cur;
    while (read_time(inp_file, &cur) == 0) {
        printf("%.f\n", difftime(cur, prev));
        prev = cur;
    }
    
    fclose(inp_file);
    return 0;
}

int read_time(FILE *inp_file, time_t *p_res)
{
    struct tm cur_tm;
    char str[STR_MAX_LEN];

    if(!fgets(str, sizeof(str), inp_file)) {
        return -1; 
    }
    sscanf(str, "%4d/%2d/%2d %2d:%2d:%2d",
            &cur_tm.tm_year,
            &cur_tm.tm_mon,
            &cur_tm.tm_mday,
            &cur_tm.tm_hour,
            &cur_tm.tm_min,
            &cur_tm.tm_sec);
    cur_tm.tm_year -= 1900;
    cur_tm.tm_mon -= 1;
    cur_tm.tm_isdst = -1;
    *p_res = mktime(&cur_tm);
    return 0;
}
