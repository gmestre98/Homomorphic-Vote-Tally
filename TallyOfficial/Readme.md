# **Tally Official Program** <h1>

**Requirements:**
This program requires you to install the CMake version at least 3.12 which can
be done following this website https://cmake.org/download/, and also the SEAL
Microsoft Library for Homomorphic encryption, at least version 3.4, which can be
done following the guide on this website https://github.com/Microsoft/SEAL.

**Description:**
This program implements the work of a tally official, choosing which votes to
count and send it to the counter.
  * Gets the files with the encrypted and the signed votes from the Ballot Box.
  * Verifies if the files have valid names and signed file names.
  * Verifies which files from the Ballot Box were signed by the root CA and are
  valid.
  * Verifies which votes are the most recent, and eliminates the rest.
  * Verifies the validity of the voters certificates.
  * Verifies the signature of the homomorphic relin keys and galois keys
  necessary for the homomorphic computations to be made on the encrypted data.
  * Computes the checksum of the votes for each voter and saves it on the
  counter folder.
  * Verifies if the files with the voters weigths are valid.
  * Computes the election results and sends it to the counter.

**Usage:**
To run this program first it is need to generate a make file using a CMake
command, then compile the program using this make file and run the program.
* cmake . -DCMAKE_PREFIX_PATH=~/mylibs
* make
* ./tally [number of voters] [number of candidates]
