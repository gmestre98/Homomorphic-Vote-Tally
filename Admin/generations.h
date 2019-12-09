/******************************************************************************
*
* File Name: generations.h
* Authors:   Gonçalo Mestre & Carolina Zerbes & Rui Pedro Silva
* Revision:  07 Dec 2019
*
* NAME
*  generations - Header file for the generations.cpp file
*
* DESCRIPTION
* This file is just an header file for the generations.cpp, with just the
* necessary declarations and includes
*
*****************************************************************************/
#include <iostream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

bool isNumber(string);
void generaterootca();
void installrootCAtally();
void createvoterfolder(int);
void voter_privatekey(int);
void voter_certificate(int);
void installvoterrootCA(int);
void install_election_public_key(int);
void votertallyfolder(int);
void weight_assign(int, int*);
void setlastfolders();
void installtallykeys();
