#include <dlfcn.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    void *l_handle = dlopen("/lib/libm.so.6", RTLD_NOW);
    double (*f)(double) = dlsym(l_handle, argv[1]);
    double arg;
    while (scanf("%lf", &arg) == 1) {
        printf("%.10g\n", f(arg));
    }
    dlclose(l_handle);
    return 0;
}
