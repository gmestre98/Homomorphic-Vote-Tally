#include <iostream>
#include <string.h>
#include <dirent.h>

#define MAX_VOTES 200


char** get_ballot_files(int*, char*, char**);
void verifysigns(char**, char**, int, int);
