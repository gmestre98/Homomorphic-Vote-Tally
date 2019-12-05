#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <bits/stdc++.h>

#define MAX_VOTES 200

using namespace std;

char** mallarraystrings(int, int, string);
char** get_ballot_files(int*, char*, char**);
int pstrcmp( const void*, const void*);
char** validtxts(char**, char**, int, int, int*);
int* voterfiltervalids(char**, int, int);
string system_listen(string);
void verifyvotercert(int);
void verifysign(int*, int, char**);
