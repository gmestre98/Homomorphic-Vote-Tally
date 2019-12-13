# **Admin Program** <h1>

**Requirements:**
This program requires you to install the CMake version at least 3.12 which can
be done following this website https://cmake.org/download/, and also the SEAL
Microsoft Library for Homomorphic encryption, at least version 3.4, which can be
done following the guide on this website https://github.com/Microsoft/SEAL.

**Description:**
This program implements the set up of an electronic voting system. For that
effect it implements the following things:
  * Generate a root CA certificate and private key.
  * Install the root CA certificate in the tally official app.
  * Generate a homomorphic key pair consisting of the election public key and
  the election secret key and also generates relin keys and galois keys used for
  some operations of the tally official.
  * For each voter:
    * Creates the corresponding folder.
    * Generates a voter private key.
    * Generate a certificate signed by the vote-ca certificate.
    * Installs the root CA certificate on the voter folder.
    * Installs the voter certificate and private key on the corresponding folder.
    * Installs the election public key and signature on the corresponding folder.
  * Encrypts the election secret key using a symmetric key and then splits the
  symmetric key using shamir's secret sharing.
  * Assigns a weight for each voter.
  * Encrypts the weights with the election public key.
  * Installs tally keys.
  * Installs the root CA certificate on the counter.

**Usage:**
To run this program first it is need to generate a make file using a CMake
command, then compile the program using this make file and run the program.
* cmake . -DCMAKE_PREFIX_PATH=~/mylibs
* make
* ./admin [number of voters]
