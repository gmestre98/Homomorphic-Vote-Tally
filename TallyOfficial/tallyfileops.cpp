#include "tallyfileops.h"

using namespace std;

char** get_ballot_files(int* size, char* str, char **f)
{
  int i = 0;
  struct dirent *de;  // Pointer for directory entry

  // opendir() returns a pointer of DIR type.
  DIR *dr = opendir("../Proj/BallotBox");

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
      f[i] = de->d_name;
      printf("%s\n", f[i]);
      i = i + 1;
    }
  }
  *size = i;
  closedir(dr);
}

void verifysigns(char** filestxt, char** fsha256, int sizetxt, int sizesha256){
  char** tokentxt = (char**)malloc(sizetxt*sizeof(char*));
  char** tokensha256 = (char**)malloc(sizesha256*sizeof(char*));

  for (int i=0; i<sizetxt; i++){
    tokentxt[i] = (char*)malloc(MAX_VOTES*sizeof(char));
  }

  for(int i=0; i<sizetxt; i=i+1){
    memcpy(tokentxt[i], filestxt[i], sizeof(char)*strlen(filestxt[i]));
    tokentxt[i] = strtok(tokentxt[i], ".");
    printf("%s\n", filestxt[i]);
    printf("%s\n", tokentxt[i]);
  }

  for(int i=0; i < sizesha256; i++)
    tokensha256[i] = (char*)malloc(MAX_VOTES*sizeof(char));

  for(int j=0; j<sizesha256; j=j+1){
    memcpy(tokensha256[j], fsha256[j], sizeof(char)*strlen(fsha256[j]));
    tokensha256[j] = strtok(tokensha256[j], ".");
    printf("%s\n", fsha256[j]);
    printf("%s\n", tokensha256[j]);
  }
}
