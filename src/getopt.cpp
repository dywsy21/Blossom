#include "getopt.h"
#include <cstring>

char* optarg = nullptr;
int optind = 1;
int optopt = 0;
int opterr = 1;

static int optwhere = 1;

int getopt(int argc, char* const argv[], const char* optstring) {
    if (argc < 2) return -1;
    
    if (optwhere == 1) {
        if (optind >= argc || argv[optind][0] != '-' || argv[optind][1] == '\0') {
            return -1;
        }
        if (strcmp(argv[optind], "--") == 0) {
            optind++;
            return -1;
        }
    }

    optopt = argv[optind][optwhere];
    
    const char* oli = strchr(optstring, optopt);
    if (optopt == ':' || !oli) {
        if (argv[optind][++optwhere] == '\0') {
            optwhere = 1;
            optind++;
        }
        return '?';
    }

    if (oli[1] != ':') {  // Don't need argument
        if (argv[optind][++optwhere] == '\0') {
            optwhere = 1;
            optind++;
        }
        optarg = nullptr;
    } else {  // Need argument
        if (argv[optind][optwhere + 1] != '\0') {
            optarg = &argv[optind][optwhere + 1];
        } else if (++optind >= argc) {
            optwhere = 1;
            return '?';
        } else {
            optarg = argv[optind];
        }
        optwhere = 1;
        optind++;
    }
    
    return optopt;
}

// Remove the custom strchr implementation - we'll use the one from cstring
