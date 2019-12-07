/******************************************************************************
*
* File Name: adminsealops.h
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  07 Dec 2019
*
* NAME
*  adminsealops - Header file for the adminsealops.cpp
*
* DESCRIPTION
* This file is just an header file for the adminsealops.cpp, with just the
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

void generate_election_keys();
void breaksecretkey();
void weight_encryption(int, int*);
