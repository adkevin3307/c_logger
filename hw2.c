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
                printf("usage: ./logger [-o file] [-p sopath] [--] cmd [cmd args ...]\n");
                printf("\t-p: set the path to logger.so, default = ./logger.so\n");
                printf("\t-o: print output to file, print to \"stderr\" if no file specified\n");
                printf("\t--: separate the arguments for logger and for the command\n");

                return 0;
        }
    }

    if (optind >= argc) {
        printf("no command given.\n");

        return 0;
    }

    cmd = argv[optind];
    arg = (argv + optind);

    if (sopath == NULL) {
        sopath = strdup("./logger.so");
    }

    if (file != NULL) {
        setenv("MONITOR", file, 1);
    }

    setenv("LD_PRELOAD", sopath, 1);
    execvp(cmd, arg);
}
