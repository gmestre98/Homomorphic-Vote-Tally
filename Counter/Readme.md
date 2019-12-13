# **Counter Program** <h1>

**Requirements:**
This program requires you to install the CMake version at least 3.12 which can
be done following this website https://cmake.org/download/, and also the SEAL
Microsoft Library for Homomorphic encryption, at least version 3.4, which can be
done following the guide on this website https://github.com/Microsoft/SEAL.

**Description:**
This program checks the checksum for each vote counted and anounces the election
result.
  * Verifies if all the trustees are signed by the root CA.
  * Restores the simmetric key used to encrypt the election secret key using
  shamir's secret sharing
  * Gets the checksum that came form the Tally Official and checks if every voter
  voted the correct number of times.
  * Decrypts the file with the election results and checks its validity.
  * Announces who won the election.

**Usage:**
To run this program first it is need to generate a make file using a CMake
command, then compile the program using this make file and run the program.
* cmake . -DCMAKE_PREFIX_PATH=~/mylibs
* make
* ./counter [number of voters] [number of candidates]
