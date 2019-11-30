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


  // Getting the public key from the corresponding file
  ifstream pkey;
  pkey.open("../Proj/Keys/election_public_key.txt", ios::binary);
  PublicKey election_public_key;
  election_public_key.load(context, pkey);
  pkey.close();

// Getting the private key from the corresponding file
  pkey.open("../Proj/Keys/election_secret_key.txt", ios::binary);
  SecretKey election_secret_key;
  election_secret_key.load(context, pkey);
  pkey.close();

  // Setting the encryption process
  Encryptor encryptor(context, election_public_key);
  Decryptor decryptor(context, election_secret_key);
  string f = "ecrypted_voter_weights";
  string txt = ".txt";
  string a;
  string forcabruta = "sudo cp ";
  string forcamenos;
  string dest = " ../Proj/Keys";
  string xau = "sudo rm -r ";
  string ciao;
  BatchEncoder batch_encoder(context);
  Plaintext plainvector;
  Ciphertext encrypted_weight;

  size_t slot_count = batch_encoder.slot_count();
  size_t row_size = slot_count / 2;
  cout << "Plaintext matrix row size: " << row_size << endl;

  vector<uint64_t> pod_matrix(slot_count, 0ULL);

  for(int i=0; i < nvoters; i = i + 1){
    pod_matrix[i] = (uint64_t) weights[i];
  }

  batch_encoder.encode(pod_matrix, plainvector);

  Ciphertext encrypted_matrix;
  encryptor.encrypt(plainvector, encrypted_matrix);

  Plaintext plain_result;
  decryptor.decrypt(encrypted_matrix, plain_result);
  vector<uint64_t> pod_result;
  batch_encoder.decode(plain_result, pod_result);

  print_matrix(pod_result, row_size);


/*
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
  }*/
}
