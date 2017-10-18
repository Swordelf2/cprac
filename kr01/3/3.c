void
normalize_path(char *buf)
{
    if (*buf == '/' && buf[1] == '\0') {
        return;
    }

    char *names[1024];
    int cur_ind = 0;
    char *ptr = buf;
    while (*(++ptr) != '\0') {
        if (*ptr == '.' && ptr[1] == '.' && (ptr[2] == '/' || ptr[2] == '\0')) {
            ptr += 3;
            --cur_ind;
            continue;
        } else if (*ptr == '.' && (ptr[1] == '/' || ptr[1] == '\0')) {
            ptr += 2;
            continue;
        }
        names[cur_ind++] = ptr;
        ++ptr;
        while (*ptr != '/' && *ptr != '\0') {
            ++ptr;
        }
        if (*(ptr - 1) == '\0') {
            break;
        }
    }

    char *ptr1 = buf;
    char *ptr2;
    for (int i = 0; i < cur_ind; ++i) {
        *(ptr1++) = '/';
        ptr2 = names[i];
        while (*ptr2 != '/' && *ptr2 != '\0') {
            *ptr1 = *ptr2;
            ++ptr1;
            ++ptr2;
        }
    }
    *ptr1 = '\0';

}
