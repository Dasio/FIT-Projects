#ifndef CA_HPP
#define CA_HPP

#include <map>
#include <string>
#include <vector>
#include <random>
#include <string>
#include "Cell.hpp"


class CA
{
using CellsMatrix = std::vector<std::vector<Cell>>;


public:
    CA(int msize = 1000,double r_rate=0.8);

    /**
     * Check probability
     * @param  chance range from 0.0 to 1.0
     * @return  True if probability occurred
     */
    bool checkProbability(double chance);

    /**
     * Generate cells to matrix
     */
    void generateLattice();

    /**
     * Based on rules generate cell for next iteration
     * @param  i      row
     * @param  j      col
     * @return        New cell for next iteration
     */
    void gen(int i, int j);

    /**
     * Check if cell shoul exist in next step
     * @param  state        State of the cell
     * @param  actualLength Length of the cell
     * @return              False if nextstep shoul be dead
     */
    bool checkCellLife(Cell::State state, int actualLength);

    /**
     * Execute next step of CA
     */
    void nextStep();

    /**
     * Generate bitmap of cells in lattice, healthy=white, dead=blue, infected=red
     * @param fileName
     */
    void generateBMP(std::string fileName);
    void printActualStatistic();

    /**
     * Save quantity of every type of cell in each step to file
     * @param fileName
     */
    void saveData(std::string fileName);

private:
    struct infectedCells
    {
        infectedCells() = default;
        infectedCells(int R,int S) : R(R), S(S) { }
        int R;
        int S;
    };
    struct infectedCells countInfectedNeighbors(int x, int y);
    int msize;
    int cell_lifespan;
    int infect_r_lifespan;
    int infect_s_lifespan;
    double infect_rate_r;
    double infect_rate_s;
    double inverse_rate1;
    double inverse_rate2;
    double replenishment;
    double infection;
    double ratioRS;
    double r_rate;
    double hbv;
    int step;

    // Used for counting healthy cells to maintain ratio between them
    std::map<Cell::State,int> counter;
    CellsMatrix *actualCells;
    CellsMatrix *nextCells;
    CellsMatrix firstCells;
    CellsMatrix secondCells;
    std::mt19937 generator;
    std::uniform_real_distribution<double> distribution;
    std::vector<std::map<Cell::State,int>> statistics;
};

#endif
