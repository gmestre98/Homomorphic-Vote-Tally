#include "tallyfileops.h"

using namespace std;

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

char** get_ballot_files(int* size, char* str, char **f)
{
  int i = 0;
  struct dirent *de;  // Pointer for directory entry
  de = (struct dirent *)malloc(sizeof(struct dirent));
  if(de == NULL)
    cout << "mal alocado\n";

  // opendir() returns a pointer of DIR type.
  DIR *dr = opendir("../Proj/Tally/BallotBox");

  if (dr == NULL)  // opendir returns NULL if couldn't open directory
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

int pstrcmp( const void* a, const void* b )
{
  return strcmp( *(const char**)a, *(const char**)b );
}

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


int* voterfiltervalids(char** validfiles, int validsize, int nvoters){
  string comp;
  int *ret=(int *)malloc(validsize*sizeof(int));
  if(ret == NULL)
    cout << "ERROR ALLOCATING voterfiltervector!!\n";


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


int* verifysign(int*voterfilter, int validsize, char** validfiles, int* valsignsize){
  int alfa=0;
  string getpubkey;
  string c1, c2, c3;
  int *ret=(int *)malloc(validsize*sizeof(int));
  if(ret == NULL)
    cout << "ERROR ALLOCATING valsignvotesfile!!\n";

  for(int i=0; i < validsize; i = i + 1){
    string s(validfiles[i]);
    cout << alfa << "\n";
    cout << voterfilter[i] << "filtro\n";
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
    if(system_listen(c1) != "Verified OK\n"){
      cout << "\nYou have got a voter certificate not signed by the CA!\n";
      cout << "Program exiting\n";
      exit(-1);
    }
    else{
      ret[*valsignsize] = i;
      *valsignsize = *valsignsize + 1;
    }
  }
  return ret;
}
/*void verifypublickey(int voterid){
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
*/
