#ifndef CRYPTER_H
#define CRYPTER_H

#include<vector>

using namespace std;

//The crypter class

class Crypter{

	public:
		//Crypter();
		//~Crypter();
		vector<int> encrypt(vector<int> message, vector<int> password);
		vector<int> decrypt(vector<int> scramMessage, vector<int> password);

	private:
		vector<int> generateKeyStream(vector<int> password, int messageSize);
		vector<int> deckFromPswd(vector<int> password);
		vector<int> scramble(vector<int> passedDeck);
		int updateJSPOT(vector<int> deckToSearch, bool jokerA);
		vector<int> passCodeDeck(vector<int> password);
		vector<int> applyKeyStream(vector<int> message, vector<int> keySteam, bool encrypting);

};


#endif