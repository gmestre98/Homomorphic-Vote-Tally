/******************************************************************************
*
* File Name: counter.cpp
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  10 Dec 2019
*
* NAME
*  counter - Execution of all the functions needed to decrypt and read the
* checksum and the election results
*
* DESCRIPTION
*  This code file implements the functionalities of verifying all the trustees,
* rebuilding the key and then restoring the secret key. After that it checks the
* checksum for each voter and then if everything is ok, it anounces the election
* results
*
******************************************************************************/

#include <iostream>
#include "counterops.h"

int main(int argc, char*argv[]){
  int ncandidates, nvoters;

  if(argc < 3){
    cout << "Introduce as an argument the number of voters and the number of";
    cout << " candidates for the program to run!\n";
    exit(-1);
  }
  string s1(argv[1]);
  string s2(argv[2]);
  if(!isNumber(s1) || !isNumber(s2)){
    cout << "Introduce numbers and nothing else as an argument!\n";
    exit(-1);
  }
  if(stoi(argv[1]) <= 0 || stoi(argv[2]) <= 0){
    cout << "The introduced arguments must be positive numbers!\n";
    exit(-1);
  }
  nvoters = stoi(argv[1]);
  ncandidates = stoi(argv[2]);

  verifytrustees();
  rebuildkey();
  checksumcheck(ncandidates, nvoters);
  electionresults(ncandidates);

  system("sudo rm -r election_secret_key.txt");
}
