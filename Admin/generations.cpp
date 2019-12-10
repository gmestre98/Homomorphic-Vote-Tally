/******************************************************************************
*
* File Name: generations.cpp
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  10 Dec 2019
*
* NAME
*  adminsealops - Declaration of the functions needed for general admin operations
* that do not need the SEAL MICROSOFT library
*
* DESCRIPTION
*  Most of the functions declaread on this file are for general verifications
* that the data needs to be subjected too, but also for folder creations,
* certififcates and keys creations with openssl and signing documents
*
*****************************************************************************/
#include "generations.h"

using namespace std;


/******************************************************************************
 * isNumber()
 *
 * Arguments: string inputed
 * Returns: boolean true if s is a number and false if not
 *
 * Description: This function verifies if the inputed string is a number and
 *  returns a boolean according to that
 *
 *****************************************************************************/
bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;
}

/******************************************************************************
 * generaterootca()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: Creates the folder for this election and the CA folder inside it.
 * This function also creates the key and certificate for the root CA
 *
 *****************************************************************************/
void generaterootca(){
  system("cd .. && sudo rm -r Proj");
  system("cd .. && sudo mkdir Proj");
  system("cd .. && sudo mkdir Proj/CA");
  system("cd ../Proj/CA && sudo openssl genrsa -out my-ca.key 2048");
  system("cd ../Proj/CA && sudo openssl req -new -x509 -days 3650 -key my-ca.key -out my-ca.crt");
}

/******************************************************************************
 * installrootCAtally()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: It creates the Tally folder for this election and saves there
 *  the root CA certificate
 *
 *****************************************************************************/
void installrootCAtally(){
  system("sudo mkdir ../Proj/Tally");
  system("sudo cp ../Proj/CA/my-ca.crt ../Proj/Tally");
  system("sudo mkdir ../Proj/Tally/Voters");
}

/******************************************************************************
 * createvoterfolder()
 *
 * Arguments: - Voter id
 * Returns: none
 *
 * Description: Creates a folder for the voter with the corresponding voter id
 *
 *****************************************************************************/
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

/******************************************************************************
 * voter_privatekey()
 *
 * Arguments: - Voter id
 * Returns: none
 *
 * Description: Creates a private key for the voter with the corresponding voter
 *  id and also signs the private key with the root CA private key
 *
 *****************************************************************************/
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

/******************************************************************************
 * voter_certificate()
 *
 * Arguments: - Voter id
 * Returns: none
 *
 * Description: Creates a certificate for the voter with the corresponding voter
 *  id and also signs the certificate with the root CA private key. The
 *  certificate is fullfiled automaticaly
 *
 *****************************************************************************/
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

/******************************************************************************
 * installvoterrootCA()
 *
 * Arguments: - Voter id
 * Returns: none
 *
 * Description: Copies the root CA certificate to the folder of the voter with
 *  the corresponding voter id
 *
 *****************************************************************************/
void installvoterrootCA(int voter){
  string command;

  command = "sudo cp ../Proj/CA/my-ca.crt ../Proj/Voters/";
  command.append(to_string(voter + 1));
  system(command.c_str());
}

/******************************************************************************
 * install_election_public_key()
 *
 * Arguments: - Voter id
 * Returns: none
 *
 * Description: Copies the election public key to the folder of the voter with
 *  the corresponding voter id and also signs this election public key file
 *
 *****************************************************************************/
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

/******************************************************************************
 * votertallyfolder()
 *
 * Arguments: - Voter id
 * Returns: none
 *
 * Description: Creates a folder for the voter with the correpsonding voter id
 *  on the voter tally folder
 *
 *****************************************************************************/
void votertallyfolder(int voterid){
  string cert1 = "sudo mkdir ../Proj/Tally/Voters/";

  cert1.append(to_string(voterid + 1));
  system(cert1.c_str());
}

/******************************************************************************
 * weight_assign()
 *
 * Arguments: - Voter id
 *            - Weights vector with the weight for the votes of each voter
 * Returns: none
 *
 * Description: Asks the user to introduce a number between 1 and 100 for then
 *  weight of each voter on this election
 *
 *****************************************************************************/
void weight_assign(int nvoters, int *weights){
  string a;

  for (int i=0; i < nvoters; i = i + 1){
    cout << "Insert a weight between 1 and 100 of the vote for voter " << i + 1 << ":\n";
    do{
      cin >> a;
    }while(!isNumber(a) || stoi(a) < 1  ||  stoi(a) > 100);
    weights[i] = stoi(a);
  }
}

/******************************************************************************
 * setlastfolders()
 *
 * Arguments: - Number of voters
 * Returns: none
 *
 * Description: Creates a folder inside the election folder with the Ballot Box
 *  where each vote and the corresponding signature is going to
 *
 *****************************************************************************/
void setlastfolders(){
  // Create Ballot Box
  system("sudo mkdir ../Proj/BallotBox");
}


/******************************************************************************
 * installtallykeys()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: This function signs the files with the relin keys and the galois
 *  keys and installs both the files and the signatures on the tally folder
 *
 *****************************************************************************/
void installtallykeys(){
  system("sudo cp ../Proj/Keys/relin_keys.txt ../Proj/Tally");
  system("sudo cp ../Proj/Keys/galois_keys.txt ../Proj/Tally");
  system("sudo openssl dgst -sha256 -sign ../Proj/CA/my-ca.key -out relin.sha256 ../Proj/Tally/relin_keys.txt");
  system("sudo openssl dgst -sha256 -sign ../Proj/CA/my-ca.key -out galois.sha256 ../Proj/Tally/galois_keys.txt");
  system("sudo mv relin.sha256 ../Proj/Tally");
  system("sudo mv galois.sha256 ../Proj/Tally");
}


/******************************************************************************
 * installrootCAtally()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: It creates the Tally folder for this election and saves there
 *  the root CA certificate
 *
 *****************************************************************************/
void installrootCAcounter(){
  system("sudo mkdir ../Proj/Counter");
  system("sudo cp ../Proj/CA/my-ca.crt ../Proj/Counter");
}
