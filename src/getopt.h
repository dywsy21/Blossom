#ifndef GETOPT_H
#define GETOPT_H
#include <cstddef>

int getopt(int argc, char* const argv[], const char* optstring);

extern char* optarg;
extern int optind;

#endif // GETOPT_H