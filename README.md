# Homomorphic-Vote-Tally <h1>

This project implements an electronic voting system, using homomorphic encryption so that the one who generates the
election results cannot access the contents of the votes because it doesn't have access to the private key and the one
who counts the results can't check who voted on who granting privacy, because it only has access to the results of the
computations made on the votes.
This project consists of 4 programs explained in detail down here.

**Admin**
The admin is the one who sets up all the system, creating the file system and giving everyone the needed certificates and keys.
It also grants everyone the access to the needed files, colocating them in the proper folder.

**Voter**
The voter is the program that allows a user to indicate his or hers vote intentions on this election.

**Tally Official**
The Tally Official program is the one that counts the votes to check if the user voted the correct number of times and computes 
the election results.

**Counter**
The counter rebuilds the election private key and then verifies the checksum for each vote and anounces the election results

**NOTE:**
Each of these programs has it's own Readme.md file on the corresponding folder and has the actions of each program explained in detail.
