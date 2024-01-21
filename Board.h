
#ifndef Board_h
#define Board_h

#include "Side.h"
#include <vector>

class Board{
public:
    Board(int nHoles, int nInitialBeansPerHole);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
private:
    int m_holes;
    int m_iholes;
    std::vector <int> m_North;
    std::vector <int> m_South;
    
};



#endif /* Board_h */
