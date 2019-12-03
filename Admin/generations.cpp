#include "generations.h"

using namespace std;

void generaterootca(){
  system("cd .. && sudo rm -r Proj");
  system("cd .. && sudo mkdir Proj");
  system("cd .. && sudo mkdir Proj/CA");
  system("cd ../Proj/CA && sudo openssl genrsa -out my-ca.key 2048");
  system("cd ../Proj/CA && sudo openssl req -new -x509 -days 3650 -key my-ca.key -out my-ca.crt");
}

void installrootCAtally(){
  system("sudo mkdir ../Proj/Tally");
  system("sudo cp ../Proj/CA/my-ca.crt ../Proj/Tally");
}

void createvoterfolder(int voter){
  string votefolder;
  string command;
  string openvotefolder;

  command = "sudo mkdir "; //create a directory
  votefolder = "../Proj/Voters/";
  votefolder.append(to_string(voter + 1));
  command.append(votefolder);
  system(command.c_str());
}

void voter_privatekey(int voter){
  string votefolder;
  string command;
  string openvotefolder;
  string sign1 = "sudo openssl dgst -sha256 -sign ../Proj/CA/my-ca.key -out signvkey.sha256 ../Proj/Voters/";
  string sign2 = "/voter-cert.key";
  string mv1 = "sudo mv signvkey.sha256 ../Proj/Voters/";

  openvotefolder = "cd "; //open the directory
  votefolder = "../Proj/Voters/";
  votefolder.append(std::to_string(voter + 1));
  openvotefolder.append(votefolder);
  command = " && sudo openssl genrsa -out voter-cert.key 1024";
  openvotefolder.append(command);
  system(openvotefolder.c_str());
  sign1.append(to_string(voter + 1));
  sign1.append(sign2);
  system(sign1.c_str());
  mv1.append(to_string(voter + 1));
  system(mv1.c_str());
}

void voter_certificate(int voter){
  string votefolder;
  string command;
  string openvotefolder;
  string votercertdata;
  string sign1 = "sudo openssl dgst -sha256 -sign ../Proj/CA/my-ca.key -out signvcert.sha256 ../Proj/Voters/";
  string sign2 = "/voter-cert.csr";
  string mv1 = "sudo mv signvcert.sha256 ../Proj/Voters/";


  openvotefolder = "cd "; //open the directory
  votefolder = "../Proj/Voters/";
  votefolder.append(to_string(voter + 1));
  openvotefolder.append(votefolder);
  command = " && sudo openssl req -new -key voter-cert.key -out voter-cert.csr -subj ";
  openvotefolder.append(command);
  votercertdata = "/C=LX/ST=Portugal/L=Lisbon/O=CSC/OU=CProject/CN=Voter";
  votercertdata.append(std::to_string(voter + 1));
  openvotefolder.append(votercertdata);
  system(openvotefolder.c_str());
  sign1.append(to_string(voter + 1));
  sign1.append(sign2);
  system(sign1.c_str());
  mv1.append(to_string(voter + 1));
  system(mv1.c_str());
}


void installvoterrootCA(int voter){
  string command;

  command = "sudo cp ../Proj/CA/my-ca.crt ../Proj/Voters/";
  command.append(to_string(voter + 1));
  system(command.c_str());
}

void install_election_public_key(int voter){
  string command;
  string sign1 = "sudo openssl dgst -sha256 -sign ../Proj/CA/my-ca.key -out homopublic.sha256 ../Proj/Voters/";
  string sign2 = "/election_public_key.txt";
  string mv1 = "sudo mv homopublic.sha256 ../Proj/Voters/";

  command = "sudo cp ../Proj/Keys/election_public_key.txt ../Proj/Voters/";
  command.append(to_string(voter + 1));
  system(command.c_str());
  sign1.append(to_string(voter + 1));
  sign1.append(sign2);
  system(sign1.c_str());
  mv1.append(to_string(voter + 1));
  system(mv1.c_str());
}


void weight_assign(int nvoters, int *weights){
  string a;

  for (int i=0; i < nvoters; i = i + 1){
    cout << "Insert a weight between 1 and 100 of the vote for voter " << i + 1 << ":\n";
    do{
      cin >> a;
    }while(stoi(a) < 1  ||  stoi(a) > 100);
    weights[i] = stoi(a);
  }
}


void setlastfolders(int nvoters){
  // Create Ballot Box
  system("sudo mkdir ../Proj/BallotBox");
}
