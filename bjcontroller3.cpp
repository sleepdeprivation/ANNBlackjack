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
#include "blackjack.cpp"
using namespace std;

#ifndef __BJCONTROL__
#define __BJCONTROL__


class BJController{

	public:

	BlackJack game = BlackJack();

	vector<vector<float>> data = vector<vector<float>>();
	vector<vector<float>> winData = vector<vector<float>>();
	vector<vector<float>> lossData = vector<vector<float>>();

	int trials = 0;
	int wins = 0;
	int losses = 0;
	int pushes = 0;

	int correctPredictions = 0;

	int bankroll = 2000;	//2000 "chips" or "bets"
	vector<int> bankrollHistory = vector<int>();

	vector<float> incorrectPList = vector<float>();

	vector<float> correctPList = vector<float>();

	vector<float> fullSize = vector<float>();

	vector<float> probabilities = vector<float>();

	BJController(){
		//get a vector that represents a full deck
		fullSize.push_back(52);
		for(int ii = 1; ii < 10; ii++){
			fullSize.push_back(4);
		}
		fullSize.push_back(16);

	}


	void generateProbabilities(){
		float cardsRemaining = (fullSize[0]-game.perfectCount[0]);
		float runningTotal = 0;
		probabilities = vector<float>();
		probabilities.push_back(0);
		for(int ii = 1; ii < fullSize.size(); ii++){
			runningTotal += ((fullSize[ii]-game.perfectCount[ii])/cardsRemaining);
			probabilities.push_back(runningTotal);
		}
	}

	void displayAllProbabilities(){
		cout << "probabilities: " << endl;
		for(int ii = 0; ii < probabilities.size(); ii++){
			cout << ii << ":\t" << probabilities[ii] << endl;
		}
	}


	/*
		Check if the dealer, player or both got a blackjack
	*/
	int blackjackCheck(){
		if(game.playerTotal == 21 && game.dealerTotal != 21){		//player won
			return 3;
		}else if(game.playerTotal != 21 && game.dealerTotal == 21){	//dealer won
			return 1;
		}else if(game.playerTotal == 21 && game.dealerTotal == 21){	//push
			return 2;
		}else{
			return -1;
		}
	}


	/*
		Assumes we've already checked if the player busted
	*/
	int determineWinner(){
		if(game.dealerBustedP()){
			return 0;		//player win
		}
		if(game.dealerTotal > game.playerTotal){
			return 1;		//dealer win
		}else if(game.dealerTotal < game.playerTotal){
			return 0;		//player win
		}else{
			return 2;		//push
		}
	}

	
	//what is the probability of drawing a card on the interval [A, B] ?
	float getProbabilityRange(int A, int B){
		return probabilities[B] - probabilities[A-1];
	}


	float getSingleProbability(int s){
		  return probabilities[s] - probabilities[s-1];
	}


	//vector<vector<float>> data = vector<vector<float>>();

	int playProbabilityBlackjack(float P){
		game.initialDeal();

		int autoWin = blackjackCheck();

		if(autoWin != -1) { return autoWin; }

		generateProbabilities();
 
		while(!game.playerBustedP() && ((game.playerTotal < 11)  || (probabilities[21-game.playerTotal] > P) )){

			game.playerHit();

			generateProbabilities();

		}
		if(game.playerBustedP()){
			return 1;	//dealer win
		}
		while(game.dealerMustHitP()){
			game.dealerHit();
		}

		int winner = determineWinner();
		return winner;
	}


	


	void playNProbabilityBlackjack(int N, float P, double c){
		double wins = 0;
		double losses = 0;
		double pushes = 0;
		int predictions = 0;
		int predictedWin = 0;
		int predictedLoss = 0;

		vector<float> single;

		for(int ii = 0; ii < N; ii++){

			bankrollHistory.push_back(bankroll);

			int bet = 0;

			int prediction = -1;

			//collecting data
			generateProbabilities();
			single = vector<float>();	//11 is the outcome, 0 is the percentage of cards revealed
			single.push_back(1);//game.perfectCount[0]/fullSize[0]);
			for(int ii = 1; ii <= 10; ii++){
				single.push_back(getSingleProbability(ii));
			}

			if(getProbabilityRange(1, 6) < .3){
				bet = 6;
				bankroll -= bet;
			}

			int result = playProbabilityBlackjack(P);
			if(result == 0){
				single.push_back(0); data.push_back(single);	//record a win
				winData.push_back(single);
				bankroll += bet*2;				//give the player $
				wins ++;					//tally the win
			}else if(result == 1){
				single.push_back(1); data.push_back(single);	//record loss
				lossData.push_back(single);
				losses ++;					//tally it
			}else if(result == 3){
				single.push_back(0); data.push_back(single);	//record a win
				winData.push_back(single);
				bankroll += bet + bet/2;				//give the player $
				wins += 1.5;
			}else{
				bankroll+= bet;					//nobody wins
				pushes ++;					//tie
			}
			game.resetTable();
			game.tryShuffle();

		}

		predictions = predictedWin+predictedLoss;

		float WLP = wins+losses+pushes;

		float winP = (float)wins/WLP;
		float lossP = (float)losses/WLP;
		float pushP = (float)pushes/WLP;
		cout << "win percent : " << winP << endl;
		cout << "loss percent : " << lossP << endl;
		cout << "push percent : " << pushP << endl;

		cout << "bankroll remaining : " << bankroll << endl;


	}


	void generateStats(vector<vector<float>> d){
		int w = 0;
		int l = 0;
		//int push = 0;
		int pW = 0;
		int pL = 0;
		int N = d.size();


		for(int ii = 0; ii < N; ii++){
			int P = d[ii].size();
			if(d[ii][P-1] == 1)
				if(d[ii][P-2] == 1)
					pL++;
				else
					pW++;
			if(d[ii][P-2] == 1)
				l++;
			else
				w++;

			/*for(int kk = 0; kk < d[N-1].size(); kk++){
				cout << d[ii][kk] << " ";
			}	cout << endl;*/
			
		}

		float winP = (float)w/N;
		float lossP = (float)l/N;
		//float pushP = (float)pushes/N;
		int predictions = pW+pL;
		float predictionP = (float)predictions/(float)(w+l);
		float cWP = (float)pW/(float)(w);
		float cLP = (float)pL/(float)(l);

		cout << "win percent : " << winP << endl;
		cout << "loss percent : " << lossP << endl;
		//cout << "push percent : " << pushP << endl;
		//cout << wins << " " << losses << " " << predictions << endl;
		cout << "prediction percent : " << predictionP << endl;
		cout << "predicted wins : " << cWP << endl;
		cout << "predicted loss : " << cLP << endl;
	}


	/*
		checks if they are the same except for the final entry
		(sort of) assumes they're the same size
	*/
	bool sameExceptLast(vector<float> one, vector<float> two){
		for(int ii = 0; ii < one.size()-1 || ii < two.size()-1; ii++){
			if(one[ii] != two[ii]) return false;
		}
		return true;
	}

	/*
		Sort and average a list using the above function to sort
	*/
	void sortAndAverage(){
		cout << "sorting and averaging..." << endl;
		cout << "size before average : " << data.size() << endl;
		//sorting is easy
		sort(data.begin(), data.end());

		//averaging is hard
		vector<vector<float>> retlist = vector<vector<float>>();

		for(int ii = 0; ii < data.size(); ii++){
			vector<float> temp = data[ii];
			float sum = temp[temp.size()-1];
			int count = 1; 
			while(ii+1 < data.size() && sameExceptLast(data[ii+1], temp) ){
				sum += data[ii+1][temp.size()-1];
				count++;
				ii++;
			}
			temp[temp.size()-1] = sum/count;
			retlist.push_back(temp);
		}
		data = retlist;

		cout << "size after average : " << data.size() << endl;
	}

	/*
		Generate data about the count using a "stay on 16" strategy
	*/
	void dataGen(int N){

		FANN::neural_net net;
		net.create_from_file("simpleTest.net");

		vector<float> singleItem = vector<float>();
		for(int ii = 0; ii < N; ii++){			//running N trials

				singleItem = vector<float>();
				float arr[10];	//ANN

				for(int qq = 1; qq < game.perfectCount.size(); qq++){
					singleItem.push_back(game.perfectCount[qq]);
					arr[qq-1] = game.perfectCount[qq];
				}

				float* prediction = net.run(arr);	//ANN
				int p = (*prediction > .7) ? 1 : 0;	//ANN

				while(!game.playerBustedP() && game.playerTotal < 16){ //stay on 16
					game.playerHit();
				}
				if(game.playerBustedP()){
					singleItem.push_back(0);
					losses++;
					if(!p){ correctPredictions++; }//correctPList.push_back(*prediction); }	//ANN
					//else	incorrectPList.push_back(*prediction);
				}else{
					singleItem.push_back(1);
					wins++;
					if(p) { correctPredictions++; }//correctPList.push_back(*prediction); }	//ANN
					//else	incorrectPList.push_back(*prediction);
				}

				data.push_back(singleItem);

				game.resetTable();		//delete all the cards
				game.tryShuffle();		//game will shuffle if it wants
		}

		trials += N;

		sortAndAverage();
	}


	void averageWinLossProbabilities(){

		vector<float> newWin = winData[0];
		cout << "A" << endl;

		for(int ii = 1; ii < winData.size(); ii++){
			for(int kk = 0; kk < winData[ii].size(); kk++){
				newWin[kk] += winData[ii][kk];
			}
		}
		for(int ii = 0; ii < newWin.size(); ii++){
			newWin[ii] /= winData.size();
		}

		vector<float> newLoss = lossData[0];

		cout << "B" << endl;
		for(int ii = 1; ii < lossData.size(); ii++){
			for(int kk = 0; kk < lossData[ii].size(); kk++){
				newLoss[kk] += lossData[ii][kk];
			}
		}
		for(int ii = 0; ii < newLoss.size(); ii++){
			newLoss[ii] /= lossData.size();
		}
		
		cout << "C" << endl;
		for(int ii = 1; ii < 11; ii++){
			cout << newWin[ii] << ",";
		}
		cout << endl;
		for(int ii = 1; ii < 11; ii++){
			cout << newLoss[ii] << ",";
		}
		cout << endl;
		
	}


	void dumpWinLossProbabilities(){
		cout << "dumping win/loss data" << endl;
		ofstream datafile;
		datafile.open("winnerProbabilities");
		for(int ii = 0; ii < winData.size(); ii++){
			for(int kk = 1; kk < winData[ii].size()-1; kk++){
				datafile << kk << "," << winData[ii][kk] << endl;
			}
		}
		datafile.close();
		cout << "wins completed" << endl;
		datafile.open("loserProbabilities");
		for(int ii = 0; ii < lossData.size(); ii++){
			for(int kk = 1; kk < lossData[ii].size()-1; kk++){
				datafile << kk << "," << lossData[ii][kk] << endl;
			}
		}
		datafile.close();
		cout << "done!" << endl;
	}



	void printReport(){

		float winP = wins/(float)trials;
		float lossP = losses/(float)trials;
		float pushP = pushes/(float)trials;

		cout << "Total played: " << trials << endl;
		cout << "Raw Numbers-------------" << endl;
		cout << "Wins        : " << wins << endl;
		cout << "Losses      : " << losses << endl;
		cout << "Pushes      : " << pushes << endl;
		cout << "Percantages-------------" << endl;
		cout << "Wins        : " << winP << endl;
		cout << "Losses      : " << lossP << endl;
		cout << "Pushes      : " << pushP << endl;


	}

	void printPredictions(){
		cout << "Correct Predictions: " << correctPredictions << endl;
		cout << "Percent Correct    : " << correctPredictions/(float)trials << endl;
	}

	void dumpPredictionData(){


		sort(correctPList.begin(), correctPList.end());

		sort(incorrectPList.begin(), incorrectPList.end());

		ofstream datafile;
		datafile.open("winner");

		for(int ii = 0; ii < correctPList.size(); ii++){
			datafile << correctPList[ii]  << endl;
		}
		datafile.close();
		datafile.open("loser");
		for(int ii = 0; ii < incorrectPList.size(); ii++){
			datafile << incorrectPList[ii]  << endl;
		}
		datafile.close();
	}

	void dumpBankrollHistory(){
		ofstream datafile;
		datafile.open("bankrollHistory");

		for(int ii = 0; ii < bankrollHistory.size(); ii++){
			datafile << bankrollHistory[ii]  << endl;
		}
		datafile.close();
	}



	void dumpToFile(){
		ofstream datafile;
		datafile.open("datafile");

		datafile << data.size() << " 11 1" << endl << endl;

		for(int ii = 0; ii < data.size(); ii++){
			for(int kk = 0; kk < data[ii].size()-1; kk++){
				datafile << data[ii][kk] << " ";
			}
			datafile << endl << endl << data[ii][data[ii].size()-1] << endl << endl;
		}
	}


};

#endif

