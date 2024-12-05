#include "getopt.h"

const char* strchr(const char* str, int c) {
    while (*str != '\0') {
        if (*str == c) {
            return str;
        }
        str++;
    }
    return nullptr;
}

int getopt(int argc, char *const argv[], const char *optstring) {
    static int optind = 1;
    static int optopt;
    static char *next = NULL;

    if (optind >= argc) {
        return -1;
    }

    if (next == NULL || *next == '\0') {
        next = argv[optind];
        if (*next != '-') {
            return -1;
        }
        next++;
        optind++;
    }

    optopt = *next++;
    const char *opt = strchr(optstring, optopt);
    if (opt == NULL) {
        return '?';
    }

    if (*(opt + 1) == ':') {
        if (*next != '\0') {
            optarg = next;
            next = NULL;
        } else if (optind < argc) {
            optarg = argv[optind];
            optind++;
        } else {
            return ':';
        }
    }

    return optopt;
}