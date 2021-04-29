#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int opt = 0;
    char* sopath = NULL;
    char* file = NULL;
    char* cmd = NULL;
    char** arg = NULL;

    while ((opt = getopt(argc, argv, "p:o:")) != -1) {
        switch (opt) {
            case 'p':
                sopath = strdup(optarg);

                break;
            case 'o':
                file = strdup(optarg);

                break;
            default:
                break;
        }
    }

    cmd = argv[optind];
    arg = (argv + optind);

    if (sopath == NULL) {
        sopath = strdup("./logger.so");
    }

    FILE* output = stderr;
    if (file != NULL) {
        output = fopen(file, "w");

        dup2(fileno(output), STDERR_FILENO);
    }

    setenv("LD_PRELOAD", sopath, 1);
    execvp(cmd, arg);
}
