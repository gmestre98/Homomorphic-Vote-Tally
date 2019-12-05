#include "dealwithvotes.h"

using namespace std;
using namespace seal;

void readvotes(int ncandidates, int* votes){
  string a;

  cout << "There are " << ncandidates << " candidates for this election!\n";
  cout << "The number of votes you have is also " << ncandidates << "\n";
  cout << "For each vote introduce a number between 1 and " << ncandidates << "\n";
  for(int i=0; i < ncandidates; i = i + 1){
    cout << "Introduce your vote number " << i + 1 << "\n";
    cout << "After this vote, you will have " << ncandidates - i - 1 << " votes left!\n";
    do{
      cin >> a;
    }while(stoi(a) < 1 || stoi(a) > ncandidates);
    votes[stoi(a) - 1] = votes[stoi(a) - 1] + 1;
  }
}

string system_listen(string cmd){
    string data;
    FILE *stream;
    const int max_buffer = 512;
    char buffer [max_buffer];

    stream = popen(cmd.c_str(), "r");

    if (stream)
    {
      while(!feof(stream))
      {
        if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
      }

      pclose(stream);
    }

    return data;
}

void verifypublickey(int voterid){
  string c1 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Voters/";
  string c2 = "/homopublic.sha256 ../Proj/Voters/";
  string c3 = "/election_public_key.txt";
  c1.append(to_string(voterid));
  c1.append(c2);
  c1.append(to_string(voterid));
  c1.append(c3);
  system("sudo openssl x509 -pubkey -noout -in ../Proj/CA/my-ca.crt > pubkey.pem");
  if(system_listen(c1) != "Verified OK\n"){
    cout << "\n You have got an election public key not signed by the CA!\n";
    cout << "Program exiting\n";
    exit(-1);
  }
  system("sudo rm -r pubkey.pem");
}


void verifyvotercert(int voterid){
  string c1 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Voters/";
  string c2 = "/signvcert.sha256 ../Proj/Voters/";
  string c3 = "/voter-cert.csr";
  c1.append(to_string(voterid));
  c1.append(c2);
  c1.append(to_string(voterid));
  c1.append(c3);
  system("sudo openssl x509 -pubkey -noout -in ../Proj/CA/my-ca.crt > pubkey.pem");
  if(system_listen(c1) != "Verified OK\n"){
    cout << "\n You have got a voter certificate not signed by the CA!\n";
    cout << "Program exiting\n";
    exit(-1);
  }
  system("sudo rm -r pubkey.pem");
}

void verifyvoterkey(int voterid){
  string c1 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Voters/";
  string c2 = "/signvkey.sha256 ../Proj/Voters/";
  string c3 = "/voter-cert.key";
  c1.append(to_string(voterid));
  c1.append(c2);
  c1.append(to_string(voterid));
  c1.append(c3);
  system("sudo openssl x509 -pubkey -noout -in ../Proj/CA/my-ca.crt > pubkey.pem");
  if(system_listen(c1) != "Verified OK\n"){
    cout << "\n You have got a voter private key not signed by the CA!\n";
    cout << "Program exiting\n";
    exit(-1);
  }
  system("sudo rm -r pubkey.pem");
}

string timestr(time_t t){
  stringstream strm;
  strm << t;
  return strm.str();
}

time_t encryptvote(int* votes, int ncandidates, int voterid){
  string votes_file= to_string(voterid);
  time_t timefile = time(0);
  string txt = ".txt";

  // Setting the parameters for the encryption
  EncryptionParameters parms(scheme_type::BFV);
  size_t poly_modulus_degree = 1024;
  parms.set_poly_modulus_degree(poly_modulus_degree);
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
  parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
  auto context = SEALContext::Create(parms);
  string cert1 = "sudo mkdir ../Proj/Tally/Voters/";
  string certcop1 = "sudo cp ../Proj/Voters/";
  string certcop2 = "/voter-cert.csr";
  string certcop3 = " ../Proj/Tally/Voters/";
  string alt;
  string certsign = "/signvcert.sha256";


  cert1.append(to_string(voterid));
  system(cert1.c_str());
  certcop1.append(to_string(voterid));
  alt = certcop1;
  certcop1.append(certcop2);
  certcop1.append(certcop3);
  certcop1.append(to_string(voterid));
  alt.append(certsign);
  alt.append(certcop3);
  alt.append(to_string(voterid));
  system(certcop1.c_str());
  system(alt.c_str());
  // Getting the election public key from the corresponding file
  ifstream pkey;
  pkey.open("../Proj/Keys/election_public_key.txt", ios::binary);
  PublicKey election_public_key;
  election_public_key.load(context, pkey);
  pkey.close();

  // Setting the encryption process
  Encryptor encryptor(context, election_public_key);
  BatchEncoder batch_encoder(context);
  Plaintext plainvector;
  Ciphertext encrypted_votes;
  size_t slot_count = batch_encoder.slot_count();
  vector<uint64_t> votes_matrix(slot_count, 0ULL);
  for(int i=0; i < ncandidates; i = i + 1){
    votes_matrix[i] = (uint64_t) votes[i];
  }
  batch_encoder.encode(votes_matrix, plainvector);
  encryptor.encrypt(plainvector, encrypted_votes);


  votes_file.append("_votes_");
  votes_file.append(timestr(timefile));
  votes_file.append(txt);
  ofstream votes_f;
  votes_f.open(votes_file, ofstream::binary);
  votes_f << voterid << "_" << timestr(timefile) << "\n";
  encrypted_votes.save(votes_f);
  votes_f.close();

  cout << votes_file << "\n";
  // Moving the file to the right place
  string mv1 = "sudo mv ";
  string mv2 = "_votes_";
  string mv3 = " ../Proj/BallotBox";
  mv1.append(to_string(voterid));
  mv1.append(mv2);
  mv1.append(timestr(timefile));
  mv1.append(txt);
  mv1.append(mv3);
  system(mv1.c_str());
  return(timefile);
}

void signvote(int voterid, time_t time){
  string sign1 = "sudo openssl dgst -sha256 -sign ../Proj/Voters/";
  string sign2 = "/voter-cert.key -out ";
  string sign3 = "_votes_";
  string sign4 = ".sha256 ../Proj/BallotBox/";
  string sign5 = "_votes_";
  string sign6 = ".txt";
  string mv1 = "sudo mv ";
  string mv2 = "_votes_";
  string mv3 = ".sha256 ../Proj/BallotBox/";


  sign1.append(to_string(voterid));
  sign1.append(sign2);
  sign1.append(to_string(voterid));
  sign1.append(sign3);
  sign1.append(timestr(time));
  sign1.append(sign4);
  sign1.append(to_string(voterid));
  sign1.append(sign5);
  sign1.append(timestr(time));
  sign1.append(sign6);
  system(sign1.c_str());
  mv1.append(to_string(voterid));
  mv1.append(mv2);
  mv1.append(timestr(time));
  mv1.append(mv3);
  system(mv1.c_str());
}
