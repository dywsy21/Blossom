#ifndef GETOPT_H
#define GETOPT_H
#include <cstddef>

extern char* optarg;
extern int optind;
extern int optopt;
extern int opterr;

int getopt(int argc, char* const argv[], const char* optstring);

#endif // GETOPT_H
