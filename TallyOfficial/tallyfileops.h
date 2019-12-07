/******************************************************************************
*
* File Name: tallyfileops.h
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  07 Dec 2019
*
* NAME
*  dealwithvotes - Header file for the tallyfileops.cpp file
*
* DESCRIPTION
* This file is just an header file for the tallyfileops.cpp, with just the
* necessary declarations and includes
*
*****************************************************************************/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include "seal/seal.h"
#include <bits/stdc++.h>

#define MAX_VOTES 200

using namespace std;
using namespace seal;

char** mallarraystrings(int, int, string);
void get_ballot_files(int*, char*, char**);
int pstrcmp( const void*, const void*);
char** validtxts(char**, char**, int, int, int*);
int* voterfiltervalids(char**, int, int);
string system_listen(string);
void verifyvotercert(int);
char** verifysign(int*, int, char**, int*);
char** verifycontent(char**, int, int*);
char** verifytime(char**, int, int);
