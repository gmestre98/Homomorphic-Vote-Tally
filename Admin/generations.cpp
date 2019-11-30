#include "generations.h"

using namespace std;

void generaterootca(){
  system("cd .. && sudo rm -r Proj");
  system("cd .. && sudo mkdir Proj");
  system("cd .. && sudo mkdir Proj/CA");
  system("cd ../Proj/CA && sudo openssl genrsa -des3 -out my-ca.key 2048");
  system("cd ../Proj/CA && sudo openssl req -new -x509 -days 3650 -key my-ca.key -out my-ca.crt");
}

void generateprivatekey(){
  system("sudo mkdir ../Proj/Keys");
  system("cd ../Proj/Keys && sudo openssl genrsa -out privatekey.pem 1024");
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

  openvotefolder = "cd "; //open the directory
  votefolder = "../Proj/Voters/";
  votefolder.append(std::to_string(voter + 1));
  openvotefolder.append(votefolder);
  command = " && sudo openssl genrsa -out voter-cert.key 1024";
  openvotefolder.append(command);
  system(openvotefolder.c_str());
}

void voter_certificate(int voter){
  string votefolder;
  string command;
  string openvotefolder;
  string votercertdata;

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
}


void installvoterrootCA(int voter){
  string command;

  command = "sudo cp ../Proj/CA/my-ca.crt ../Proj/Voters/";
  command.append(to_string(voter + 1));
  system(command.c_str());
}


void sign_election_public_key(){
  string command;

  command = "sudo openssl dgst -sha256 -sign ../Proj/CA/my-ca.key -out /tmp/sign.sha256 ../Proj/Keys/election_public_key.txt";
  system(command.c_str());
  command = "sudo openssl base64 -in /tmp/sign.sha256 -out ../Proj/Keys/pub_key_sign";
  system(command.c_str());
}

void install_election_public_key(int voter){
  string command;

  command = "sudo cp ../Proj/Keys/election_public_key.txt ../Proj/Voters/";
  command.append(to_string(voter + 1));
  system(command.c_str());
  command = "sudo cp ../Proj/Keys/pub_key_sign ../Proj/Voters/";
  command.append(to_string(voter + 1));
  system(command.c_str());
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
