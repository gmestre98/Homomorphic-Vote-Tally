#include <cstddef>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <thread>
#include <mutex>
#include <memory>
#include <limits>
#include <algorithm>
#include <numeric>
#include "seal/seal.h"
extern "C" {
  #include "sss.h"
  #include "randombytes.h"
  #include <assert.h>
  #include <string.h>
}



void generate_election_keys();
void breaksecretkey();
void weight_encryption(int, int*);
