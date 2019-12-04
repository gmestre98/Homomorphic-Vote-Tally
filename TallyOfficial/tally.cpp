#include <iostream>
#include "tallyfileops.h"
#include <string>
#include <seal/seal.h>

using namespace std;

int main(void){
  int sizetxt = 0;
  int sizesha256 = 0;
  char txt[] = ".txt";
  char sha256[] = ".sha256";
  char** filestxt = (char**)malloc(MAX_VOTES*sizeof(char*));
  memset(filestxt, 0, sizeof(filestxt));
  char** fsha256 = (char**)malloc(MAX_VOTES*sizeof(char*));
  memset(fsha256, 0, sizeof(filestxt));

  for(int i=0; i < MAX_VOTES; i++){
    filestxt[i] = (char*)malloc(MAX_VOTES*sizeof(char));
    fsha256[i] = (char*)malloc(MAX_VOTES*sizeof(char));
  }


  get_ballot_files(&sizetxt, txt, filestxt);
  get_ballot_files(&sizesha256, sha256, fsha256);
  //printf("%s\n", filestxt[0]);
  verifysigns(filestxt, fsha256, sizetxt, sizesha256);

  free(filestxt);
  free(fsha256);
}
