void
normalize_path(char *buf)
{
    if (buf[1] == '\0') {
        return;
    }

    char *ptr1 = buf;
    char *ptr2 = buf;
    while (ptr2[0] && ptr2[1]) {
        ++ptr2;
        if (ptr2[0] == '.' && (ptr2[1] == '/' || ptr2[1] == '\0')) {
            ++ptr2;
        } else if (ptr2[0] == '.' && ptr2[1] == '.' && (ptr2[2] == '\0' || ptr2[2] == '/')) {
            ptr2 += 2;
            if (ptr1 != buf) {
                do {
                    --ptr1;
                } while (ptr1[0] != '/');
            }
        } else {
            *ptr1 = '/';
            ++ptr1;
            while (*ptr2 != '/') {
                *ptr1 = *ptr2;
                ++ptr2;
                ++ptr1;
            }
        }
    }
    if (ptr1 != buf) {
        *ptr1 = '\0';
    } else {
        buf[1] = '\0';
    }
}
