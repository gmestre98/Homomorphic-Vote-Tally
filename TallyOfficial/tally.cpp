/******************************************************************************
*
* File Name: tally.cpp
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  07 Dec 2019
*
* NAME
*  tally - Verification of the validity of the votes and calculation of the
* checksum for each vote and the election results
*
* DESCRIPTION
*  The execution of this file verifies the signature on each vote file on the
* Ballot Box and removes the unsigned ones. It also chooses the last vote to be
* made for each voter and then does the checksum for each of the chosen ones.
* after this it computes the election results.
*
******************************************************************************/
#include <iostream>
#include "tallyfileops.h"
#include <string>
#include <seal/seal.h>

using namespace std;

int main(int argc, char*argv[]){
  int sizetxt = 0, sizesha256 = 0;
  char txt[] = ".txt", sha256[] = ".sha256";
  char** filestxt = mallarraystrings(MAX_VOTES, MAX_VOTES, "filestxt array!!");
  char** fsha256 = mallarraystrings(MAX_VOTES, MAX_VOTES, "sha256 array!!");
  int validsize = 0;
  char** validfiles;
  int nvoters, ncandidates;
  int* voterfilter;
  char** signfiles;
  int valsignsize = 0;
  char** valcontent;
  int valcontentsize = 0;
  char** votesfinal;
  int* electionvotes;

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
  electionvotes = (int*)malloc(nvoters*sizeof(int));
  if(electionvotes == NULL){
    cout << "ERROR ALLOCATING INT ARRAY!\n";
    exit(-1);
  }

  copyballot();
  get_ballot_files(&sizetxt, txt, filestxt); // Gets the files with txt extension
  get_ballot_files(&sizesha256, sha256, fsha256); // Gets the files with sha256 extension
  validfiles = validtxts(filestxt, fsha256, sizetxt, sizesha256, &validsize);
  voterfilter = voterfiltervalids(validfiles, validsize, nvoters);
  signfiles = verifysign(voterfilter, validsize, validfiles, &valsignsize);
  valcontent = verifycontent(signfiles, valsignsize, &valcontentsize);
  votesfinal = verifytime(valcontent, valcontentsize, nvoters);
  verifykeyssigns();
  for(int i=0; i < nvoters; i = i + 1)
    checksumvote(votesfinal[i], electionvotes, i, ncandidates);
  verifyweightfiles(nvoters);
  electionresults(votesfinal, electionvotes, nvoters);
}
