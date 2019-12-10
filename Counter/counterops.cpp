/******************************************************************************
*
* File Name: counterops.cpp
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  10 Dec 2019
*
* NAME
*  counterops - Implementation of all the functions necessary for the counter
* execution
*
* DESCRIPTION
*  This code file implements the functionalities of verifying if a string is a
* number, verifying the trustees and after that rebuilding the symmetric key and
* restoring the secret key. It also implements the functionalities of checking
* the chekcusm and anouncing the election results
*
******************************************************************************/
#include "counterops.h"

using namespace std;
using namespace seal;

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
 * system_listen()
 *
 * Arguments: - Terminal command line to be executed
 * Returns: String with the terminal response for the executed command
 *
 * Description: This function executes a system call and returns the system call
 *  answer
 *
 *****************************************************************************/
string system_listen(string cmd){
    string data;
    FILE *stream;
    const int max_buffer = 512;
    char buffer [max_buffer];

    stream = popen(cmd.c_str(), "r");

    if (stream){
      while(!feof(stream)){
        if (fgets(buffer, max_buffer, stream) != NULL)
          data.append(buffer);
      }
      pclose(stream);
    }
    return data;
}

/******************************************************************************
 * verifytrustees()
 *
 * Arguments: - none
 * Returns: none
 *
 * Description: This function verifies if all the trustees are signed by the
 *  root CA
 *
 *****************************************************************************/
void verifytrustees(){
  string c1 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Trustees/election_secret_key.sha256 ../Proj/Trustees/election_secret_key.enc";
  system("sudo openssl x509 -pubkey -noout -in ../Proj/Counter/my-ca.crt > pubkey.pem");
  if(system_listen(c1) != "Verified OK\n"){
    cout << "\n You have got an election encrypted key not signed by the CA!\n";
    cout << "Program exiting\n";
    exit(-1);
  }
  for(int i=0; i < 5; i = i + 1){
    c1 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Trustees/sharefile" + to_string(i+1) + ".sha256 ../Proj/Trustees/sharefile" + to_string(i+1) + ".txt";
    if(system_listen(c1) != "Verified OK\n"){
      cout << "\n You have got an share file not signed by the CA!\n";
      cout << "Program exiting\n";
      exit(-1);
    }
  }
  system("sudo rm -r pubkey.pem");
}


/******************************************************************************
 * rebuildkey()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: This function restores the symmetric key used to encrypt the
 *  election secret key
 *
 *****************************************************************************/
void rebuildkey(){
  uint8_t restored[sss_MLEN];
	sss_Share shares[5];
  sss_Share s;
  string aux;
  char buffer[113];
	int tmp;

  for(int i=0; i < 5; i = i + 1){
    ifstream sharefile;
    sharefile.open(("../Proj/Trustees/sharefile" + to_string(i+1) + ".txt").c_str(), ios::binary);
    sharefile.read(buffer, 113);
    sharefile.close();
    memcpy(&(shares[i]), &buffer, 113);
  }

  // Combine some of the shares to restore the original secret
  tmp = sss_combine_shares(restored, shares, 5);
  assert(tmp == 0);

  memcpy (&buffer, &restored, sss_MLEN*sizeof(uint8_t));
  ofstream file;
  file.open("restored.txt", ofstream::binary);
  file.write((char*)buffer, sss_MLEN*sizeof(uint8_t));
  file.close();

  string encrypt = "sudo openssl enc -aes-256-cbc -d -pbkdf2 -kfile restored.txt -in ../Proj/Trustees/election_secret_key.enc -out election_secret_key.txt";
  system(encrypt.c_str());
  system("sudo rm -r restored.txt");
}


/******************************************************************************
 * checksumcheck()
 *
 * Arguments: - Number of candidates on this election
 *            - Number of voters on this election
 * Returns: none
 *
 * Description: This function gets the checksum from each files and verifies if
 *  every voter voted the correct number of times
 *
 *****************************************************************************/
void checksumcheck(int ncandidates, int nvoters){
  // Setting the parameters for the decoding and decryption
  EncryptionParameters parms(scheme_type::BFV);
  size_t poly_modulus_degree = 8192;
  parms.set_poly_modulus_degree(poly_modulus_degree);
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
  parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
  auto context = SEALContext::Create(parms);
  SecretKey secret_key;
  BatchEncoder batch_encoder(context);
  // Getting the secret key from it's file
  ifstream pkey;
  pkey.open("election_secret_key.txt", ios::binary);
  secret_key.load(context, pkey);
  pkey.close();
  Decryptor decryptor(context, secret_key);

  for(int i=0; i < nvoters; i = i + 1){
    int a = 0;
    Ciphertext checksumvec;
    Plaintext result;
    vector<uint64_t> final;
    ifstream voter;
    voter.open(("../Proj/Counter/Voter" + to_string(i+1) + ".txt").c_str(), ios::binary);
    if(voter.is_open()){
      checksumvec.load(context, voter);
      voter.close();
      decryptor.decrypt(checksumvec, result);
      batch_encoder.decode(result, final);
      a = (int) final[0];
      if(a != ncandidates){
        cout << "SOMEONE IS TAMPERING WITH THE ELECTION, KILL THIS, FIND THE GUILTY GUY!\n";
        exit(-1);
      }
    }
  }
}


/******************************************************************************
 * electionresults()
 *
 * Arguments: - Number of candidates on this election
 * Returns: none
 *
 * Description: This function reads the election results from the file and
 *  anounces them
 *
 *****************************************************************************/
void electionresults(int ncandidates){
  // Setting the parameters for the decoding and decryption
  EncryptionParameters parms(scheme_type::BFV);
  size_t poly_modulus_degree = 8192;
  parms.set_poly_modulus_degree(poly_modulus_degree);
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
  parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
  auto context = SEALContext::Create(parms);
  SecretKey secret_key;
  BatchEncoder batch_encoder(context);
  // Getting the secret key from it's file
  ifstream pkey;
  pkey.open("election_secret_key.txt", ios::binary);
  secret_key.load(context, pkey);
  pkey.close();
  ifstream file;
  Decryptor decryptor(context, secret_key);
  Ciphertext encresults;
  Plaintext encodedresults;
  vector<uint64_t> finalresults;
  int* winnerkeeper = (int*)malloc(ncandidates*sizeof(int));
  if(winnerkeeper == NULL){
    cout << "ALLOCATION FAILURE AT THE END!\n";
    exit(-1);
  }

  file.open("../Proj/Counter/results.txt", ios::binary);
  if(file.is_open()){
    encresults.load(context, file);
    file.close();
    decryptor.decrypt(encresults, encodedresults);
    batch_encoder.decode(encodedresults, finalresults);
    for(int i=0; i < ncandidates; i = i + 1){
      winnerkeeper[i] = (int)finalresults[i];
      cout << "Votes on candidate " << i + 1 << ": " << winnerkeeper[i] << "\n";
    }
  }

}
