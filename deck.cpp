#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <fann.h>
#include <floatfann.h>
#include <fann_cpp.h>
using namespace std;


#ifndef __DECK__
#define __DECK__

/*
	Represents a deck of playing cards.

	Value key:
	A = 1
	1-10 = 1-10
	0 = King
	11 = Jack
	12 = Queen
*/
class Deck{

	public:

	vector<int> cards = vector<int>();

	int index = 0;

	Deck(){
		srand ( unsigned ( std::time(0) ) );
		init();
	}

	void init(){
		for(int ii = 0; ii < 52; ii++){
			cards.push_back(ii);
		}
		shuffle();
	}

	void shuffle(){
		random_shuffle ( cards.begin(), cards.end() );
		index = 0;
	}

	//Applies / 13 to separate 52 cards into 4 classes (0, 1, 2, 3)
	//simply truncate the result to class [0, 51] into {0,1,2,3}
	int getSuit(int x){
		return x/13;
	}

	//Applies % 13 to separate the 52 cards into 13 classes
	int getValue(int x){
		return x%13;
	}

	//show the letter or number for human reading
	//assumes getSuit has already been applied
	string getHumanValue(int x){
		switch(x){
			case 1:
				return "Ace";
			case 0:
				return "King";
			case 11:
				return "Jack";
			case 12:
				return "Queen";
			case 10:
				return "10";
			default:
				char c = x + '0';
				string r = "";
				r += c;
				return string(r);
		}
	}

	//show the suit for human reading
	string getHumanSuit(int x){
		switch(x){
			case 0:
				return "heart";
			case 1:
				return "spade";
			case 2:
				return "diamond";
			case 3:
				return "club";
		}
	}

	int draw(){
		return cards[index++];
	}

	string getHumanReadable(int x){
		//string value = to_string(getValue(x));
		return string(
				getHumanValue(getValue(x))
				+ "\t of \t"
				+ getHumanSuit(getSuit(x)) + "s "
			);
	}

	string getHumanReadable(vector<int> X){
		string retval = "";
		for(int ii = 0; ii < X.size(); ii++){
			retval += getHumanReadable(X[ii]) + "\n";
		}
		return string(retval);
	}

	void print(){
		cout << getHumanReadable(cards);
	}
};



#endif







//graveyard




/*
		for(int ii = 0; ii < tenTally.size(); ii++){
			int sum = tenTally[ii][0] + tenTally[ii][1] + tenTally[ii][2];
			cout 	<< ii << ","
				<< (double)tenTally[ii][0]/sum << ","
				<< (double)tenTally[ii][1]/sum << ","
				<< (double)tenTally[ii][2]/sum << ","
				<< sum << endl;
		}

		cout << endl << endl << "Card count: " << endl;

		for(int ii = 0; ii < cardTally.size(); ii++){
			int sum = cardTally[ii][0] + cardTally[ii][1] + cardTally[ii][2];
			cout 	<< ii << ","
				<< (double)cardTally[ii][0]/sum << ","
				<< (double)cardTally[ii][1]/sum << ","
				<< (double)cardTally[ii][2]/sum << ","
				<< sum << endl;
		}
*/
