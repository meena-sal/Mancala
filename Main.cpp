#include "Game.h"
    #include "Player.h"
    #include "Board.h"
    #include "Side.h"
    #include <iostream>
    #include <cassert>
    using namespace std;


    int main()
    {
        HumanPlayer h("Meena");
        SmartPlayer r("Andrew");
        Board b(6, 4);
        Game g(b, &r ,&h);
        g.play();
    }


