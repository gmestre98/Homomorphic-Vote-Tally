#include <iostream>
#include <string>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <vector>
#include "generations.h"
#include "adminsealops.h"
#include "seal/seal.h"

using namespace std;
using namespace seal;

int main(int argc, char *argv[]){

  int nvoters = stoi(argv[1]);
  int* weights = NULL;

  weights = (int*)malloc(nvoters*sizeof(int));
  if(weights == NULL)
    cout << "error creating weight array!\n";
  generaterootca();
  installrootCAtally();
  generate_election_keys();
  system("sudo mkdir ../Proj/Voters");
  for(int i=0; i < nvoters; i = i + 1){
    createvoterfolder(i);
    voter_privatekey(i);
    voter_certificate(i);
    installvoterrootCA(i);
    install_election_public_key(i);
  }
  breaksecretkey();
  weight_assign(nvoters, weights);
  weight_encryption(nvoters, weights);
  setlastfolders(nvoters);
}
