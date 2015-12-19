#include "Crypter.h"
#include<iostream>

int Crypter::updateJSPOT(vector<int> deckToSearch, bool jokerA){
	if(jokerA){
		for(int i = 0; i < 54; i++){
			if(deckToSearch.at(i) == 52){ return i; }
		}
	}else{
		for(int i = 0; i < 54; i++){
			if(deckToSearch.at(i) == 53){ return i; }
		}
	}
	return 0;//If something done messed up
	std::cout << "Something messed up in Crypter::updateJSPOT" << endl;
}

vector<int> Crypter::scramble(vector<int> passedDeck){

	int JASPOT = updateJSPOT(passedDeck, true);
    int JBSPOT = updateJSPOT(passedDeck, false);
    vector<int>::iterator iter;
    
    //Begin Step 1
    swap(passedDeck[JASPOT], passedDeck[(JASPOT + 1) % 54]);
    
    JASPOT = updateJSPOT(passedDeck, true);
	JBSPOT = updateJSPOT(passedDeck, false);
    
    //Begin Step 2
    swap(passedDeck[JBSPOT], passedDeck[(JBSPOT + 1) % 54]);
    JASPOT = updateJSPOT(passedDeck, true);
    JBSPOT = updateJSPOT(passedDeck, false);
    
    swap(passedDeck[JBSPOT], passedDeck[(JBSPOT + 1) % 54]);//Again
    JASPOT = updateJSPOT(passedDeck, true);
    JBSPOT = updateJSPOT(passedDeck, false);
    
    //Begin Step 3
    vector<int> leftPiece;
    vector<int> rightPiece;
    vector<int> middlePiece;
    if(JASPOT > JBSPOT){ int temp = JASPOT; JASPOT = JBSPOT; JBSPOT = temp; }//JASPOT is on the left
    
    for(int i = 0; i < JASPOT; i++){//Gathering everything left of joker A
        leftPiece.push_back(passedDeck[i]);
    }
    
    for(int i = JASPOT; i <= JBSPOT; i++){//Gathering everything between jokers, inclusive
        middlePiece.push_back(passedDeck[i]);
    }
    
    for(int i = JBSPOT + 1; i < 54; i++){//Gathering everything right of joker B
        rightPiece.push_back(passedDeck[i]);
    }
    
    passedDeck.clear();//Prep for swap
    passedDeck.insert(passedDeck.end(), rightPiece.begin(), rightPiece.end());
    passedDeck.insert(passedDeck.end(), middlePiece.begin(), middlePiece.end());
	//Actual swapping of chunks of deck
    passedDeck.insert(passedDeck.end(), leftPiece.begin(), leftPiece.end());
    
    //Begin Step 4
    int howFar = passedDeck.at(53);
    leftPiece.clear();
    rightPiece.clear();
    for(int i = 0; i < howFar; i++){//Gathering left-most numbers
        leftPiece.push_back(passedDeck[i]);
    }
    
    for(int i = howFar; i < 53; i++){//Gathering numbers on the right
        rightPiece.push_back(passedDeck[i]);
    }
    
    passedDeck.clear();//Prep for swap
    passedDeck.insert(passedDeck.end(), rightPiece.begin(), rightPiece.end());
	//Actual swapping of chunks
    passedDeck.insert(passedDeck.end(), leftPiece.begin(), leftPiece.end());
    passedDeck.push_back(howFar);//Adding last value to the end
    
    return passedDeck;
}

vector<int> Crypter::passCodeDeck(vector<int> password){
	vector<int> toReturn;
	for(int i = 0; i < 52; i++){//Fill with numbers 1-52
		toReturn.push_back(i);
	}


	/* This would place the jokers at a spot determined by the 
	last two letters of the password, but gives errors
	vector<int>::reverse_iterator rIter = password.rbegin();
	int whereToInsert = *rIter;
	toReturn.insert(password.begin() + whereToInsert, 52);//Joker A
	rIter++;
	whereToInsert = *rIter;
	toReturn.insert(password.begin() + whereToInsert, 53);//Joker B
	*/

	toReturn.push_back(52);
	toReturn.push_back(53);//Just tacks the jokers onto the end of the deck

	return toReturn;



}

vector<int> Crypter::generateKeyStream(vector<int> password, int messageSize)
{
	//Generate keystream for encrypting the method, uses the scramble function defined above
	vector<int> deckToScramble = passCodeDeck(password);

	//password.pop_back(); /*These lines are commented for reasons given in passCodeDeck */
	//password.pop_back();


	for(vector<int>::iterator iter = password.begin(); iter != password.end(); iter++){//For every value in deckToScramble
		//Perform steps 1 through 4
		deckToScramble = scramble(deckToScramble);
		//Perform step 5, which is a rehash of step 4
		int howFar = *iter;
		vector<int> leftPiece;
		vector<int> rightPiece;
		for(int i = 0; i < howFar; i++){//Gathering left-most numbers
			leftPiece.push_back(deckToScramble[i]);
		}
    
		for(int i = howFar; i < 53; i++){//Gathering numbers on the right
			rightPiece.push_back(deckToScramble[i]);
		}
		deckToScramble.clear();//Prep for swap
		deckToScramble.insert(deckToScramble.end(), rightPiece.begin(), rightPiece.end());//Actual swapping of chunks
		deckToScramble.insert(deckToScramble.end(), leftPiece.begin(), leftPiece.end());
		deckToScramble.push_back(howFar);
		//End of step 5

	}


	//deckToScramble is now ready to be used to generate a keystream
	vector<int> keyStream;
	for(int i = 0; i < messageSize; i++){
		int addToKeyStream = 0;
		//do{
			//Perform steps 1 through 4
			deckToScramble = scramble(deckToScramble);
			//Perform step 5
			int goDown = deckToScramble.at(0);

			addToKeyStream = deckToScramble.at(goDown); //Used to be goDown -1 but that gave errors
		//}while(addToKeyStream != 52 && addToKeyStream != 53)
		keyStream.push_back((addToKeyStream % 26) + 1);//The keystream numbers must be 1-26
	}//End for loop, keyStream is now ready to use


	//cout << "The keyStream is ";
	//for(int i = 0; i < messageSize; i++){ cout << keyStream.at(i) << ','; }
	//cout << endl;

	return keyStream;

}

vector<int> Crypter::encrypt(vector<int> message, vector<int> password){

	vector<int> keyStream = generateKeyStream(password, message.size());

	message = applyKeyStream(message, keyStream, true);//True because it is encrypting

	//Return the new encrypted message
	return message;

}

vector<int> Crypter::decrypt(vector<int> scramMessage, vector<int> password)
{
	
	vector<int> keyStream = generateKeyStream(password, scramMessage.size());
	
	scramMessage = applyKeyStream(scramMessage, keyStream, false);//False because its decrypting

	return scramMessage;
}

vector<int> Crypter::applyKeyStream(vector<int> message, vector<int> keyStream, bool encrypting){
	if(message.size() != keyStream.size()){ 
		std::cout << "The message and keystream are unequal in size" << std::endl;
		std::cout << "This means there is a problem in Crypter.cpp" << std::endl;
	}

	vector<int> newMessage;
	for(unsigned int i = 0; i < message.size(); i++){
		if(encrypting){//Note that one is plus and one is minus
			int temp = message.at(i) + keyStream.at(i);
			while(temp > 26){ temp -= 26; }//Modulo, but must be 1-26, not 0-25
			newMessage.push_back(temp);
		}else{//False was given for 'encrypting'
			int temp = message.at(i) - keyStream.at(i);
			while(temp < 0){ temp += 26; }//Modulo, unsure how it interacts with negative numbers
			newMessage.push_back(temp % 26);
		}
	}

	return newMessage;//The message has been scrambled through use of the keystream
}