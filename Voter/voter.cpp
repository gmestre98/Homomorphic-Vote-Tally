#include <iostream>
#include <string>
#include <fstream>
#include "seal/seal.h"
#include "dealwithvotes.h"

int main(int argc, char*argv[]){

  int ncandidates = stoi(argv[1]);
  int voterid = stoi(argv[2]);
  int* votes;

  votes = (int*)malloc(ncandidates*sizeof(int));
  if(votes == NULL)
    cout << "votes vector error initializing!\n";

  readvotes(ncandidates, votes);
  verifypublickey();
  encryptvote(votes);
}
