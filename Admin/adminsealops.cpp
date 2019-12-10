/******************************************************************************
*
* File Name: adminsealops.cpp
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  10 Dec 2019
*
* NAME
*  adminsealops - Declaration of the functions needed for the admin operations
* that have operations using the SEAL MICROSOFT library.
*
* DESCRIPTION
*  This file implements important functions for the election system, including
* the generation of the election keys, the creation of the trustees and the
* encryption of the vector of the voters weights
*
*****************************************************************************/
#include "adminsealops.h"

using namespace std;
using namespace seal;


/******************************************************************************
 * generate_election_keys()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: Generates the chosen context and the keys for this context.
 *  Then this saves the generated keys to the corresponding files and puts each
 *  file in the right place.
 *
 *****************************************************************************/
void generate_election_keys(){
  // Defining the context and parameters for the key generation
  EncryptionParameters parms(scheme_type::BFV);
  size_t poly_modulus_degree = 8192;
  parms.set_poly_modulus_degree(poly_modulus_degree);
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
  parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
  auto context = SEALContext::Create(parms);
  // Key Generation
  KeyGenerator keygen(context);
  PublicKey election_public_key = keygen.public_key();
  SecretKey election_secret_key = keygen.secret_key();
  RelinKeys relin_keys = keygen.relin_keys();
  GaloisKeys gal_keys = keygen.galois_keys();

  // Saving the keys to files
  ofstream publickeyfile;
  publickeyfile.open("election_public_key.txt", ofstream::binary);
  election_public_key.save(publickeyfile);
  publickeyfile.close();
  ofstream secretkeyfile;
  secretkeyfile.open("election_secret_key.txt", ofstream::binary);
  election_secret_key.save(secretkeyfile);
  secretkeyfile.close();
  ofstream relinkeysfile;
  relinkeysfile.open("relin_keys.txt", ofstream::binary);
  relin_keys.save(relinkeysfile);
  relinkeysfile.close();
  ofstream galoiskeysfile;
  galoiskeysfile.open("galois_keys.txt", ofstream::binary);
  gal_keys.save(galoiskeysfile);
  galoiskeysfile.close();

  // Moving the files to the right place
  system("sudo mkdir ../Proj/Keys");
  system("sudo cp election_public_key.txt ../Proj/Keys");
  system("sudo rm -r election_public_key.txt");
  system("sudo cp election_secret_key.txt ../Proj/Keys");
  system("sudo rm -r election_secret_key.txt");
  system("sudo cp relin_keys.txt ../Proj/Keys");
  system("sudo rm -r relin_keys.txt");
  system("sudo cp galois_keys.txt ../Proj/Keys");
  system("sudo rm -r galois_keys.txt");
}


/******************************************************************************
 * generate_symetric_key()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: Generates the symetric key 64 bits and an iv to store it in
 *  a file that will be modified to contain only the key itself.
 *  Then encrypts the secret key election with the previously generated key
 *  and save the operation result in the file named
 *   encrypted_election_secret_key.txt.
 *
 *****************************************************************************/
void generate_symetric_key(){
  string generatekey = "openssl rand 64 > sym_keyfile.txt";
  string encrypt = "sudo openssl enc -aes-256-cbc -pbkdf2 -kfile sym_keyfile.txt -in ../Proj/Keys/election_secret_key.txt -out ../Proj/Keys/election_secret_key.enc";

  system(generatekey.c_str());
  system(encrypt.c_str());
}

/******************************************************************************
 * breaksecretkey()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: This function calls the function that generates the file with
 *  the symmetric key, then it breaks the content of this file into 5 shares and
 *  stores it on the trustees and signs the content
 *
 *****************************************************************************/
void breaksecretkey(){
  char buf[64];
  uint8_t message[sss_MLEN];
  sss_Share shares[5];

  generate_symetric_key();
  // Getting the secret key from its file
  ifstream skey;
  skey.open("sym_keyfile.txt", ios::binary);
  skey.read(buf, 64);
  skey.close();
  memcpy (&message, &buf, sizeof(buf));
  sss_create_shares(shares, message, 5, 5);
  system("cd .. && sudo mkdir Proj/Trustees");
  system("sudo mv ../Proj/Keys/election_secret_key.enc ../Proj/Trustees");
  for(int i=0; i < 5; i = i + 1){
    ofstream sharefile;
    string aaa = "sharefile";
    string txt = ".txt";
    string mv1 = "sudo mv ";
    string mv2 = " ../Proj/Trustees";
    aaa.append(to_string(i + 1));
    aaa.append(txt);
    sharefile.open(aaa, ofstream::binary);
    sharefile.write((char*)shares[i], sizeof(sss_Share));
    sharefile.close();
    mv1.append(aaa);
    mv1.append(mv2);
    system(mv1.c_str());
    system(("sudo openssl dgst -sha256 -sign ../Proj/CA/my-ca.key -out ../Proj/Trustees/sharefile" + to_string(i+1) + ".sha256 ../Proj/Trustees/sharefile" + to_string(i+1) + ".txt").c_str());
  }
  system("sudo rm -r sym_keyfile.txt");
  system("sudo rm -r ../Proj/Keys/election_secret_key.txt");
  system("sudo openssl dgst -sha256 -sign ../Proj/CA/my-ca.key -out ../Proj/Trustees/election_secret_key.sha256 ../Proj/Trustees/election_secret_key.enc");
}


/******************************************************************************
 * weight_encryption()
 *
 * Arguments: - The number of voters on this election
 *            - Array of integers with the weights for each voter
 * Returns: none
 *
 * Description: Encrypts the weights array and saves the encrypted data in a
 *  file, then saves the file on the right place
 *
 *****************************************************************************/
void weight_encryption(int nvoters, int* weights){
  // Setting the parameters for the encryption
  EncryptionParameters parms(scheme_type::BFV);
  size_t poly_modulus_degree = 8192;
  parms.set_poly_modulus_degree(poly_modulus_degree);
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
  parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
  auto context = SEALContext::Create(parms);

  // Getting the public key from the corresponding file
  ifstream pkey;
  pkey.open("../Proj/Keys/election_public_key.txt", ios::binary);
  PublicKey election_public_key;
  election_public_key.load(context, pkey);
  pkey.close();


  // Setting the encryption process
  Encryptor encryptor(context, election_public_key);
  for(int i=0; i < nvoters; i = i + 1){
    Plaintext xplain(to_string(weights[i]));
    Ciphertext xencrypted;
    ofstream weightsfile;
    encryptor.encrypt(xplain, xencrypted);
    weightsfile.open((to_string(i+1) + "_weights.txt").c_str(), ofstream::binary);
    weightsfile << i + 1 << "\n";
    xencrypted.save(weightsfile);
    weightsfile.close();
    system(("sudo mv " + to_string(i+1) + "_weights.txt ../Proj/Tally").c_str());
    system(("sudo openssl dgst -sha256 -sign ../Proj/CA/my-ca.key -out " + to_string(i+1) + "_weights.sha256 ../Proj/Tally/" + to_string(i+1) + "_weights.txt").c_str());
    system(("sudo mv " + to_string(i+1) + "_weights.sha256 ../Proj/Tally").c_str());
  }
}
