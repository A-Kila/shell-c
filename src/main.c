#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Flush after every printf
    setbuf(stdout, NULL);

    printf("$ ");

    char command[256];
    scanf("255%s", command);

    // error message
    printf("{%s}: command not found\n", command);

    return 0;
}
