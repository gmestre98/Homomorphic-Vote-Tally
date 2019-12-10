/******************************************************************************
*
* File Name: voter.cpp
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  10 Dec 2019
*
* NAME
*  admin - This allows a user to vote on the wanted election and puts the vote
* on the Ballot Box
*
* DESCRIPTION
*  The execution of this file makes the user introduce a vote, having the user
* the same number of votes as candidates. It also encrypts the vote and signs
* the encrypted vote. All given keys and certificates are verified if they are
* signed by the root CA
*
*****************************************************************************/
#include <iostream>
#include <string>
#include <fstream>
#include "seal/seal.h"
#include "dealwithvotes.h"

using namespace std;

int main(int argc, char*argv[]){

  int ncandidates, voterid;
  int* votes;
  time_t votetime;

  if(argc < 3){
    cout << "Introduce as an argument the number of candidates and the voter id";
    cout << " for the program to run!\n";
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
  ncandidates = stoi(argv[1]);
  voterid = stoi(argv[2]);

  votes = (int*)malloc(ncandidates*sizeof(int));
  if(votes == NULL)
    cout << "votes vector error initializing!\n";
  memset(votes, 0, ncandidates*sizeof(int));

  readvotes(ncandidates, votes);
  verifypublickey(voterid);
  verifyvoterkey(voterid);
  verifyvotercert(voterid);
  givecertsto_tally(voterid);
  votetime = encryptvote(votes, ncandidates, voterid);
  signvote(voterid, votetime);
}
