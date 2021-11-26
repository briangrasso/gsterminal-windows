#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <algorithm>


using namespace std;

string initLogin;
string userChoice = "\0";
string* initLoginPoint = &initLogin;
int totalCmds = 9;
bool choiceFlag = 0;
int counter = 0;
bool newUser = 0;
bool registeredUser = 0;
bool entryFlag = 0;
int y = 0;

vector<string> cmdList(totalCmds);

struct UserInfo
{
	UserInfo() {};
	UserInfo(const char* Username, const char* Password) {
#pragma warning(suppress:4996)
		strcpy(newUser, Username);
#pragma warning(suppress:4996)
		strcpy(newPassword, Password);
	}

	char newUser[10];
	char newPassword[10];
};

struct FileObj {
	FileObj() {};
	FileObj(const int reg) :registered(reg) {}

	friend void SaveRegUsrLs();
	friend void LoadRegUsrLs();

	int registered;
	vector<string> userList;
};

void UserPrompt();
void EntryPrompt();
int Login();
int InLogFunc();
void Verify();
void MakeUser();
void VerifyNew();
int Entry();
void ChoiceList();
void Help();
int Mail();
int HebrewConsole();
int Core();
void SaveRegUsrLs();
void LoadRegUsrLs();

UserInfo Admin;
FileObj UsrList;

int main() {
	cout << "Enter Main" << endl;
	//LoadRegUsrLs();

	Login();

	if (registeredUser == 1)
		Entry();
	else
		exit(0);

	return 0;
}

void UserPrompt() {
	cout << *initLoginPoint << ": ";
}

void EntryPrompt() {
	if (newUser == 0) {
		UserPrompt();
		cin >> userChoice;
	}
	else {
		cout << Admin.newUser << ": ";
		cin >> userChoice;
	}
}

void HebPrompt() {
	if (newUser == 0)
		UserPrompt();
	else
		cout << Admin.newUser << ": ";
}

int InLogFunc() {
	string* loginPoint = &initLogin;
	string openMeth = "userfiles/" + *loginPoint;
	string* openFile = &openMeth;

	if (initLogin == "new") {
		MakeUser();
		VerifyNew();
	}
	else if (initLogin == "\0") {
		do {
			++counter;
			cin >> initLogin;
			if (initLogin != "\0") {
				InLogFunc();
				break;
			}
			else
				continue;
		} while (counter <= 2);
	}
	else {
		for (int r = 0; r < UsrList.registered; ++r) {
			if (initLogin == UsrList.userList[r]) {
				registeredUser = 1;
				Verify();
				break;
			}
			else {
				cout << "Username not found" << endl;
				cout << "Enter username: ";
				cin >> initLogin;
				InLogFunc();			// add limiter for number of repeats
			}
		}
	}

	return 0;
}

int Login() {
	cout << "G# Management Platform" << endl;
	cout << "Enter 'new' to create a new account," << endl;
	cout << "or enter your username." << endl;
	cin >> initLogin;

	InLogFunc();

	return 0;
}

void Verify() {
	cout << "Please enter your password: " << endl;
	string verification = "\0";
	cin >> verification;

	string* initPoint = &initLogin;
	string methodOpen = "userfiles/" + *initPoint;
	string* Opener = &methodOpen;

	ifstream UserFile(*Opener, ios_base::in | ios_base::binary);

	UserInfo GetUser;

	if (UserFile.is_open()) {
		cout << "File opened" << endl;

		UserFile.read((char*)&GetUser, sizeof(GetUser));

		stringstream convertInfo;
		convertInfo << GetUser.newPassword;
		string compareVerify;
		convertInfo >> compareVerify;

		UserFile.close();

		if (verification == compareVerify) {
			cout << "Successful verification" << endl;
			entryFlag = 1;
		}
		else {
			cout << "Incorrect password." << endl;
			Verify();
		}
	}
	else {
		cout << "Verify function: Open failed" << endl;
		Verify();
	}

}

void VerifyNew() {
	if (registeredUser == 1) {
		cout << "Please enter your password." << endl;
		string verification = "\0";
		cout << Admin.newUser << ": ";
		cin >> verification;

		stringstream thisConverter;
		thisConverter << Admin.newUser;
		string stringConvert;
		thisConverter >> stringConvert;

		string* initPoint = &stringConvert;
		string methodOpen = "userfiles/" + *initPoint;
		string* Opener = &methodOpen;

		ifstream UserFile(*Opener, ios_base::in | ios_base::binary);

		if (UserFile.is_open()) {
			cout << "File opened" << endl;

			UserInfo GetUser;

			UserFile.read((char*)&GetUser, sizeof(GetUser));

			stringstream stringConvert;
			stringConvert << GetUser.newPassword;
			string compareVerNew;
			stringConvert >> compareVerNew;

			if (verification == compareVerNew) {
				UserFile.close();
				entryFlag = 1;
			}
			else {
				cout << "Wrong password" << endl;
				VerifyNew();
			}
		}
		else {
			cout << "Open Failed!!!" << endl;
			VerifyNew();
		}
	}
	else
		exit(0);

}

void MakeUser() {
	newUser = 1;
	cout << "Enter username: " << endl;
	cin >> Admin.newUser;

	if (Admin.newUser != "new") {
		cout << "Enter password: " << endl;
		cin >> Admin.newPassword;

		stringstream converterStream;
		converterStream << Admin.newUser;
		string convertedString;
		converterStream >> convertedString;

		string* filePointer = &convertedString;
		string openMethod = "userfiles/" + *filePointer;
		string* methodPoint = &openMethod;

		ofstream FileOut;
		FileOut.open(*methodPoint, ios_base::out | ios_base::binary);

		if (FileOut.is_open()) {
			FileOut.write(reinterpret_cast<const char*>(&Admin), sizeof(Admin));
			FileOut.close();
		}
		else
			cout << "Fail" << endl;

		UsrList.registered++;
		// UsrList.userList[UsrList.registered].push_back(Admin.newUser);
		UsrList.userList.push_back(Admin.newUser);
		registeredUser = 1;
	}
	else {
		cout << "Please select different username" << endl;             // need to make check to prevent new user from writing over existing user: userList reloader
		MakeUser();
	}
}

int Entry() {
	if (entryFlag == 1) {	// from Verify and VerifyNew for second verification for entry
		cout << "Welcome to the Main Menu" << endl;
		cout << "Enter 'help' to display commands." << endl;
		EntryPrompt();
		ChoiceList();
	}
	else
		exit(0);

	return 0;
}

void ChoiceList() {
	choiceFlag = 1; 	// to watch for if the cmdLine is in use. To be able to see anytime anyone is actively entering commands

	cmdList[0] = "help";
	cmdList[1] = "mail";
	cmdList[2] = "print";
	cmdList[3] = "text";
	cmdList[4] = "בש";
	cmdList[5] = "מחש";
	cmdList[6] = "core";
	cmdList[7] = "server";
	cmdList[8] = "exit";

	do {
		if (userChoice == cmdList[0]) {
			choiceFlag = 0;
			Help();
			break;
		}
		else if (userChoice == cmdList[1]) {
			choiceFlag = 0;
			Mail();
			break;
		}
		else if (userChoice == cmdList[2]) {
			cout << "print" << endl;
			choiceFlag = 0;
			break;
		}
		else if (userChoice == cmdList[3]) {
			cout << "text" << endl;
			choiceFlag = 0;
			break;
		}
		else if (userChoice == cmdList[4]) {
			choiceFlag = 0;
			HebrewConsole();
			break;
		}
		else if (userChoice == cmdList[5]) {
			cout << "calc" << endl;
			choiceFlag = 0;
			EntryPrompt();
			ChoiceList();       // temporary for testing !!!!!!!!!!!!!!!!
			break;
		}
		else if (userChoice == cmdList[6]) {
			choiceFlag = 0;
			Core();
			break;
		}
		else if (userChoice == cmdList[7]) {
			cout << "Start TestServer" << endl;		// write cmd to build and run C# obj.
			break;
		}
		else if (userChoice == cmdList[8]) {
			choiceFlag = 0;
			SaveRegUsrLs();		///////Where I link to FileObj to write userList and total registered to file////////////////////////////
			exit(0);
		}
		else {
			choiceFlag = 0;
			cout << "Please enter valid command, or enter" << endl;
			cout << "'help' to display a list of commands." << endl;
			EntryPrompt();
			ChoiceList();
			continue;
		}
	} while (userChoice != "\0");

	EntryPrompt();
	ChoiceList();
}

void Help() {
	choiceFlag = 0;
	for (int s = 0; s < totalCmds; ++s) {
		cout << cmdList[s] << endl;
	}
	EntryPrompt();
	ChoiceList();
}

int Mail() {
	cout << "mail" << endl;
	choiceFlag = 0;
	EntryPrompt();
	ChoiceList();

	return 0;
}

int HebrewConsole() {
	string hebChoice;
	HebPrompt();
	cin >> hebChoice;

	unsigned short int hebInd = 3;
	vector<string> HebCon(hebInd);
	HebCon[0] = "אזרה";
	HebCon[1] = "לב";	// instantiation of Core with Hebrew conversion -- move alephbet variables here and make a friend of ToneTable
	HebCon[2] = "מוצא";

	if (hebChoice == HebCon[0]) {
		cout << "----------- פקודות -----------" << endl;
		for (int b = 0; b < hebInd; ++b) {
			cout << HebCon[b] << endl;
		}
	}
	else if (hebChoice == HebCon[1]) {
		cout << "הציג הלב" << endl;
		cout << "here is where I will be able to manipulate the core in Hebrew" << endl;
	}
	else if (hebChoice == HebCon[2]) {
		ChoiceList();
	}
	else {
		cout << "נא בזן פקודה חוקית" << endl;
		HebrewConsole();
	}

	HebrewConsole();

	return 0;
}

int Core() {
	cout << "Core" << endl;

	wchar_t AlephBet[22] = { L'א', L'ב', L'ג', L'ד', L'ה', L'ו', L'ז', L'ח', L'ט', L'י', L'כ', L'ל', \
							L'מ', L'נ', L'ס', L'ע', L'פ', L'צ', L'ק', L'ר', L'ש', L'ת' };

	struct ToneTable {
		ToneTable() {};
		~ToneTable() {};
		ToneTable(short int tone1, short int tone2, short int tone3, short int tone4, short int tone5, short int tone6, \
			short int tone7, short int tone8, short int tone9, short int tone10, short int tone11, \
			short int tone12, short int tone13, short int tone14, short int tone15, short int tone16, \
			short int tone17, short int tone18, short int tone19, short int tone20, short int tone21, \
			short int tone22, short int tone23, short int tone24, short int tone25, short int tone26, \
			short int tone27, short int tone28, short int tone29, short int tone30, short int tone31, \
			short int tone32, short int tone33, short int tone34, short int tone35, short int tone36, \
			short int tone37, short int tone38, short int tone39, short int tone40, short int tone41, \
			short int tone42, short int tone43, short int tone44, short int tone45, short int tone46, \
			short int tone47, short int tone48, short int tone49, short int tone50, short int tone51, \
			short int tone52, short int tone53, short int tone54, short int tone55, short int tone56, \
			short int tone57, short int tone58, short int tone59, short int tone60, short int tone61, \
			short int tone62, short int tone63, short int tone64, short int tone65, short int tone66, \
			short int tone67, short int tone68, short int tone69, short int tone70, short int tone71, \
			short int tone72, short int tone73, short int tone74, short int tone75, short int tone76, \
			short int tone77, short int tone78, short int tone79, short int tone80, short int tone81, \
			short int tone82, short int tone83, short int tone84, short int tone85, short int tone86, \
			short int tone87, short int tone88, short int tone89, short int tone90, short int tone91, \
			short int tone92, short int tone93, short int tone94, short int tone95, short int tone96, \
			short int tone97, short int tone98, short int tone99, short int tone100, short int tone101, \
			short int tone102, short int tone103, short int tone104, short int tone105, short int tone106, \
			short int tone107, short int tone108, short int tone109, short int tone110, short int tone111, \
			short int tone112, short int tone113, short int tone114, short int tone115, short int tone116, \
			short int tone117, short int tone118, short int tone119, short int tone120, short int tone121, \
			short int tone122, short int tone123, short int tone124, short int tone125, short int tone126): \
			EO1(tone1), AO1(tone2), DO1(tone3), GO1(tone4), BO1(tone5), EO2(tone6), \
			FO1(tone7), Bf1(tone8), Ef1(tone9), GS1(tone10), CO1(tone11), FO2(tone12), \
			FS1(tone13), BO2(tone14), EO3(tone15), AO2(tone16), Df1(tone17), FS2(tone18), \
			GO2(tone19), CO2(tone20), FO3(tone21), Bf2(tone22), DO2(tone23), GO3(tone24), \
			GS2(tone25), Df2(tone26), FS3(tone27), BO3(tone28), Ef2(tone29), GS3(tone30), \
			AO3(tone31), DO3(tone32), GO4(tone33), CO3(tone34), EO4(tone35), AO4(tone36), \
			Bf3(tone37), Ef3(tone38), GS4(tone39), Df3(tone40), FO4(tone41), Bf4(tone42), \
			BO4(tone43), EO5(tone44), AO5(tone45), DO4(tone46), FS4(tone47), BO5(tone48), \
			CO4(tone49), FO5(tone50), Bf5(tone51), Ef4(tone52), GO5(tone53), CO5(tone54), \
			Df4(tone55), FS5(tone56), BO6(tone57), EO6(tone58), GS5(tone59), Df5(tone60), \
			DO5(tone61), GO6(tone62), CO6(tone63), FO6(tone64), AO6(tone65), DO6(tone66), \
			Ef5(tone67), GS6(tone68), Df6(tone69), FS6(tone70), Bf6(tone71), Ef6(tone72), \
			EO7(tone73), AO7(tone74), DO7(tone75), GO7(tone76), BO7(tone77), EO8(tone78), \
			FO7(tone79), Bf7(tone80), Ef7(tone81), GS7(tone82), CO7(tone83), FO8(tone84), \
			FS7(tone85), BO8(tone86), EO9(tone87), AO8(tone88), Df7(tone89), FS8(tone90), \
			GO8(tone91), CO8(tone92), FO9(tone93), Bf8(tone94), DO8(tone95), GO9(tone96), \
			GS8(tone97), Df8(tone98), FS9(tone99), BO9(tone100), Ef8(tone101), GS9(tone102), \
			AO9(tone103), DO9(tone104), GOt(tone105), CO9(tone106), EOt(tone107), AOt(tone108), \
			Bf9(tone109), Ef9(tone110), GSt(tone111), Df9(tone112), FOt(tone113), Bft(tone114), \
			BOt(tone115), EOe(tone116), AOe(tone117), DOt(tone118), FSt(tone119), BOe(tone120), \
			COt(tone121), FOe(tone122), Bfe(tone123), Eft(tone124), GOe(tone125), COe(tone126) {}
		// Associations can be altered to add a SECOND level of encryption = 2 keys to enter plus Username and password to decrypt
		// re-write as dynamic array that can alter which tone is being placed in which tone## later for further "attenuation" -- encoding

		int GetValues(int forKey) {
			ToneTable Core;

			int index = 1;
			int* point = &index;

			cout << "Values in Key:" << endl;

			string pathmaker = "progdata/valuesheet";
			string* opener = &pathmaker;

			if (forKey >= -4 && forKey <= 7) {
				ofstream ValueSheet(*opener, ios_base::out);

				if (ValueSheet.is_open()) {
					// six rows of 21 -- make 5 sets of variables that point to x,y; add 21; and substitute to each row
					unsigned short int cOne = 1;
					unsigned short int cTwo = 22;
					unsigned short int cThree = 43;
					unsigned short int cFour = 64;
					unsigned short int cFive = 85;
					unsigned short int cSix = 106;

					do {
						for (int x = 0; x < 1; ++x) {
							for (int y = 0; y < 21; ++y) {
								ToneTable gs;
								int a = x;
								int b = y + 21;
								int c = x;
								int d = y + 42;
								int e = x;
								int f = y + 63;
								int g = x;
								int h = y + 84;
								int i = x;
								int j = y + 105;
								ValueSheet << "tone[" << cOne << "]  " << &gs.fretBoard[x][y] << "  tone[" << cTwo << "]  " << &gs.fretBoard[a][b] << "  tone[" << cThree << "]  " << \
									& gs.fretBoard[c][d] << "  tone[" << cFour << "]  " << &gs.fretBoard[e][f] << "  tone[" << cFive << "]  " << &gs.fretBoard[g][h] << "  tone[" << cSix << "]  " \
									<< &gs.fretBoard[i][j] << endl;

								++cOne;
								++cTwo;
								++cThree;
								++cFour;
								++cFive;
								++cSix;
							}
						}
					} while (cOne < 21);

					do {
						for (int x = 0; x < 6; ++x) {
							for (int y = 0; y < 21; ++y) {
								int* toKey = &forKey;
								int values = { Core.fretBoard[x][y] + (*toKey) };
								int* valuePoint = &values;
								ValueSheet << "tone[" << *point << "]   \t" << *valuePoint << endl;
								++index;
							}
						}
					} while (index < 126);
					ValueSheet.close();
				}
				else
					cout << "Failed" << endl;

				ifstream SameSheet(*opener, ios_base::in);

				if (SameSheet.is_open()) {
					string sheetContent;
					while (SameSheet.good()) {
						getline(SameSheet, sheetContent);
						cout << sheetContent << endl;
					}
					SameSheet.close();
				}
				else
					cout << "File open failed" << endl;
			}
			else {
				cout << "Invalid entry." << endl;
				cout << "GetValues function" << endl;
				cout << "Enter Key Value (-4 through 7): ";
				int repeat = 0;
				cin >> repeat;
				ToneTable Repeat;
				Repeat.GetValues(repeat);
			}

			return 0;
		}

		short int GS1 = 10, \
			GS2 = 25, GS3 = 30, GS4 = 39, GS5 = 59, GS6 = 68, GS7 = 82, GS8 = 97, GS9 = 102, GSt = 111, \
			Bf1 = 8, Bf2 = 22, Bf3 = 37, Bf4 = 42, Bf5 = 51, Bf6 = 71, Bf7 = 80, Bf8 = 94, Bf9 = 109, Bft = 114, Bfe = 123, \
			CO1 = 11, CO2 = 20, CO3 = 34, CO4 = 49, CO5 = 54, CO6 = 63, CO7 = 83, CO8 = 92, CO9 = 106, COt = 121, COe = 126, \
			Df1 = 17, Df2 = 26, Df3 = 40, Df4 = 55, Df5 = 60, Df6 = 69, Df7 = 89, Df8 = 98, Df9 = 112, \
			Ef1 = 9, Ef2 = 29, Ef3 = 38, Ef4 = 52, Ef5 = 67, Ef6 = 72, Ef7 = 81, Ef8 = 101, Ef9 = 110, Eft = 124, \
			FO1 = 7, FO2 = 12, FO3 = 21, FO4 = 41, FO5 = 50, FO6 = 64, FO7 = 79, FO8 = 84, FO9 = 93, FOt = 113, FOe = 122, \
			GO1 = 4, GO2 = 19, GO3 = 24, GO4 = 33, GO5 = 53, GO6 = 62, GO7 = 76, GO8 = 91, GO9 = 96, GOt = 105, GOe = 125;

		short int EO1 = 1, FS1 = 13, AO3 = 31, BO4 = 43, DO5 = 61, EO7 = 73, FS7 = 85, AO8 = 88, BOt = 115, \
			AO1 = 2, BO2 = 14, DO3 = 32, EO5 = 44, FS5 = 56, AO7 = 74, BO8 = 86, DO9 = 104, EOt = 107, EOe = 116, \
			DO1 = 3, EO3 = 15, FS3 = 27, AO5 = 45, BO6 = 57, DO7 = 75, EO8 = 78, EO9 = 87, FS9 = 99, AOe = 117, \
			AO2 = 16, BO3 = 28, DO4 = 46, EO6 = 58, FS6 = 70, BO9 = 100, AO9 = 103, DOt = 118, \
			BO1 = 5, DO2 = 23, EO4 = 35, FS4 = 47, AO6 = 65, BO7 = 77, DO8 = 95, FSt = 119, \
			EO2 = 6, FS2 = 18, AO4 = 36, BO5 = 48, DO6 = 66, FS8 = 90, AOt = 108, BOe = 120;

		short int fretBoard[6][21] =
		{ {EO1, FO1, FS1, GO2, GS2, AO3, Bf3, BO4, CO4, Df4, DO5, Ef5, EO7, FO7, FS7, GO8, GS8, AO9, Bf9, BOt, COt}, \
		 {AO1, Bf1, BO2, CO2, Df2, DO3, Ef3, EO5, FO5, FS5, GO6, GS6, AO7, Bf7, BO8, CO8, Df8, DO9, Ef9, EOe, FOe}, \
		 {DO1, Ef1, EO3, FO3, FS3, GO4, GS4, AO5, Bf5, BO6, CO6, Df6, DO7, Ef7, EO9, FO9, FS9, GOt, GSt, AOe, Bfe}, \
		 {GO1, GS1, AO2, Bf2, BO3, CO3, Df3, DO4, Ef4, EO6, FO6, FS6, GO7, GS7, AO8, Bf8, BO9, CO9, Df9, DOt, Eft}, \
		 {BO1, CO1, Df1, DO2, Ef2, EO4, FO4, FS4, GO5, GS5, AO6, Bf6, BO7, CO7, Df7, DO8, Ef8, EOt, FOt, FSt, GOe}, \
		 {EO2, FO2, FS2, GO3, GS3, AO4, Bf4, BO5, CO5, Df5, DO6, Ef6, EO8, FO8, FS8, GO9, GS9, AOt, Bft, BOe, COe} };

		short int aleph = fretBoard[1][10] + fretBoard[2][11] + fretBoard[3][10] + fretBoard[4][11];
		short int bet = fretBoard[0][3] + fretBoard[1][3] + fretBoard[2][5] + fretBoard[3][5] + fretBoard[4][4] + fretBoard[5][3];
		short int gimmel = fretBoard[0][11] + fretBoard[1][11] + fretBoard[2][13] + fretBoard[3][12] + fretBoard[4][13] + fretBoard[5][11];
		short int dalet = fretBoard[0][4] + fretBoard[1][4] + fretBoard[2][6] + fretBoard[3][6] + fretBoard[4][4] + fretBoard[5][4];
		short int heh = fretBoard[0][1] + fretBoard[1][3] + fretBoard[2][1] + fretBoard[3][1] + fretBoard[4][1] + fretBoard[5][3];
		short int vav = fretBoard[0][3] + fretBoard[1][3] + fretBoard[2][6] + fretBoard[3][5] + fretBoard[4][4] + fretBoard[5][3];
		short int zayin = fretBoard[0][10] + fretBoard[1][10] + fretBoard[2][10] + fretBoard[3][12] + fretBoard[4][13] + fretBoard[5][11];
		short int chet = fretBoard[0][6] + fretBoard[1][8] + fretBoard[2][6] + fretBoard[3][6] + fretBoard[4][6] + fretBoard[5][9];
		short int tet = fretBoard[0][11] + fretBoard[1][11] + fretBoard[2][13] + fretBoard[3][13] + fretBoard[4][13] + fretBoard[5][11];
		short int yod = fretBoard[0][4] + fretBoard[1][4] + fretBoard[2][3];
		short int kaf = fretBoard[0][3] + fretBoard[1][3] + fretBoard[2][3] + fretBoard[3][5] + fretBoard[4][4] + fretBoard[5][4];
		short int lamed = fretBoard[0][8] + fretBoard[1][11] + fretBoard[2][10] + fretBoard[3][8] + fretBoard[4][9] + fretBoard[5][8];
		short int mem = fretBoard[0][8] + fretBoard[1][10] + fretBoard[2][8] + fretBoard[3][10] + fretBoard[4][11] + fretBoard[5][8];
		short int nun = fretBoard[1][11] + fretBoard[2][8] + fretBoard[3][10] + fretBoard[4][11];
		short int samech = fretBoard[1][4] + fretBoard[2][3] + fretBoard[3][5] + fretBoard[4][4];
		short int ayin = fretBoard[0][6] + fretBoard[1][8] + fretBoard[2][8] + fretBoard[3][6] + fretBoard[4][6] + fretBoard[5][6];
		short int pe = fretBoard[0][9] + fretBoard[1][11] + fretBoard[2][11] + fretBoard[3][10] + fretBoard[4][9] + fretBoard[5][9];
		short int tzade = fretBoard[0][4] + fretBoard[1][6] + fretBoard[2][5] + fretBoard[3][6] + fretBoard[4][4] + fretBoard[5][4];
		short int qoph = fretBoard[0][1] + fretBoard[1][1] + fretBoard[2][1] + fretBoard[3][3] + fretBoard[4][4] + fretBoard[5][3];
		short int resh = fretBoard[1][8] + fretBoard[3][8] + fretBoard[4][9] + fretBoard[5][11];
		short int shin = fretBoard[0][6] + fretBoard[1][6] + fretBoard[2][8] + fretBoard[3][6] + fretBoard[4][8] + fretBoard[5][9];
		short int tav = fretBoard[0][6] + fretBoard[1][8] + fretBoard[2][8] + fretBoard[3][6] + fretBoard[4][6] + fretBoard[5][8];

		/*
		unsigned short int fretBoard[6][21] =		// ex. ToneTable GS; cout << GS.MakeRegisters.fretBoard[x][y];
		{ {GS.EO1, GS.FO1, GS.FS1, GS.GO2, GS.GS2, GS.AO3, GS.Bf3, GS.BO4, GS.CO4, GS.Df4, GS.DO5, GS.Ef5, GS.EO7, GS.FO7, GS.FS7, GS.GO8, GS.GS8, \
		GS.AO9, GS.Bf9, GS.BOt, GS.COt}, {GS.AO1, GS.Bf1, GS.BO2, GS.CO2, GS.Df2, GS.DO3, GS.Ef3, GS.EO5, GS.FO5, GS.FS5, GS.GO6, GS.GS6, GS.AO7, \
		GS.Bf7, GS.BO8, GS.CO8, GS.Df8, GS.DO9, GS.Ef9, GS.EOe, GS.FOe}, {GS.DO1, GS.Ef1, GS.EO3, GS.FO3, GS.FS3, GS.GO4, GS.GS4, GS.AO5, GS.Bf5, \
		GS.BO6, GS.CO6, GS.Df6, GS.DO7, GS.Ef7, GS.EO9, GS.FO9, GS.FS9, GS.GOt, GS.GSt, GS.AOe, GS.Bfe}, {GS.GO1, GS.GS1, GS.AO2, GS.Bf2, GS.BO3, \
		GS.CO3, GS.Df3, GS.DO4, GS.Ef4, GS.EO6, GS.FO6, GS.FS6, GS.GO7, GS.GS7, GS.AO8, GS.Bf8, GS.BO9, GS.CO9, GS.Df9, GS.DOt, GS.Eft}, {GS.BO1, \
		GS.CO1, GS.Df1, GS.DO2, GS.Ef2, GS.EO4, GS.FO4, GS.FS4, GS.GO5, GS.GS5, GS.AO6, GS.Bf6, GS.BO7, GS.CO7, GS.Df7, GS.DO8, GS.Ef8, GS.EOt, \
		GS.FOt, GS.FSt, GS.GOe}, {GS.EO2, GS.FO2, GS.FS2, GS.GO3, GS.GS3, GS.AO4, GS.Bf4, GS.BO5, GS.CO5, GS.Df5, GS.DO6, GS.Ef6, GS.EO8, GS.FO8, \
		GS.FS8, GS.GO9, GS.GS9, GS.AOt, GS.Bft, GS.BOe, GS.COe} };
		// Make register pointers in different method
		//return 0;
		//}
		*/
	};

	ToneTable GS;

	auto* pointer = &GS.aleph;
	const vector<int> letterArray{ GS.aleph, GS.bet, GS.gimmel, GS.dalet, GS.heh, GS.vav, GS.zayin, GS.chet, GS.tet, GS.yod, GS.kaf, GS.lamed, \
		GS.mem, GS.nun, GS.samech, GS.ayin, GS.pe, GS.tzade, GS.qoph, GS.resh, GS.shin, GS.tav };
	const vector<string> letterWords{ "Aleph", "Bet", "Gimmel", "Dalet", "Heh", "Vav", "Zayin", "Chet", "Tet", "Yod", "Kaf", "Lamed", "Mem", "Nun", \
		"Samech", "Ayin", "Pe", "Tzade", "Qoph", "Resh", "Shin", "Tav" };
	for (int w = 0; w < 22; ++w) {
		cout << letterWords[w] << " is at:\t" << &letterArray[w] << "\tand it contains:\t" << letterArray[w] << endl;
	}

	cout << "===============" << endl;
	cout << "GetValues function" << endl;
	cout << "Enter Key Value (-4 through 7): ";
	int keyValue = 0;
	cin >> keyValue;
	GS.GetValues(keyValue);
	cout << "==============" << endl;
	cout << "GS.EOt at:\t" << &GS.EOt << "\tcontains:\t" << GS.EOt << endl;
	cout << "GS.AOt at:\t" << &GS.AOt << "\tcontains:\t" << GS.AOt << endl;
	cout << "GS.Bf9 at:\t" << &GS.Bf9 << "\tcontains:\t" << GS.Bf9 << endl;
	cout << "==============" << endl;
	cout << sizeof(GS) << endl;
	// cout << sizeof(GS
	return 0;
}

void SaveRegUsrLs() {
	cout << "Saving userList" << endl;

	string usrLsPath = "progdata/usrLs";
	string* pathUsrLs = &usrLsPath;

	ofstream UserList(*pathUsrLs, ios_base::out | ios_base::binary);

	if (UserList.is_open()) {
		int y = 0;
		UserList << UsrList.registered << endl;
		do {
			//for(int x = 0; x < UsrList.registered; ++x){
			UserList << UsrList.userList[y] << endl;
			++y;
			//}
		} while (y <= UsrList.registered);
		cout << "Saved UsrList to file" << endl;
		UserList.close();
	}
	else {
		cout << "saving UserList vector to file failed" << endl;
	}
}

void LoadRegUsrLs() {
	cout << "Loading userList" << endl;
	string fileLoad = "progdata/usrLs";
	string* openLoader = &fileLoad;

	ifstream LoadFile(*openLoader, ios_base::in | ios_base::binary);

	if (LoadFile.is_open()) {
		FileObj UsrVec;
		LoadFile.read((char*)&UsrVec, sizeof(UsrVec));
		//UsrList.registered = UsrVec.registered;
		//cout << "==============" << UsrList.registered << "==============" << endl;
		stringstream conRegistered;
		conRegistered << UsrVec.registered;
		int newRegistered;
		conRegistered >> newRegistered;
		cout << newRegistered << endl;
		cout << "loaded registered" << endl;
		for (int t = 0; t < UsrVec.registered; ++t) {
			UsrList.userList.push_back(UsrVec.userList[t]);
		}
		cout << "==========================userlist loaded=============================" << endl;
		for (int z = 0; z < UsrVec.registered; ++z) {
			cout << UsrList.userList[z] << endl;
		}
		LoadFile.close();
	}
	else
		cout << "LoadRegUsrLs() failed" << endl;
}
