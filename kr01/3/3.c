void
normalize_path(char *buf)
{
    // don't need to do anything if the string is "/"
    if (buf[1] == '\0') {
        return;
    }

    // we will be copying from *ptr2 to *ptr1
    char *ptr1 = buf;
    char *ptr2 = buf;
    // loop until we hit a final slash or '\0'
    while (ptr2[0] && ptr2[1]) {
        ++ptr2;
        // current directory
        if (ptr2[0] == '.' && (ptr2[1] == '/' || ptr2[1] == '\0')) {
            ++ptr2;
        // parent directory
        } else if (ptr2[0] == '.' && ptr2[1] == '.' && (ptr2[2] == '\0' || ptr2[2] == '/')) {
            ptr2 += 2;
            if (ptr1 != buf) {
                // if ptr1 is not on the beginning of the string, move it one directory back
                do {
                    --ptr1;
                } while (ptr1[0] != '/');
            }
        } else {
            // copy the name of a directory with a preceding slash
            *ptr1 = '/';
            ++ptr1;
            while (*ptr2 != '/' && *ptr2 != '\0') {
                *ptr1 = *ptr2;
                ++ptr2;
                ++ptr1;
            }
        }
    }
    // finally put a termination null-byte
    if (ptr1 != buf) {
        *ptr1 = '\0';
    } else {
        buf[1] = '\0';
    }
}
