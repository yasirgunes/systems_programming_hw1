#include <stdio.h>

int main(int argc, char const *argv[])
{
    // print all the arguments
    for(int i = 0; i < argc; i++) {
        printf("Arg %d: %s\n", i, argv[i]);
    }
    return 0;
}
