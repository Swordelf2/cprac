void
normalize_path(char *buf)
{
    if (*buf == '/' && buf[1] == '\0') {
        return;
    }
    char *ptr1 = buf;
    char *ptr2 = buf;
    while (*ptr2 != '\0') {
        while (*ptr2 != '/' && *ptr2 != '\0') {
            *ptr1 = *ptr2;
            ++ptr2;
            ++ptr1;
        }
        
