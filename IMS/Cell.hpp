#ifndef CELL_HPP
#define CELL_HPP

class Cell
{
public:
    /*
     R - Resistant S - Susceptible
    */
    enum State
    {
        // Dont change order!
        HEALTHY_R,
        HEALTHY_S,
        INFECTED_R,
        INFECTED_S,
        DEF_INFECTED,
        DEAD
    };
    Cell() { }
    Cell(State state, int lifeTime = 0);
    State state;
    int lifeTime;


};

#endif
