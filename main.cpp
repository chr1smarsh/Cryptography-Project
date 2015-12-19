#include<cstdlib>
#include<iostream>
#include<string>
#include<fstream>
#include "Crypter.h"

using namespace std;

void giveIntro();
int takeMenuChoice();
void dispInfo();
void helpMenu();
void encrypt();
void decrypt();
vector<int> toIntVector(string text);
void displayVectorInts(vector<int> toDisplay);
string convertToString(vector<int> text);

int main(int argc, char *argv[])
{
    cout << "Welcome to my cryptography program!!" << endl << 
			"Currently, this allows for encrypting a message by providing a password." << endl <<
            "Whatever you do, don't tell other people the password!" << endl;
    bool keepRunning = true;
    do{
        giveIntro();
        int choice = takeMenuChoice(); //Decides which option to do
        switch(choice){
            case 1:
                encrypt();
                break;
            case 2:
                decrypt();
                break;
            case 3:
                dispInfo();
                break;
            case 4:
                helpMenu();
                break;
            case 5:
                keepRunning = false; //Ends loop
                break;
            case 6:
                cout << "Unable to handle request, please try again." << endl; //Error message
                break;
        }
        
    }while(keepRunning);
    
    
    return 0;
    
    
    
    return EXIT_SUCCESS;
}

void giveIntro()
{
    cout << "Please select an item from the menu:" << endl;
    cout << "1: Encrypt" << endl <<
            "2: Decrypt" << endl <<
            "3: Information on this program" << endl <<
            "4: Help menu" << endl << 
            "5: Quit Program" << endl;
}

int takeMenuChoice()
{
    string input;
	getline(cin, input); //User's choice
    for(unsigned int i = 0; i < 64; i++){  //Makes the input from user all lowercase
        input[i] = tolower(input[i]);
    }
    //Turns input in various forms to returning a simple number
	if(input[0] == '1' || input[0] == 'e'){ return 1;}
    
    if(input[0] == '2' || input[0] == 'd'){ return 2; }
    
    if(input[0] == '3' || input[0] == 'i'){ return 3; }
    
    if(input[0] == '4' || input[0] == 'h'){ return 4; }
    
    if(input[0] == '5' || input[0] == 'q'){ return 5; }
    
    return 6;
}

void dispInfo(){
	cout << "This is a basic, text-based encryption program that works with a password." <<
			"As long as the password is secret, the message is secure." << endl;
	
}

void helpMenu(){
	cout << "Under construction..." << endl;
}

string convertToString(vector<int> text)
{
	string toReturn;
	for(vector<int>::iterator vIter = text.begin(); vIter != text.end(); vIter++){
		char * temp = new char;
		*temp = (char)(*vIter + 64);
		string toAppend(temp);
		toReturn.append(toAppend);
	}

	return toReturn;
}

vector<int> toIntVector(string text){
	vector<int> toReturn;
	for(string::iterator sIter = text.begin(); sIter != text.end(); sIter++){
		int addToVector = (int)*sIter;//Turns the character into an  integer

		if(addToVector >= 65 && addToVector <= 90){ addToVector -= 64;}
		//May have to be 65 and 97, possible error
		
		if(addToVector >= 97 && addToVector <= 122){ addToVector -= 96;}
		//Turns ASCII values into 1-26 for the alphabet

		toReturn.push_back(addToVector);
	}

	return toReturn;
}

void displayVectorInts(vector<int> toDisplay)
{
	for(unsigned int i = 0; i < toDisplay.size(); i++){
		cout << toDisplay.at(i) << ",";
	}
	cout << endl;
}

void displayIntVector(vector<int> toDisplay)
{
	for(unsigned int i = 0; i < toDisplay.size(); i++){
		char tempChar = (char)(toDisplay.at(i) + 64);
		cout << tempChar;
	}
	cout << endl;
}

void encrypt()
{
	cout << "What is the message you wish to encrypt? " << endl << "Enter 'from file' to open a file." << endl;
	string inputMessage;
	getline(cin, inputMessage);
	cout << "What is your password? ";
	string inputPswd;
	getline(cin, inputPswd);

	vector<int> intMessage = toIntVector(inputMessage);

	if(inputMessage == "from file" || inputMessage == "From file" || inputMessage == "From File"){
		cout << "What is the name of the file? ";
		string fileName;
		getline(cin, fileName);
		fileName.append(".txt");

		fstream dataFile;
		dataFile.open(fileName, ios::in);

		intMessage.clear();
		while(!dataFile.eof()){
			int temp = (int)dataFile.get();
			if(temp >= 65 && temp <= 90){
				intMessage.push_back(temp - 64);
			}
			if(temp >= 97 && temp <= 122){
				intMessage.push_back(temp - 96);
			}
			//Nothing will happen if it hits a whitespace or punctuation
		}
	}



	//displayVectorInts(intMessage);

	vector<int> intPswd = toIntVector(inputPswd);

	Crypter * crypterObject = new Crypter;
	vector<int> encryptedMessage;

	encryptedMessage = crypterObject->encrypt(intMessage, intPswd);

	cout << endl << "Your scrambled message is ";
	displayIntVector(encryptedMessage);
	cout << endl;
	//displayVectorInts(encryptedMessage);

	cout << "Do you wish to save it to a file? Y/N ";
	string yesOrNo;
	getline(cin, yesOrNo);

	if(yesOrNo[0] == 'y' || yesOrNo[0] == 'Y'){
		cout << "What should be the name of this file? ";
		string fileName;
		getline(cin, fileName);
		fileName.append(".txt");//Adds the extension for the file

		fstream dataFile;
		dataFile.open(fileName, ios::out);

		if(!dataFile){ cout << "Error opening file!!" << endl; }//Error message

		//dataFile << convertToString(encryptedMessage);//Writing to file
		for(vector<int>::iterator vIter = encryptedMessage.begin(); vIter != encryptedMessage.end(); vIter++){
			char temp = (char)(*vIter + 64);//To match ASCII value
			dataFile << temp;
		}

		cout << "Your file has been successfully created!! Please come again." << endl;
		dataFile.close();//Close the file
	}

	delete crypterObject;
	
}

void decrypt()
{
	cout << "What is the message that you wish to decrypt?" << endl << "Enter 'from file' to open a file." << endl;
	string inputMessage;
	getline(cin, inputMessage);
	cout << "What is your password? ";
	string inputPswd;
	getline(cin, inputPswd);

	vector<int> intMessage = toIntVector(inputMessage);

	//displayVectorInts(intMessage);

	vector<int> intPswd = toIntVector(inputPswd);

	Crypter * crypterObject = new Crypter;
	vector<int> decryptedMessage;

	if(inputMessage == "from file" || inputMessage == "From file" || inputMessage == "From File"){
		cout << "What is the name of this file? ";
		string fileName;
		getline(cin, fileName);
		fileName.append(".txt");//To match the name, will only work with .txt files

		fstream dataFile;
		dataFile.open(fileName, ios::in);

		intMessage.clear();
		while(!dataFile.eof()){
			int temp = (int)dataFile.get();
			if(temp >= 65 && temp <= 90){//Uppercase
				intMessage.push_back(temp - 64);//To turn into a 1-26 value
			}
			if(temp >= 97 && temp <= 122){//Lowercase
				intMessage.push_back(temp - 96);//To turn into a 1-26 value
			}
		}
	}

	decryptedMessage = crypterObject->decrypt(intMessage, intPswd);

	cout << endl << "Your unscrambled message is ";
	displayIntVector(decryptedMessage);
	cout << endl;
	//displayVectorInts(decryptedMessage);

	cout << "Would you like to save it to a file? Y/N ";
	string yesOrNo;
	getline(cin, yesOrNo);

	if(yesOrNo[0] == 'y' || yesOrNo[0] == 'Y'){
		cout << "What is the name of this file? ";
		string fileName;
		getline(cin, fileName);
		fileName.append(".txt");

		fstream dataFile;
		dataFile.open(fileName, ios::out);

		if(!dataFile){ cout << "Error opening file!!" << endl; }//Error message

		//dataFile << convertToString(encryptedMessage);//Writing to file
		for(vector<int>::iterator vIter = decryptedMessage.begin(); vIter != decryptedMessage.end(); vIter++){
			char temp = (char)(*vIter + 64);//To match ASCII value
			dataFile << temp;
		}

		cout << "Your file has been successfully created!! Please come again." << endl;
		dataFile.close();//Close the file

	}


	delete crypterObject;

}
