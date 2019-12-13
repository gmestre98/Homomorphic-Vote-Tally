/******************************************************************************
*
* File Name: admin.cpp
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  10 Dec 2019
*
* NAME
*  admin - Execution of all the functions needed to setup the Homomorphic Vote
* System for one election.
*
* DESCRIPTION
*  The execution of this file creates a set of folders necessary for the
* execution of the developed election system. This sets the root CA certificate,
* root CA private key, the voters certificates and private keys. It also sets
* the election homomorphic private and public key, uses a symetric key to
* encrypt the private key and breaks it into shares. After this it also sets the
* weights for each voter and encrypts it. Besides all of the execution of this
* code also sets the necessary folders and signs the necessary documents for the
* system to run
*
******************************************************************************/
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

  int nvoters;
  int* weights = NULL;

  if(argc < 2){
    cout << "Introduce as an argument the necessary number of voters for the";
    cout << " program to run!\n";
    exit(-1);
  }
  string s(argv[1]);
  if(!isNumber(s)){
    cout << "Introduce a number and nothing else as an argument!\n";
    exit(-1);
  }
  if(stoi(argv[1]) <= 0){
    cout << "The number must be positive!\n";
    exit(-1);
  }
  nvoters = stoi(argv[1]);

  weights = (int*)malloc(nvoters*sizeof(int));
  if(weights == NULL){
    cout << "error creating weight array!\n";
    exit(-1);
  }
  
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
    votertallyfolder(i);
  }
  breaksecretkey();
  weight_assign(nvoters, weights);
  weight_encryption(nvoters, weights);
  setlastfolders();
  installtallykeys();
  installrootCAcounter();
}
