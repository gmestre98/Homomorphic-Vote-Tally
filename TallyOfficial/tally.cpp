#include <iostream>
#include "tallyfileops.h"
#include <string>
#include <seal/seal.h>

using namespace std;

int main(int argc, char*argv[]){
  int sizetxt = 0;
  int sizesha256 = 0;
  char txt[] = ".txt";
  char sha256[] = ".sha256";
  char** filestxt = mallarraystrings(MAX_VOTES, MAX_VOTES, "filestxt array!!");
  char** fsha256 = mallarraystrings(MAX_VOTES, MAX_VOTES, "sha256 array!!");
  int validsize = 0;
  char** validfiles;
  int nvoters = stoi(argv[1]);
  int ncandidates = stoi(argv[2]);
  int* voterfilter;
  char** signfiles;
  int valsignsize = 0;
  char** valcontent;
  int valcontentsize = 0;
  char** votesfinal;

  system("sudo rm -r ../Proj/Tally/BallotBox");
  system("sudo cp -r ../Proj/BallotBox ../Proj/Tally");
  get_ballot_files(&sizetxt, txt, filestxt);
  get_ballot_files(&sizesha256, sha256, fsha256);
  validfiles = validtxts(filestxt, fsha256, sizetxt, sizesha256, &validsize);
  free(filestxt);
  free(fsha256);
  voterfilter = voterfiltervalids(validfiles, validsize, nvoters);
  signfiles = verifysign(voterfilter, validsize, validfiles, &valsignsize);
  free(validfiles);
  free(voterfilter);
  valcontent = verifycontent(signfiles, valsignsize, &valcontentsize);
  free(signfiles);
  votesfinal = verifytime(valcontent, valcontentsize, nvoters);
  for(int i=0; i < nvoters; i= i + 1)
    cout << votesfinal[i] << "\n";

}
