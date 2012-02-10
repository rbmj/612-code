#include "612.h"

#include <cstdio>

void perror_612_(const char * file, int line, const char * msg) {
    //This should be std::fprintf(stderr, ...), but i'm not sure if NetConsole
    //will work with stderr or if it just gets stdout
    std::printf("||ERROR at line %i in file %s: %s\n", line, file, msg);
}
