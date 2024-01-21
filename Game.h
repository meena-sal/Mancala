//
//  Game.h
//  CS32P3
//
//  Created by Meena Salib on 5/23/23.
//

#ifndef Game_h
#define Game_h

#include "Board.h"
#include "Player.h"
#include "Side.h"

class Game{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move(Side s);
    void play();
    int beans(Side s, int hole) const;
private:
    Player* m_south;
    Player* m_north;
    Board m_board;
    Side m_turn;
};



#endif /* Game_h */
