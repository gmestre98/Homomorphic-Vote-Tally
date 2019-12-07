/******************************************************************************
*
* File Name: dealwithvotes.h
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  07 Dec 2019
*
* NAME
*  dealwithvotes - Header file for the dealwithvotes.cpp file
*
* DESCRIPTION
* This file is just an header file for the dealwithvotes.cpp, with just the
* necessary declarations and includes
*
*****************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "seal/seal.h"

using namespace std;

bool isNumber(string);
void readvotes(int, int*);
string system_listen(string);
void verifypublickey(int);
void verifyvotercert(int);
void verifyvoterkey(int);
string timestr(time_t);
void givecertstotally(int);
time_t encryptvote(int*, int, int);
void signvote(int, time_t);
