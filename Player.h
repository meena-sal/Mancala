
#ifndef Player_h
#define Player_h

#include "Board.h"
#include <string>
#include "Side.h"
#include <iostream>
#include <random>
#include <climits>


class Player{
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
private:
    std::string m_name;
};

class HumanPlayer: public Player
{
public:
    HumanPlayer(std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer: public Player{
public:
    BadPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
};

class SmartPlayer: public Player{
public:
    SmartPlayer(std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
private:
    int evaluate(const Board& b, Side s) const;
    void minimax(const Board& b, Side s, int depth, int& bestHole, int& value, JumpyTimer& timer, double timeLimit) const;
    

};
#endif /* Player_h */
