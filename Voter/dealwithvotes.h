#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "seal/seal.h"

using namespace std;

void readvotes(int, int*);
string system_listen(string);
void verifypublickey(int);
void verifyvotercert(int);
void verifyvoterkey(int);
string timestr(time_t);
time_t encryptvote(int*, int, int);
void signvote(int, time_t);
