#include "dealwithvotes.h"

using namespace std;

void readvotes(int ncandidates, int* votes){
  string a;

  cout << "There are " << ncandidates << "candidates for this election!\n";
  cout << "The number of votes you have is also " << ncandidates;
  cout << "For each vote introduce a number between 1 and " << ncandidates << "\n";
  for(int i=0; i < ncandidates; i = i + 1){
    cout << "Introduce your vote number " << i + 1 << "\n";
    cout << "After this vote, you will have " << ncandidates - i - 1 << " votes left!\n";
    do{
      cin >> a
    }while(stoi(a) < 1 || stoi(a) > ncandidates);
  }
}

void verifypublickey(){
  /*Make a function to verify if the public key is signed with the
  root CA certificate */
}

void encryptvote(int* votes){
  
}
