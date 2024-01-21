

#include <stdio.h>
#include "Player.h"
#include "Board.h"


Player::Player(std::string name){
    m_name = name;
}

std::string Player::name() const{
    return m_name;
}

bool Player::isInteractive() const
{
    return false;
}

Player::~Player(){
    
}

HumanPlayer::HumanPlayer(std::string name): Player(name) {
    
};

bool HumanPlayer::isInteractive() const
{
    return true;
}

int HumanPlayer::chooseMove(const Board& b, Side s) const{
    int hole = 0;

// while hole is a pot, not a hole on the board, or there are no beans in the hole, reprompt the user
    while (hole < 1 || hole > b.holes() || b.beans(s, hole) <= 0){
        std::cout << "Choose a hole, " << name() << ": ";
        std::cin >> hole;
        std::cout << std::endl;
    }

    return hole;
}

BadPlayer::BadPlayer(std::string name): Player(name) {};



int BadPlayer::chooseMove(const Board& b, Side s) const{
    int hole = 0;

    // generates a random hole between hole 1 and the number of holes on a side
    hole = 1 + (rand() % b.holes());

    while (b.beans(s, hole) <= 0){
        hole = 1 + (rand() % b.holes());
    }
    return hole;
}

SmartPlayer::SmartPlayer(std::string name): Player(name){

}

int SmartPlayer::chooseMove(const Board& b, Side s) const{
    int depth = 10;
    int bestHole;
    int value;
    // set timeLimit and timer
    double timeLimit = 4900;
    JumpyTimer timer(1000);
    minimax(b, s, depth, bestHole, value, timer, timeLimit);
    return bestHole;
}

int SmartPlayer::evaluate(const Board& b, Side s) const{



    // only called in minimax when the game on the board is over
    //
    int value = (b.beansInPlay(SOUTH) + b.beans(SOUTH, 0)) - (b.beansInPlay(NORTH) + b.beans(NORTH, 0));
    // if South has more beans at the end, South won so best possible outcome
    if (value > 0){
        value = INT_MAX;
    // if South has less beans at the end, South lost so worst possible outcome
    } else if (value < 0){
        value = INT_MIN;
    // it's a tie, so meh outcome
    } else {
        value = 0;
    }
    return value;
}




void SmartPlayer::minimax(const Board& b, Side s, int depth, int& bestHole, int& value, JumpyTimer& timer, double timeLimit) const {
    int count = 1;
    // if game on the board is over, evaluate the position for the playing side
    if (b.beansInPlay(s) == 0){
        bestHole = -1;
        value = evaluate(b, s);
        return;
    }
    // once timer hits 0, end searching and just evaluate what we have so far
    if (timeLimit <= 0){
        value = b.beans(SOUTH, 0) - b.beans(NORTH, 0);
        bestHole = -1;
        return;
    }
    // don't want to search anymore branches but game is still going, so just evaluate the current position
    // evaluation is determined by difference in pots 
    if (depth <= 0){
        bestHole = -1;
        value = b.beans(SOUTH, 0) - b.beans(NORTH, 0);
        return;
    }
    // for every hole for a side
    for (int i = 1; i <= b.holes(); i++){
        double thisBranchTimeLimit = timeLimit / (b.holes() - i + 1);
        double startTime = timer.elapsed();
        // if there's no beans in that hole, test the next hole because invalid move
        if (b.beans(s, i) == 0){
            count++;
            continue;
        }
        // initialize variables to pass through functions
        int b2, b3;
        int v2;
        // make a copy so we don't have to unmake our move at the end
        Board copyPlayer = b;
        Side endSide;
        int endHole;
        if (copyPlayer.beans(s, i) != 0){
            // make the move
            copyPlayer.sow(s, i, endSide, endHole);
            // if in a capture situation, proceed with the capture
            if (endSide == s && endHole != 0 && b.beans(s, endHole) == 0 && copyPlayer.beans(opponent(s), endHole) > 0){
                copyPlayer.moveToPot(NORTH, endHole, s);
                copyPlayer.moveToPot(SOUTH, endHole, s);
            }
            // if we can make another move, determine possible outcomes for that move
            if (endSide == s && endHole == 0){
                minimax(copyPlayer, s, depth - 1, bestHole, v2, timer, thisBranchTimeLimit);
            }
        // determine possible outcomes from the opponent's side after we make our move
        minimax(copyPlayer, opponent(s), depth - 1, b2, v2, timer, thisBranchTimeLimit);
        } else {
        // if we can't make a move, determine other possible outcomes from our position
        minimax(copyPlayer, s, depth, b3, v2, timer, thisBranchTimeLimit);
        }
        // stores our first legal move to compare it to moves after
        if (i == count){
            bestHole = i;
            value = v2;
        }
        // if our side is North, the more favorable outcome would be if original value is greater than opponent value
        if (s == NORTH){
            if (value > v2){
                bestHole = i;
                value = v2;
            }
        // if our side is South, the more favorable outcome would be if the opponent's value is greater than ours
        } else {
            if (v2 > value){
                bestHole = i;
                value = v2;
            }
        }
        timeLimit -= (timer.elapsed() - startTime);
        if (timeLimit <= 0){
            timeLimit = 0;
        }
    }
    return;
    
}


