#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STRLEN PATH_MAX

static FILE* monitor = NULL;

void _log(const char* partial_format, ...)
{
    char format[STRLEN] = { '\0' };
    sprintf(format, "[logger] %s", partial_format);

    if (monitor == NULL) {
        char* monitor_env = getenv("MONITOR");

        if (monitor_env) {
            FILE* (*origin_fopen)(const char*, const char*) = dlsym(RTLD_NEXT, "fopen");

            monitor = (*origin_fopen)(monitor_env, "w");
        }
        else {
            monitor = stderr;
        }
    }

    va_list arg;

    va_start(arg, partial_format);
    vfprintf(monitor, format, arg);
    va_end(arg);
}

void _check(char* buffer, char result[])
{
    for (int i = 0; i < strlen(buffer) + 1 && i < 32; i++) {
        if (isprint(buffer[i])) {
            result[i] = buffer[i];
        }
        else {
            result[i] = '.';
        }
    }
}

void _get_real_path(const char* path, char real_path[])
{
    char* exist;

    exist = realpath(path, real_path);

    if (exist == NULL) {
        strcpy(real_path, path);
    }
}

void _get_fd_path(int fd, char fd_path[])
{
    char path[STRLEN] = { '\0' };
    sprintf(path, "/proc/self/fd/%d", fd);

    if (readlink(path, fd_path, STRLEN) == -1) {
        perror("_get_fd_path error\n");

        exit(EXIT_FAILURE);
    }
}

void _get_file_path(FILE* stream, char file_path[])
{
    int fd = fileno(stream);

    _get_fd_path(fd, file_path);
}

int chmod(const char* path, mode_t mode)
{
    char real_path[STRLEN] = { '\0' };
    _get_real_path(path, real_path);

    int (*origin_function)(const char*, mode_t);
    origin_function = dlsym(RTLD_NEXT, "chmod");
    int result = (*origin_function)(path, mode);

    _log("chmod(\"%s\", %o) = %d\n", real_path, mode, result);

    return result;
}

int chown(const char* path, uid_t owner, gid_t group)
{
    char real_path[STRLEN] = { '\0' };
    _get_real_path(path, real_path);

    int (*origin_function)(const char*, uid_t, gid_t);
    origin_function = dlsym(RTLD_NEXT, "chown");
    int result = (*origin_function)(path, owner, group);

    _log("chown(\"%s\", %d, %d) = %d\n", real_path, owner, group, result);

    return result;
}

int close(int fildes)
{
    char fd_path[STRLEN] = { '\0' };
    _get_fd_path(fildes, fd_path);

    int (*origin_function)(int);
    origin_function = dlsym(RTLD_NEXT, "close");
    int result = (*origin_function)(fildes);

    _log("close(\"%s\") = %d\n", fd_path, result);

    return result;
}

int creat(const char* path, mode_t mode)
{
    char real_path[STRLEN] = { '\0' };
    _get_real_path(path, real_path);

    int (*origin_function)(const char*, mode_t);
    origin_function = dlsym(RTLD_NEXT, "creat");
    int result = (*origin_function)(path, mode);

    _log("creat(\"%s\", %o) = %d\n", real_path, mode, result);

    return result;
}

int fclose(FILE* stream)
{
    char file_path[STRLEN] = { '\0' };
    _get_file_path(stream, file_path);

    int (*origin_function)(FILE*);
    origin_function = dlsym(RTLD_NEXT, "fclose");
    int result = (*origin_function)(stream);

    _log("fclose(\"%s\") = %d\n", file_path, result);

    return result;
}

FILE* fopen(const char* pathname, const char* mode)
{
    char real_path[STRLEN] = { '\0' };
    _get_real_path(pathname, real_path);

    FILE* (*origin_function)(const char*, const char*);
    origin_function = dlsym(RTLD_NEXT, "fopen");
    FILE* result = (*origin_function)(pathname, mode);

    _log("fopen(\"%s\", \"%s\") = %p\n", real_path, mode, result);

    return result;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    char file_path[STRLEN] = { '\0' };
    _get_file_path(stream, file_path);

    size_t (*origin_function)(void*, size_t, size_t, FILE*) = dlsym(RTLD_NEXT, "fread");
    size_t result = (*origin_function)(ptr, size, nmemb, stream);

    char buffer[33] = { '\0' };
    _check((char*)ptr, buffer);

    _log("fread(\"%s\", %lu, %lu, \"%s\") = %lu\n", buffer, size, nmemb, file_path, result);

    return result;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    char file_path[STRLEN] = { '\0' };
    _get_file_path(stream, file_path);

    size_t (*origin_function)(const void*, size_t, size_t, FILE*);
    origin_function = dlsym(RTLD_NEXT, "fwrite");
    size_t result = (*origin_function)(ptr, size, nmemb, stream);

    char buffer[33] = { '\0' };
    _check((char*)ptr, buffer);

    _log("fwrite(\"%s\", %lu, %lu, \"%s\") = %lu\n", buffer, size, nmemb, file_path, result);

    return result;
}

int open(const char* path, int oflag, ...)
{
    char real_path[STRLEN] = { '\0' };
    _get_real_path(path, real_path);

    int mode = 0;

    if (__OPEN_NEEDS_MODE(oflag)) {
        va_list arg;

        va_start(arg, oflag);
        mode = va_arg(arg, int);
        va_end(arg);
    }

    int (*origin_function)(const char*, int, ...);
    origin_function = dlsym(RTLD_NEXT, "open");
    int result = (*origin_function)(path, oflag, mode);

    _log("open(\"%s\", %o, %o) = %d\n", real_path, oflag, mode, result);

    return result;
}

ssize_t read(int fildes, void* buf, size_t nbyte)
{
    char fd_path[STRLEN] = { '\0' };
    _get_fd_path(fildes, fd_path);

    ssize_t (*origin_function)(int, void*, size_t);
    origin_function = dlsym(RTLD_NEXT, "read");
    ssize_t result = (*origin_function)(fildes, buf, nbyte);

    char buffer[33] = { '\0' };
    _check((char*)buf, buffer);

    _log("read(\"%s\", \"%s\", %lu) = %lu\n", fd_path, buffer, nbyte, result);

    return result;
}

int remove(const char* pathname)
{
    char real_path[STRLEN] = { '\0' };
    _get_real_path(pathname, real_path);

    int (*origin_function)(const char*);
    origin_function = dlsym(RTLD_NEXT, "remove");
    int result = (*origin_function)(pathname);

    _log("remove(\"%s\") = %d\n", real_path, result);

    return result;
}

int rename(const char* old, const char* new)
{
    char real_path_1[STRLEN] = { '\0' };
    char real_path_2[STRLEN] = { '\0' };
    _get_real_path(old, real_path_1);
    _get_real_path(new, real_path_2);

    int (*origin_function)(const char*, const char*);
    origin_function = dlsym(RTLD_NEXT, "rename");
    int result = (*origin_function)(old, new);

    _log("rename(\"%s\", \"%s\") = %d\n", real_path_1, real_path_2, result);

    return result;
}

FILE* tmpfile(void)
{
    FILE* (*origin_function)(void);
    origin_function = dlsym(RTLD_NEXT, "tmpfile");
    FILE* result = (*origin_function)();

    _log("tmpfile() = %p\n", result);

    return result;
}

ssize_t write(int fildes, const void* buf, size_t nbyte)
{
    char fd_path[STRLEN] = { '\0' };
    _get_fd_path(fildes, fd_path);

    ssize_t (*origin_function)(int, const void*, size_t);
    origin_function = dlsym(RTLD_NEXT, "write");
    ssize_t result = (*origin_function)(fildes, buf, nbyte);

    char buffer[33] = { '\0' };
    _check((char*)buf, buffer);

    _log("write(\"%s\", \"%s\", %lu) = %lu\n", fd_path, buffer, nbyte, result);

    return result;
}
