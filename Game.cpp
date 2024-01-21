//
//  Game.cpp
//  CS32P3
//
//  Created by Meena Salib on 5/23/23.
//

#include <stdio.h>
#include "Game.h"
using namespace std;

Game::Game(const Board& b, Player* south, Player* north): m_board(b){
    m_south = south;
    m_north = north;
    m_turn = SOUTH;
}

void Game::display() const{
    cout << "          " << m_north->name() << endl;
    cout << "    ";
    // prints North's holes
    for (int i = 1; i <= m_board.holes(); i++){
        cout << m_board.beans(NORTH, i);
        cout << "   ";
    }
    cout << endl;
    
    cout << m_board.beans(NORTH, 0);
    
    
// ensures correct spacing for South's pot no matter the size of the board
    for (int i = 0; i <= m_board.holes(); i++){
        cout << "    ";
    }
    
    cout << m_board.beans(SOUTH, 0) << endl;
    
    cout << "    ";
    // prints South's holes
    for (int i = 1; i <= m_board.holes(); i++){
        cout << m_board.beans(SOUTH, i);
        cout << "   ";
    }
    
    cout << endl;
    
    
    cout << "          " << m_south->name() << endl;
    
    
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const{
    // counter for how many holes in South have no beans
    int SNBC = 0;
    // counter for how many holes in North have no beans
    int NNBC = 0;
    for (int i = 1; i <= m_board.holes(); i++){
        if (m_board.beans(SOUTH, i) == 0){
            SNBC++;
        }
    }
    for (int i = 1; i <= m_board.holes(); i++){
        if (m_board.beans(NORTH, i) == 0){
            NNBC++;
        }
    }
    
    
    // if it's north's turn but they don't have any beans, they can't make a turn and the game is over
    if (m_turn == NORTH && NNBC == m_board.holes()){
        over = true;
        // Game is over and North has more beans in pot, so North wins
        if (m_board.beans(NORTH, 0) > m_board.beans(SOUTH, 0)){
            hasWinner = true;
            winner = NORTH;
        // Game is over and South has more beans in pot, so South wins
        } else if (m_board.beans(NORTH, 0) < m_board.beans(SOUTH, 0)) {
            hasWinner = true;
            winner = SOUTH;
        // Game is over but both pots are equal, so it's a tie
        } else {
            hasWinner = false;
        }
    // If it's South's turn but they don't have any beans, they can't make a turn and the game is over
    } else if (m_turn == SOUTH && SNBC == m_board.holes()) {
        over = true;
        // Game is over and North has more beans in pot, so North wins
        if (m_board.beans(NORTH, 0) > m_board.beans(SOUTH, 0)){
            hasWinner = true;
            winner = NORTH;
        // Game is over and South has more beans in pot, so South wins
        } else if (m_board.beans(NORTH, 0) < m_board.beans(SOUTH, 0)) {
            hasWinner = true;
            winner = SOUTH;
        // Game is over but both pots are equal, so it's a tie
        } else {
            hasWinner = false;
        }
    // game is still going
    }else {
        over = false;
        hasWinner = false;
    }
    
}

bool Game::move(Side s){
    
    // counts holes on a side that have no beans
    int NBC = 0;
    // copy of board is created to keep track of what the board looked like before sowing to determine captures
    Board copy = m_board;
    int hole;
    Side endSide;
    int endHole;
    
    // if South is attempting a move
    if (s == SOUTH){
        for (int i = 1; i <= m_board.holes(); i++){
            if (m_board.beans(SOUTH, i) == 0){
                NBC++;
            }
        }
        // if every hole on that side has no beans, no move can be made and game is over so sweep opponent's beans into their pot
        if (NBC == m_board.holes()){
            for (int i = 1; i <= m_board.holes(); i++){
                m_board.moveToPot(NORTH, i, NORTH);
                }
            m_turn = SOUTH;
            return false;
        }
        // prompts user for a move and stores that value, then sows starting with that hole
        hole = m_south->chooseMove(m_board, s);
        m_board.sow(s, hole, endSide, endHole);
        // if we end at our own pot, display the board to user and reprompt for another move for their additional turn
        if (endSide == s && endHole == 0){
            display();
            cout << "Additional turn!" << endl;
            m_turn = SOUTH;
            return move(SOUTH);
        // if we are in a capture situation, proceed with the capture
        } else if (endSide == s && copy.beans(SOUTH, endHole) == 0 && m_board.beans(NORTH, endHole) > 0){
            m_board.moveToPot(NORTH, endHole, SOUTH);
            m_board.moveToPot(SOUTH, endHole, SOUTH);
            m_turn = NORTH;
            return true;
        // if no additional turn or capture, then just end the turn there
        } else {
            m_turn = NORTH;
            return true;
        }
    // North is attempting a move
    } else {
        for (int i = 1; i <= m_board.holes(); i++){
            if (m_board.beans(NORTH, i) == 0){
                NBC++;
            }
        }
        // if every hole on that side has no beans, no move can be made, game is over so sweep beans
        if (NBC == m_board.holes()){
            for (int i = 1; i <= m_board.holes(); i++){
                m_board.moveToPot(SOUTH, i, SOUTH);
                }
            m_turn = NORTH;
            return false;
        }
        // prompts user for a move and stores that value, then sows starting with that hole
        hole = m_north->chooseMove(m_board, s);
        m_board.sow(s, hole, endSide, endHole);
        // if we end at our own pot, display the board to user and reprompt for another move for their additional turn
        if (endSide == s && endHole == 0){
            copy.sow(s, hole, endSide, endHole);
            display();
            cout << "Additional turn!" << endl;
            m_turn = NORTH;
            return move(NORTH);
        // if we are in a capture situation, proceed with the capture
        } else if (endSide == s && endHole != 0 && copy.beans(NORTH, endHole) == 0 && m_board.beans(SOUTH, endHole) > 0){
            m_board.moveToPot(NORTH, endHole, NORTH);
            m_board.moveToPot(SOUTH, endHole, NORTH);
            m_turn = SOUTH;
            return true;
        // if no additional turn or capture, then just end the turn there
        } else {
            m_turn = SOUTH;
            return true;
        }
    }
    
    
    
    
}

void Game::play(){
    display();
    string enter;
    bool over = false;
    bool hasWinner = false;
    Side winner;
    // loop while the game isn't over
    while (over == false){
        // if both players are bots, let both of them choose their moves then continue to play when the human user presses enter
        if (m_south->isInteractive() == false && m_north->isInteractive() == false){
            cout << "Press ENTER to continue: ";
            getline(cin, enter);
            cout << endl;
        }
        // play South's move then display the board
        move(SOUTH);
        display();
        // check if the game is over, end loop if it is
        status(over, hasWinner, winner);
        if (over == true){
            move(NORTH);
            display();
            status(over, hasWinner, winner);
            break;
        }
        // play North's move then display the board
        move(NORTH);
        display();
        // check if the game is over, end loop if it is
        status(over, hasWinner, winner);
        if (over == true){
            move(SOUTH);
            display();
            status(over, hasWinner, winner);
            break;
        }
    }
    // if we don't have a winner but the game is over, means it is a tie
    if (hasWinner == false){
        cout << "Game ended in a tie!" << endl;
    } else if (winner == NORTH){
        cout << "Game over! The winner is " << m_north->name() << endl;
    } else {
        cout << "Game over! The winner is " << m_south->name() << endl;
    }
    
}

int Game::beans(Side s, int hole) const{
    return m_board.beans(s, hole);
}
