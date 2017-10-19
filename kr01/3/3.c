void
normalize_path(char *buf)
{
    if (buf[1] == '\0') {
        return;
    }

    char *ptr1 = buf + 1;
    char *ptr2 = buf;
    while (ptr2[0] && ptr2[1]) {
        ++ptr2;
        if (ptr2[0] == '.' && (ptr2[1] == '/' || ptr2[1] == '\0')) {
            ++ptr2;
        } else if (ptr2[0] == '.' && ptr2[1] == '.' && (ptr2[2] == '\0' || ptr2[2] == '/')) {
            ptr2 += 2;
            if (ptr1 != buf + 1) {
                do {
                    --ptr1;
                } while (ptr1[-1] != '/');
            }
        } else {
            while (ptr2 != '/') {
                *ptr1 = *ptr2;
                ++ptr2;
                ++ptr1;
            }
            *(ptr1++) = '/'; // TODO: this has to be done in the beginning
        }
    }
    *ptr1 = '\0';
}
