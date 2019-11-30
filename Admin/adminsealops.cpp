#include "adminsealops.h"

using namespace std;
using namespace seal;

void generate_election_keys(){

  // Defining the context and parameters for the key generation
  EncryptionParameters parms(scheme_type::BFV);
  size_t poly_modulus_degree = 4096;
  parms.set_poly_modulus_degree(poly_modulus_degree);
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
  parms.set_plain_modulus(1021);
  auto context = SEALContext::Create(parms);
  PublicKey election_public_key;
  SecretKey election_secret_key;

  // Key Generation
  KeyGenerator keygen(context);
  election_public_key = keygen.public_key();
  election_secret_key = keygen.secret_key();

  // Saving the keys to files
  ofstream publickeyfile;
  publickeyfile.open("election_public_key.txt", ofstream::binary);
  election_public_key.save(publickeyfile);
  publickeyfile.close();
  ofstream secretkeyfile;
  secretkeyfile.open("election_secret_key.txt", ofstream::binary);
  election_secret_key.save(secretkeyfile);
  secretkeyfile.close();

  // Moving the files to the right place
  system("sudo cp election_public_key.txt ../Proj/Keys");
  system("sudo rm -r election_public_key.txt");
  system("sudo cp election_secret_key.txt ../Proj/Keys");
  system("sudo rm -r election_secret_key.txt");
}



void weight_encryption(int nvoters, int* weights){

  // Setting the parameters for the encryption
  EncryptionParameters parms(scheme_type::BFV);
  size_t poly_modulus_degree = 4096;
  parms.set_poly_modulus_degree(poly_modulus_degree);
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
  parms.set_plain_modulus(1021);
  auto context = SEALContext::Create(parms);



  Ciphertext encrypted_weight;
  ifstream pkey;
  pkey.open("../Proj/Keys/election_public_key.txt", ios::binary);
  PublicKey election_public_key;
  election_public_key.load(context, pkey);
  Encryptor encryptor(context, election_public_key);
  string f = "ecrypted_voter_weights";
  string txt = ".txt";
  string a;
  string forcabruta = "sudo cp ";
  string forcamenos;
  string dest = " ../Proj/Keys";
  string xau = "sudo rm -r ";
  string ciao;


  for(int i=0; i < nvoters; i = i + 1){
    ofstream file;
    Plaintext xplain(to_string(weights[i]));
    encryptor.encrypt(xplain, encrypted_weight);
    a = f;
    a.append(to_string(i + 1));
    a.append(txt);
    file.open(a.c_str(), ofstream::binary);
    encrypted_weight.save(file);
    file.close();

    forcamenos = forcabruta;
    forcamenos.append(a);
    forcamenos.append(dest);
    system(forcamenos.c_str());
    ciao = xau;
    ciao.append(a);
    system(ciao.c_str());
  }
}
