/******************************************************************************
*
* File Name: tallyfileops.cpp
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  07 Dec 2019
*
* NAME
*  adminsealops - Declaration of the functions needed for general operations on
* the tally official
*
* DESCRIPTION
*  Most of the functions declaread on this file are for general verifications
* that the data needs to be subjected too, but also for signing verifications,
* filename verifications and getting files from a folder
*
*****************************************************************************/
#include "tallyfileops.h"

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
 * mallarraystrings()
 *
 * Arguments: - Number of lines
 *            - Number of columns
 *            - String with part of the error message
 * Returns: Allocated array of strings
 *
 * Description: This function allocates an array of strings doing the proper
 *  verifications
 *
 *****************************************************************************/
char** mallarraystrings(int n1, int n2, string error){
  char** array1 = (char**)malloc(n1*sizeof(char *));
  char *alfa;
  if(array1 == NULL){
    cout << "\nERROR ALOCATING" << error << "\n";
    exit(-1);
  }
  for(int i=0; i < n1; i=i+1){
    array1[i] = (char*)malloc(n2*sizeof(char));
    if(array1[i] == NULL){
      cout << "\nERROR ALOCATING" << error << "\n";
      exit(-1);
    }
    memset(array1[i], 0, n2*sizeof(array1[i][0]));
  }
  return array1;
}

/******************************************************************************
 * get_ballot_files()
 *
 * Arguments: - Pointer for the number of found files
 *            - String with part of the wanted files name
 *            - Array of strings to be returned
 * Returns: none
 *
 * Description: This function gets all the files on the Ballot Box Tally
 *  directory that have in the name a given string str
 *
 *****************************************************************************/
void get_ballot_files(int* size, char* str, char **f)
{
  int i = 0;
  struct dirent *de;  // Pointer for directory entry
  de = (struct dirent *)malloc(sizeof(struct dirent));
  if(de == NULL)
    cout << "Dirent struct didn't get properly allocated!\n";

  // opendir() returns a pointer of DIR type.
  DIR *dr = opendir("../Proj/Tally/BallotBox");

  if (dr == NULL)  // opendir returns NULL if the directory couldn't be opened
  {
    fprintf(stderr, "Could not open current directory");
    exit(1);
  }

  i = 0;
  while (((de = readdir(dr)) != NULL ) && (i < MAX_VOTES))
  {
    if(strstr(de->d_name, "votes") != NULL && strstr(de->d_name, str) != NULL)
    {
      memcpy(f[i], de->d_name, strlen(de->d_name));
      printf("%s\n", f[i]);
      i = i + 1;
    }
  }
  *size = i;
  closedir(dr);
}

/******************************************************************************
 * pstrcmp()
 *
 * Arguments: - String a
 *            - String b
 * Returns: An integer with a value that signals which string "is first"
 *
 * Description: This function checks the string compare of both inputed strings
 *
 *****************************************************************************/
int pstrcmp( const void* a, const void* b )
{
  return strcmp( *(const char**)a, *(const char**)b );
}


/******************************************************************************
 * validtxts()
 *
 * Arguments: - Array of strings with the txt files
 *            - Array of strings with the sha256 files
 *            - Size of the txt array of strings
 *            - Size of the sha256 array of strings
 *            - Pointer to the size of the array to be returned
 * Returns: Array of strings with the names of the found txt files that have a
 *  match on the sha256 files
 *
 * Description: This function verifies which txt files and sha256 files given
 *  have a match and returns an array of strings with the names of the files
 *  that have a match, excluding the others
 *
 *****************************************************************************/
char** validtxts(char** filestxt, char** fsha256, int sizetxt, int sizesha256, int *validsize){
  char** tokentxt = mallarraystrings(sizetxt, MAX_VOTES, "tokentxt array!!");
  char** tokensha256 = mallarraystrings(sizesha256, MAX_VOTES, "tokentxt array!!");
  char** valids = mallarraystrings(MAX_VOTES, MAX_VOTES, "valid matches array!!");

  for(int i=0; i<sizetxt; i=i+1){
    memcpy(tokentxt[i], filestxt[i], sizeof(char)*strlen(filestxt[i]));
    tokentxt[i] = strtok(tokentxt[i], ".");
  }

  for(int j=0; j<sizesha256; j=j+1){
    memcpy(tokensha256[j], fsha256[j], sizeof(char)*strlen(fsha256[j]));
    tokensha256[j] = strtok(tokensha256[j], ".");
  }
  qsort(tokentxt, sizetxt, sizeof(tokentxt[0]), pstrcmp);
  qsort(tokensha256, sizesha256, sizeof(tokensha256[0]), pstrcmp);

  for(int i=0; i < sizetxt; i=i+1){
    for(int j=0; j < sizesha256; j=j+1){
      if(strcmp(tokentxt[i], tokensha256[j]) == 0){
        memcpy(valids[*validsize], tokentxt[i], strlen(tokentxt[i])*sizeof(char));
        *validsize = *validsize + 1;
        break;
      }
    }
  }
  free(tokentxt);
  free(tokensha256);
  return valids;
}

/******************************************************************************
 * voterfiltervalid()
 *
 * Arguments: - Array of strings with the files that have a match between txt
 *          and sha256
 *            - Size of the first array
 *            - Number of voters
 * Returns: Array of integers with the voter id in the corresponding position
 *          for each file
 *
 * Description: This function searches which voter corresponds to each file name
 *   and returns an array of integers in which each position has the voter id
 *   for the corresponding file
 *
 *****************************************************************************/
int* voterfiltervalids(char** validfiles, int validsize, int nvoters){
  string comp;
  int *ret=(int *)malloc(validsize*sizeof(int));
  if(ret == NULL)
    cout << "ERROR ALLOCATING voterfiltervector!!\n";
  memset(ret, 0, validsize*sizeof(int));


  for(int i=0; i < nvoters; i=i+1){
    comp = "";
    comp.append(to_string(i + 1));
    comp.append("_");
    for(int j=0; j < validsize; j=j+1){
      if(strstr(validfiles[j], comp.c_str()) != NULL)
        ret[j] = i + 1;
    }
  }
  return ret;
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
 * verifyvotercert()
 *
 * Arguments: - Voter id
 * Returns: none
 *
 * Description: This function verifies if the given voter certificate for this
 *  voter is signed by the root CA, if not the program exits.
 *
 *****************************************************************************/
void verifyvotercert(int voterid){
  string c1 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Tally/Voters/";
  string c2 = "/signvcert.sha256 ../Proj/Tally/Voters/";
  string c3 = "/voter-cert.csr";
  c1.append(to_string(voterid));
  c1.append(c2);
  c1.append(to_string(voterid));
  c1.append(c3);
  system("sudo openssl x509 -pubkey -noout -in ../Proj/Tally/my-ca.crt > pubkey.pem");
  if(system_listen(c1) != "Verified OK\n"){
    cout << "\nYou have got a voter certificate not signed by the CA!\n";
    cout << "Program exiting\n";
    exit(-1);
  }
  system("sudo rm -r pubkey.pem");
}

/******************************************************************************
 * verifysign()
 *
 * Arguments: - Array of integers with the voters id for each file
 *            - Size of the array of strings with the valid matched files
 *            - Array of strings with the valid matched files
 *            - Pointer to the size of the array of strings of valid signed files
 * Returns: Array of strings with the names of the valid txt file names that are
 *  also signed
 *
 * Description: This function verifies which txt files of the given array have
 * a valid signature file also
 *
 *****************************************************************************/
char** verifysign(int*voterfilter, int validsize, char** validfiles, int* valsignsize){
  int alfa=0;
  string getpubkey;
  string c1, c2, c3;
  char** ret = mallarraystrings(validsize, MAX_VOTES, "signedfiles array");

  for(int i=0; i < validsize; i = i + 1){
    string s(validfiles[i]);
    if(alfa != voterfilter[i]){
      alfa = voterfilter[i];
      verifyvotercert(voterfilter[i]);
      system("sudo rm -r temp.pem");
      getpubkey = "sudo openssl req -in ../Proj/Tally/Voters/";
      getpubkey.append(to_string(voterfilter[i]));
      getpubkey.append("/voter-cert.csr -pubkey -out temp.pem");
      system(getpubkey.c_str());
    }
    c1 = "sudo openssl dgst -sha256 -verify temp.pem -signature ../Proj/Tally/BallotBox/";
    c1.append(s);
    c1.append(".sha256");
    c1.append(" ../Proj/Tally/BallotBox/");
    c1.append(s);
    c1.append(".txt");
    if(system_listen(c1) != "Verified OK\n")
      cout << "\nYou have got a voter certificate not signed by the CA!\n";
    else{
      memcpy(ret[*valsignsize], validfiles[i], strlen(validfiles[i])*sizeof(char));
      *valsignsize = *valsignsize + 1;
    }
  }
  return ret;
}

/******************************************************************************
 * verifycontent()
 *
 * Arguments: - Array of strings with the valid signed files
 *            - Size of the array of strings with the valid signed files
 *            - Pointer to the size of the array of strings of files with valid
 *          content
 * Returns: Array of strings with the names of the valid txt file names that are
 *  also signed and have the timestamp content validated
 *
 * Description: This function verifies the timestamp written inside the signed
 * file with the one on the file name
 *
 *****************************************************************************/
char** verifycontent(char** signfiles, int valsignsize, int* valcontentsize){
  ifstream file;
  char** ret = mallarraystrings(valsignsize, MAX_VOTES, "content val array");
  string a;

  cout << "\n\n\n\n\n";
  for(int i=0; i < valsignsize; i=i+1){
    string s(signfiles[i]);
    string token, token2;
    string delimiter = "_";
    size_t pos = 0;
    // Setting the parameters for the key load
    EncryptionParameters parms(scheme_type::BFV);
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
    auto context = SEALContext::Create(parms);
    Ciphertext enc_vote;
    file.open(("../Proj/Tally/BallotBox/" + s + ".txt").c_str(), ios::binary);
    getline(file, a);
    enc_vote.load(context, file);
    file.close();
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + delimiter.length());
    }
    pos=0;
    while ((pos = a.find(delimiter)) != std::string::npos) {
        token2 = a.substr(0, pos);
        a.erase(0, pos + delimiter.length());
    }
    if(a == s){
      memcpy(ret[*valcontentsize], signfiles[i], strlen(signfiles[i])*sizeof(char));
      cout << ret[*valcontentsize] << "\n";
      *valcontentsize = *valcontentsize + 1;
    }
    ofstream errfile;
    string alfa(signfiles[i]);
    errfile.open((alfa + ".err").c_str(), ofstream::binary);
    enc_vote.save(errfile);
    errfile.close();
  }
  return ret;
}


/******************************************************************************
 * verifytime()
 *
 * Arguments: - Array of strings with the files for which everything was validated
 *            - Size of the array of strings used
 *            - Number of voters
 * Returns: Array of strings that must have the valid votes for each different
 *        voter
 *
 * Description: This function returns an array of strings that has the vote that
 *  is going to be used for each voter and that has all the information except
 *  the vote itself validated.
 *
 *****************************************************************************/

char** verifytime(char** valcontent, int valcontentsize, int nvoters){
    char** ret = mallarraystrings(nvoters, MAX_VOTES, "votes final array");
    string comp;

    for(int i=0; i < nvoters; i = i + 1){
      ret[i][0] = 0;
      comp = "";
      comp.append(to_string(i + 1));
      comp.append("_");
      for(int j=0; j < valcontentsize; j = j + 1){
        if(strstr(valcontent[j], comp.c_str()) != NULL)
          memcpy(ret[i], valcontent[j], strlen(valcontent[j])*sizeof(char));
      }
    }

    return ret;
}

/******************************************************************************
 * verifykeyssigns()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: This function verifies the signature of the homomorphic relin
 *  keys and galois keys necessary for the homomorphic computations to be made
 *  on the encrypted data
 *
 *****************************************************************************/
 void verifykeyssigns(){
   string c1 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Tally/relin.sha256 ../Proj/Tally/relin_keys.txt";
   string c2 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Tally/galois.sha256 ../Proj/Tally/galois_keys.txt";

   system("sudo openssl x509 -pubkey -noout -in ../Proj/Tally/my-ca.crt > pubkey.pem");
   if(system_listen(c1) != "Verified OK\n"){
     cout << "\nYou have got your relin keys not signed by the CA!\n";
     cout << "Program exiting\n";
     exit(-1);
   }
   if(system_listen(c2) != "Verified OK\n"){
     cout << "\nYou have got your galois keys not signed by the CA!\n";
     cout << "Program exiting\n";
     exit(-1);
   }
   system("sudo rm -r pubkey.pem");
 }

/******************************************************************************
* checksumvote()
*
* Arguments: - String with the name of the file with the vote for voter if
             - Array that is going to be filled with 0 for the voters that didnt
             participate in the election and 1 for the others
             - Voter id - 1
             - Number of candidates for this election
* Returns: none
*
* Description: This function computes the checksum of the votes for each voter
*   using rotations and sums on the encrypted data, then it saves the encrypted
*   checksums on the counter folder
*
******************************************************************************/
void checksumvote(char* votename, int* votes, int voter, int ncandidates){
  if(votename[0] == 0){
    votes[voter] = 0;
    return;
  }
  else{
    votes[voter] = 1;
    // Setting the parameters for the key load
    EncryptionParameters parms(scheme_type::BFV);
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
    auto context = SEALContext::Create(parms);
    RelinKeys relin_keys;
    GaloisKeys galois_keys;
    ifstream relfile, galfile, votefile;
    Ciphertext enc_vote;
    string s(votename);
    string a;
    Evaluator evaluator(context);
    Ciphertext *aux = (Ciphertext *)malloc(ncandidates*sizeof(Ciphertext));
    if(aux == NULL){
      cout << "Here there is a serious error on alocating the aux holy graal\n";
      exit(-1);
    }

    // Getting the relin keys and galois keys
    relfile.open("../Proj/Tally/relin_keys.txt", ios::binary);
    relin_keys.load(context, relfile);
    relfile.close();
    galfile.open("../Proj/Tally/galois_keys.txt", ios::binary);
    galois_keys.load(context, galfile);
    galfile.close();
    // Getting the encrypted vote to be processed
    cout << s + ".err" << "\n";
    votefile.open((s + ".err").c_str(), ofstream::binary);
    enc_vote.load(context, votefile);
    votefile.close();

    // Here is where magic will happen thanks to rotations and sums
    for(int i=0; i < ncandidates; i = i + 1)
      memcpy(&(aux[i]), &enc_vote, sizeof(Ciphertext));
    for(int i=1; i < ncandidates; i = i + 1){
      evaluator.rotate_rows_inplace(aux[i], i ,galois_keys);
      evaluator.add_inplace(aux[0], aux[i]);
    }

    // Now let's save the checksum for this voter on a file
    ofstream mostimportantfileever;
    mostimportantfileever.open(("Voter" + to_string(voter+1) + ".txt").c_str(), ofstream::binary);
    (aux[0]).save(mostimportantfileever);
    mostimportantfileever.close();
    free(aux);
    system(("sudo mv Voter" + to_string(voter + 1) + ".txt ../Proj/Counter").c_str());
  }
}

/******************************************************************************
* verifyweightfiles()
*
* Arguments: none
* Returns: none
*
* Description: This function computes the checksum of the votes for each voter
*   using rotations and sums on the encrypted data, then it saves the encrypted
*   checksums on the counter folder
*
******************************************************************************/
void verifyweightfiles(int nvoters){
  string c1;
  ifstream f;


  system("sudo openssl x509 -pubkey -noout -in ../Proj/Tally/my-ca.crt > pubkey.pem");
  for(int i=0; i < nvoters; i = i + 1){
    c1 = "sudo openssl dgst -sha256 -verify pubkey.pem -signature ../Proj/Tally/" + to_string(i + 1) + "_weights.sha256 ../Proj/Tally/" + to_string(i + 1) + "_weights.txt";

    if(system_listen(c1) != "Verified OK\n"){
      cout << "\nYou have got your relin keys not signed by the CA!\n";
      cout << "Program exiting\n";
      exit(-1);
    }
    f.open(("../Proj/Tally/" + to_string(i+1) + "_weights.txt").c_str(), ios::binary);
    getline(f, c1);
    f.close();
    if(c1 != to_string(i+1)){
      cout << "Someone has been tampering with the weight files, abort mission!\n";
      exit(-1);
    }
  }
  system("sudo rm -r pubkey.pem");
}

/******************************************************************************
* electionresults()
*
* Arguments: none
* Returns: none
*
* Description: This function computes the checksum of the votes for each voter
*   using rotations and sums on the encrypted data, then it saves the encrypted
*   checksums on the counter folder
*
******************************************************************************/
void electionresults(char **votesfinal, int* electionvotes, int nvoters){
  int startpoint = 0;
  // Setting the parameters for the key load
  EncryptionParameters parms(scheme_type::BFV);
  size_t poly_modulus_degree = 8192;
  parms.set_poly_modulus_degree(poly_modulus_degree);
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
  parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
  auto context = SEALContext::Create(parms);
  Evaluator evaluator(context);
  ifstream file;
  Ciphertext result, weight, aux;
  ofstream f;
  string a;

  while(startpoint < nvoters  &&  electionvotes[startpoint] == 0)
    startpoint = startpoint + 1;

  if(startpoint == nvoters)
    return;


  string s(votesfinal[startpoint]);
  file.open((s + ".err").c_str(), ofstream::binary);
  result.load(context, file);
  file.close();
  file.open(("../Proj/Tally/" + to_string(startpoint+1) + "_weights.txt").c_str(), ios::binary);
  getline(file, a);
  weight.load(context, file);
  file.close();
  evaluator.multiply_inplace(result, weight);
  for(int i=startpoint+1; i < nvoters; i=i+1){
    if(electionvotes[i] == 0)
      continue;
    else{
      string s2(votesfinal[i]);
      file.open((s2 + ".err").c_str(), ofstream::binary);
      aux.load(context, file);
      file.close();
      file.open(("../Proj/Tally/" + to_string(i+1) + "_weights.txt").c_str(), ios::binary);
      file.open(("../Proj/Tally/" + to_string(i+1) + "_weights.txt").c_str(), ios::binary);
      weight.load(context, file);
      file.close();
      evaluator.multiply_inplace(aux, weight);
      evaluator.add_inplace(result, aux);
    }
  }
  f.open("results.txt", ofstream::binary);
  result.save(f);
  f.close();

  system("sudo mv results.txt ../Proj/Counter");
}
