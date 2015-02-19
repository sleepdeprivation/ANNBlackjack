# ANNBlackjack
Training neural nets to play blackjack and experimenting with different strategies

Requires libfann, get it with
  sudo apt-get install libfann-dev
  
  First, the blackjack simulator (generates data)

There is an underlying deck class that you may recognize from my javascript repository "grandma game".
The next layer up is the game of blackjack, I mean a class that helps the next layer play blackjack.

The highest layer is bjcontroller3. Yes, this is the 3rd iteration. Iterating on it hasn't helped though,
it still looks like garbage. This is where I did experiments and generated data. Unlike the other classes,
I actually worked out of this one and frequently changed it.

And of course the true highest layer is starter. It contains what used to live in the bjcontroller3 and simply
serves to run the experiments.

  Second, the ANN trainer
  
This is where I created and trained nets on the data I was generating. You can make it with the file maketrainer




There is a makefile for making the data generating side (requires libfann) and there is also a makefile for trainup.cpp,
which is a simple modification of an example file I found for creating and training nets.

Eventually I was so exasperated with my nets (which could never net win... get it... ? ) that I resorted to
probabilities. I made what amounts to a 1D summed area table to let me find any range of cards. It was just as (I mean almost exactly) successful as my best net.
