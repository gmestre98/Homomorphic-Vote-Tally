#include "counterops.h"

using namespace std;

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

void verifytrustees(){
  string c1 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Trustees/election_secret_key.sha256 ../Proj/Trustees/election_secret_key.enc";
  system("sudo openssl x509 -pubkey -noout -in ../Proj/Counter/my-ca.crt > pubkey.pem");
  if(system_listen(c1) != "Verified OK\n"){
    cout << "\n You have got an election encrypted key not signed by the CA!\n";
    cout << "Program exiting\n";
    exit(-1);
  }
  for(int i=0; i < 2; i = i + 1){
    c1 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Trustees/sharefile" + to_string(i+1) + ".sha256 ../Proj/Trustees/sharefile" + to_string(i+1) + ".txt";
    if(system_listen(c1) != "Verified OK\n"){
      cout << "\n You have got an share file not signed by the CA!\n";
      cout << "Program exiting\n";
      exit(-1);
    }
  }
  system("sudo rm -r pubkey.pem");
}

void rebuildkey(){
  uint8_t data[sss_MLEN], restored[sss_MLEN];
	sss_Share shares[2];
  string aux;
	int tmp;

  for(int i=0; i < 2; i = i + 1){
    string buffer = "";
    ifstream sharefile;
    sharefile.open(("../Proj/Trustees/sharefile" + to_string(i+1) + ".txt").c_str(), ofstream::binary);
    sharefile >> buffer;
    while(!sharefile.eof()){
      sharefile >> aux;
      buffer.append("\n");
      buffer.append(aux);
    }
    cout << buffer << "\n";
    sharefile.close();
    memcpy(&(shares[i]), &buffer, sizeof(buffer));
  }

  // Combine some of the shares to restore the original secret
  tmp = sss_combine_shares(restored, shares, 2);
  cout << tmp << "\n";
  assert(tmp == 0);
  assert(memcmp(restored, data, sss_MLEN) == 0);

  ofstream file;
  file.open("restored.txt", ofstream::binary);
  file << data;
  file.close();
}
