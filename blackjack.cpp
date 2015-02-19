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
#include "deck.cpp"
using namespace std;


#ifndef __BLACKJACK__
#define __BLACKJACK__

//a class to enable the game of blackjack
class BlackJack{


//everything is public for now
public:

	Deck d = Deck();

	int dealCount = 0;
	int dealThreshold = 6;	//7 hands

	vector<float> perfectCount = vector<float>();

							//should be in bjcontroller but more efficient to have it here and
							// keep it updated on draws n such

	//good idea to keep track of these, calls to evaluateHand might get excessive otherwise
	int playerTotal = 0;	//with aces acting as 11 if possible
	int playerLow = 0;	//with aces acting as 1
	int dealerTotal = 0;
	int dealerLow = 0;

	//keeps track of the blackjack-style values of the cards in each hand
	vector<int> playerHand = vector<int>();
	vector<int> dealerHand = vector<int>();

	//keeps track of the actual values without information lost in specialValue
	vector<int> playerRealHand = vector<int>();
	vector<int> dealerRealHand = vector<int>();

	BlackJack(){
		initPerfectCount();
	}


//////////////////////GAME LOGIC FUNCTIONS /////////////////////////////

	//ie, what this class is meant to do

	/*
		Give the player a card and update all the logs
	*/
	void playerHit(){
		int temp = d.draw();
		int tempV = d.getValue( temp );
		int tempS = specialValue( tempV );
		perfectCount[tempS]++;
		perfectCount[0]++;
		playerHand.push_back( tempS );
		playerRealHand.push_back(temp);

		updatePlayerTotal();	//really should not happen :(
	}

	/*
		Give the dealer a card and update all the logs
	*/
	void dealerHit(){
		int temp = d.draw();
		int tempS = specialValue( d.getValue( temp ) );
		dealerHand.push_back( tempS );
		perfectCount[tempS]++;
		perfectCount[0]++;
		dealerRealHand.push_back(temp);
		updateDealerTotal();	//really should not happen :(
					//What I mean to say is, this should happen automatically inline here
					//could do some other tricky stuff like pass updateDT a # or even
					//make it assume the last card... yeah... that's a good idea
	}

	/*
		These two essentially cache the current hand totals
		They work in tandem with playerHit() and dealerHit()
		evaluateHand is a mess of an expensive function,
		but these actually reduce the number of times it would need to be called
	*/
	void updatePlayerTotal(){
		playerTotal = evaluateHand(playerHand);
		playerLow = lowCount;			//yeah, this is good code, heh
	}

	void updateDealerTotal(){
		dealerTotal = evaluateHand(dealerHand);
		dealerLow = lowCount;			//nope nothing wrong with this
	}


	/*
		Deal 2 cards to the player and the dealer and update their totals
		update all the crazy logs and other nonsense I've tacked on to this class too
	*/
	void initialDeal(){
		playerHit(); dealerHit();
		playerHit(); dealerHit();
	}

	/*
		Logs every card that comes out of the deck
		Using a float vector for the sake of future endeavors
	*/
	void initPerfectCount(){
		perfectCount = vector<float>();
		for(int ii = 0; ii <= 10; ii++){
			perfectCount.push_back(0);
		}
	}

	/*
		Return the specialValue() of the card the dealer has face-up
	*/
	int dealerShowing(){
		return dealerHand[0];
	}


	/*
		Check if the dealer or player busted or the dealer must hit
	*/
	bool playerBustedP(){
		return (playerTotal > 21 );
	}

	bool dealerBustedP(){
		return (dealerTotal > 21 );
	}
	/*
		Does the dealer need to hit?
	*/
	bool dealerMustHitP(){
		if(dealerTotal != dealerLow){	//we must have an ace acting as an 11
			return dealerTotal <= 17;	//hit on 17
		}else{
			return dealerTotal < 17;	//hit less than 17
		}
	}

	/*
		assumes the hand has been transformed already using specialValue()
		gets as close to 21 as it can without going over (referring to Aces being 1 or 11)
		relies on the fact that only one ace can act as 11.
	*/
	int lowCount;		//essentially return value # 2, gives the low estimate
	int evaluateHand (vector<int> hand) {
		bool sawAce = false;
		int sum = 0;
		for(int ii = 0; ii < hand.size(); ii++){	//sum up a low estimate (A = 1)
			if(!sawAce && hand[ii] == 1){
				sawAce = true;
			}
			sum += hand[ii];
		}
		lowCount = sum;	//record the low count with all aces = 1
		if(sawAce && ((sum + 10) <= 21) ){	//there's an ace and we can add 10 safely
			sum += 10;
		}
		return sum;
	}

	//makes A = 1; Q, J, K, and 10 = 10; and numbers = themselves
	//assumes you've done a getValue inside deck
	int specialValue (int x){
		switch(x){
			case 0:
			case 11:
			case 12:
				return 10;
			default:
				return x;
		}
	}


	void resetTable(){
		playerTotal = 0;
		dealerTotal = 0;
		playerLow = 0;
		dealerLow = 0;
		playerHand = vector<int>();
		dealerHand = vector<int>();
	}
	
	void tryShuffle(){
		if(dealCount >= dealThreshold){ //(! dealCount < dealThreshold)
			d.shuffle();
			dealCount = 0;
			initPerfectCount();
		}else{
			dealCount++;
		}
	}

};


#endif


//graveyard






/*
	int updateCardCount(){
		cardCount += countCards();
	}

	int countCards(){
		int sum = 0;
		for(int ii = 0; ii < playerHand.size(); ii++){
			if(playerHand[ii] < 7){
				sum --;
			}else if(playerHand[ii] > 9){
				sum++;
			}
		}
		for(int ii = 0; ii < dealerHand.size(); ii++){
			if(dealerHand[ii] < 7){
				sum --;
			}else if(dealerHand[ii] > 9){
				sum++;
			}
		}
		return sum;
	}


	
		Should be called at the end of a hand
		DOES NOT HAPPEN AUTOMATICALLY
	
	int updateTenCount(){
		tenCount += countTens();
	}

	int countTens(){
		int sum = 0;
		for(int ii = 0; ii < playerHand.size(); ii++){
			if(playerHand[ii] == 10){
				sum ++;
			}
		}
		for(int ii = 0; ii < dealerHand.size(); ii++){
			if(dealerHand[ii] == 10){
				sum ++;
			}
		}
		return sum;
	}

*/



