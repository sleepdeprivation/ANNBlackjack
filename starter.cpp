#include "deck.cpp"
#include "blackjack.cpp"
#include "bjcontroller3.cpp"
using namespace std;

int main (void){
	//Deck d = Deck();
	//d.print();
	BJController b = BJController();
	//b.asdf(500000, .51, .65, .1);

	//b.playNProbabilityBlackjack(500000, .4, .53);
	//b.dumpBankrollHistory();



	b.playNProbabilityBlackjack(1000000, .4, .53);
	//b.averageWinLossProbabilities();
/*
	b.playNProbabilityBlackjack(500000, .4, .53);
	b.playNProbabilityBlackjack(500000, .4, .532);

	b.playNProbabilityBlackjack(500000, .4, .534);
	b.playNProbabilityBlackjack(500000, .4, .536);
/*
	b.playNProbabilityBlackjack(500000, .4, .56);
	b.playNProbabilityBlackjack(500000, .4, .57);
	b.playNProbabilityBlackjack(500000, .4, .58);
	b.playNProbabilityBlackjack(500000, .4, .59);
	b.playNProbabilityBlackjack(500000, .4, .60);
	b.playNProbabilityBlackjack(500000, .4, .61);
	b.playNProbabilityBlackjack(500000, .4, .62);
	b.playNProbabilityBlackjack(500000, .4, .63);
	b.playNProbabilityBlackjack(500000, .4, .64);
	b.playNProbabilityBlackjack(500000, .4, .65);


	//b.generateProbabilities();
	//b.displayAllProbabilities();
	/*
	for(float ii = .2; ii < 1; ii+=.1){
		cout << ii << endl;
		b.playNProbabilityBlackjack(500000, ii);
	}
	*/
	//b.dataGen(10000);
	//b.printReport();
	//b.printPredictions();
	//b.dumpPredictionData();
	//b.sortAndAverage();
	//b.dumpToFile();



	//b.playBlackjackWithHuman();
	//b.playNThresholdRounds(1000000, 16);
	//b.playNConfidanceANNRounds(1000000, .5, 1);
	return 0;
}

