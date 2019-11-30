#include "adminsealops.h"

using namespace std;
using namespace seal;

void generate_election_keys(){

  // Defining the context and parameters for the key generation
  EncryptionParameters parms(scheme_type::BFV);
  size_t poly_modulus_degree = 4096;
  parms.set_poly_modulus_degree(poly_modulus_degree);
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
  parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
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
  BatchEncoder batch_encoder(context);
  Plaintext plainvector;
  Ciphertext encrypted_matrix;
  size_t slot_count = batch_encoder.slot_count();
  vector<uint64_t> pod_matrix(slot_count, 0ULL);

  for(int i=0; i < nvoters; i = i + 1){
    pod_matrix[i] = (uint64_t) weights[i];
  }

  batch_encoder.encode(pod_matrix, plainvector);
  encryptor.encrypt(plainvector, encrypted_matrix);

  ofstream weights_file;
  weights_file.open("weights_file.txt", ofstream::binary);
  encrypted_matrix.save(weights_file);
  weights_file.close();

  // Moving the file to the right place
  system("sudo cp weights_file.txt ../Proj/Tally");
  system("sudo rm -r weights_file.txt");
}
