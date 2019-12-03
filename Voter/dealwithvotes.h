#include <iostream>
#include <string>
#include <fstream>
#include "seal/seal.h"

using namespace std;

void readvotes(int, int*);
string system_listen(string);
void verifypublickey(int);
void verifyvotercert(int);
void verifyvoterkey(int);
void encryptvote(int*, int, int);
void signvote(int);
