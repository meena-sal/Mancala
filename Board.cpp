
#include <stdio.h>
#include "Board.h"

Board::Board(int nHoles, int nInitialBeansPerHole){
    if (nHoles <= 0){
        m_holes = 1;
    } else {
        m_holes = nHoles;
    }
    
    if (nInitialBeansPerHole < 0){
        m_iholes = 0;
    } else {
        m_iholes = nInitialBeansPerHole;
    }
    
    // will use vector to hold the holes
    for (int i = 0; i < nHoles + 1; i++){
        m_North.push_back(nInitialBeansPerHole);
        m_South.push_back(nInitialBeansPerHole);
    }
    
    // set pot; for my convenience and understanding, I will have south's pot be the element at the end of the vector that way
    // it corresponds to the counterclockwise rotation of the game
    m_North.at(0) = 0;
    m_South.at(m_holes) = 0;
    
    
    
}

int Board::holes() const{
    return m_holes;
}

int Board::beans(Side s, int hole) const{
    if (hole > m_holes || hole < 0){
        return -1;
    } else if (s == 0){
        return m_North.at(hole);
    // if input hole is 0, it is referring to South's pot, which is at the end of the vector.
    } else if (s == 1 && hole == 0){
        return m_South.at(m_holes);;
    // because pot is at the end of the vector, the holes are indexed in the vector one less than their spot on the game board
    } else {
        return m_South.at(hole - 1);
    }
}

int Board::beansInPlay(Side s) const{
    int count = 0;
    
    if (s == 0){
        for (int i = 1; i < m_North.size(); i++){
            count += m_North.at(i);
        }
    } else {
        for (int i = 0; i < m_South.size() - 1; i++){
            count += m_South.at(i);
        }
    }
        
    return count;
}

int Board::totalBeans() const{
    int count = 0;
    
    for (int i = 0; i < m_North.size(); i++){
        count += m_North.at(i);
    }
        
    for (int i = 0; i < m_South.size(); i++){
        count += m_South.at(i);
    }
    
    return count;
}

bool Board::sow(Side s, int hole, Side& endSide, int& endHole){
    
    // if paramaters are invalid, return false and do nothing
    if (hole <= 0 || hole > m_holes || beans(s, hole) <= 0){
        return false;
    } else {
        // bean represents the beans that we need to sow across the board from the given hole
        int bean = beans(s, hole);
        // if sowing hole is North...
        if (s == 0){
            // set that hole to 0 because we took the beans out of it
            m_North.at(hole) = 0;
            // set the hole we're going to start sowing at as the hole to the left of the hole we took the beans from
            int start = hole - 1;
            // loop as long as there's still beans left to sow
            while (bean > 0){
                // sow North's side including our pot
                for (int i = start; i >= 0; i--){
                    m_North.at(i) += 1;
                    bean--;
                    // if we ran out of beans to sow at this hole, mark it as the end and stop everything
                    if (bean == 0){
                        endSide = NORTH;
                        endHole = i;
                        return true;
                    }
                }
                
                // sow through South's side but skip its pot
                for (int i = 0; i < m_South.size() - 1; i++){
                    m_South.at(i) += 1;
                    bean--;
                    if (bean == 0){
                        endSide = SOUTH;
                        endHole = i + 1;
                        // correctly convert to South's pot
                        if (endHole == holes() + 1){
                            endHole = 0;
                        }
                        return true;
                    }
                }
                // if we're back at North, start sowing at its last hole
                start = m_North.size() - 1;
                
            }
        // if sowing hole is South...
        } else {
            // take all the beans from that hole
            m_South.at(hole - 1) = 0;
            // start sowing at the whole to its right
            int start = hole;
            // while there's still beans to sow
            while (bean > 0){
                // sow and don't skip South's pot
                for (int i = start; i < m_South.size(); i++){
                    m_South.at(i) += 1;
                    bean--;
                    if (bean == 0){
                        endSide = SOUTH;
                        endHole = i + 1;
                        // correctly convert to South's pot
                        if (endHole == holes() + 1){
                            endHole = 0;
                        }
                        return true;
                    }
                }
                // sow but skip North's pot
                for (int i = m_North.size() - 1; i > 0; i--){
                    m_North.at(i) += 1;
                    bean--;
                    if (bean == 0){
                        endSide = NORTH;
                        endHole = i;
                        return true;
                    }
                }
                // if we're back at South, start sowing at South's first hole
                start = 0;
            }
        }
    }
    
    
    return true;
}

bool Board::moveToPot(Side s, int hole, Side potOwner){
    bool retValue = false;
    
    if (hole > m_holes || hole <= 0){
        ;
    } else {
        // save the number of beans at the hole we want to move to pot
       int temp = beans(s, hole);
        if (potOwner == SOUTH){
            // add beans from the hole we took from to the pot
            m_South.at(m_holes) += temp;
            // set hole we took from to 0
            if (s == 0){
                m_North.at(hole) = 0;
            } else {
                m_South.at(hole - 1) = 0;
            }
        } else {
            // add beans from the hole we took from to the pot
            m_North.at(0) += temp;
            // set hole we took from to 0
            if (s == 0){
                m_North.at(hole) = 0;
            } else {
                m_South.at(hole - 1) = 0;
            }
        }
    }
    return retValue;
}

bool Board::setBeans(Side s, int hole, int beans){
    bool retValue = false;
    
    if (hole < 0 || hole > m_holes || beans < 0){
        ;
    } else {
        retValue = true;
        if (s == 0){
            m_North.at(hole) = beans;
        } else if (s == 1 && hole == 0) {
            m_South.at(m_holes) = beans;
        } else {
            m_South.at(hole - 1) = beans;
        }
    }
    
    
    return retValue;
}
