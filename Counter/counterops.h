/******************************************************************************
*
* File Name: counterops.h
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  10 Dec 2019
*
* NAME
*  dealwithvotes - Header file for the counterops.cpp file
*
* DESCRIPTION
* This file is just an header file for the counterops.cpp, with just the
* necessary declarations and includes
*
*****************************************************************************/
#include <cstddef>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "seal/seal.h"
extern "C" {
  #include "sss.h"
  #include "randombytes.h"
  #include <assert.h>
  #include <string.h>
}

using namespace std;

bool isNumber(string);
string system_listen(string);
void verifytrustees();
void rebuildkey();
void checksumcheck(int, int);
void electionresults(int);
