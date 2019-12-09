#include <cstddef>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "seal/seal.h"
extern "C" {
  #include "sss.h"
  #include "randombytes.h"
  #include <assert.h>
  #include <string.h>
}

using namespace std;

string system_listen(string);
void verifytrustees();
void rebuildkey();
