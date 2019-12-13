# **Voter Program** <h1>

**Requirements:**
This program requires you to install the CMake version at least 3.12 which can
be done following this website https://cmake.org/download/, and also the SEAL
Microsoft Library for Homomorphic encryption, at least version 3.4, which can be
done following the guide on this website https://github.com/Microsoft/SEAL.

**Description:**
This program gives the user the ability to vote on the electronic voting system
that the admin did set up for an upcoming election.
* Reads the number of candidates and the votes from the command line.
* Verifies if the public key, the voter private key and the voter certificate
are signed by the root CA.
* Gives the voter's certificate to Tally Official.
* Encrypts the votes using the election public key and puts it on a file on
the Ballot Box.
* Signs the files with the encrypted votes using the voter's private key and
puts the signature file on the Ballot Box.

**Usage:**
To run this program first it is need to generate a make file using a CMake
command, then compile the program using this make file and run the program.
* cmake . -DCMAKE_PREFIX_PATH=~/mylibs
* make
* ./voter [number of candidates] [voter id]
