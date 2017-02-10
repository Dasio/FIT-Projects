#include "CA.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
#include <chrono>
#include "bitmap_image.hpp"

CA::CA(int msize,double r_rate) : msize(msize),r_rate(r_rate),step(0)
{
    if(msize < 3)
        msize = 3;
    if(r_rate < 0.0 || r_rate > 1.0)
        r_rate = 0.05;
    firstCells.resize(msize, std::vector<Cell>(msize,Cell::HEALTHY_R));
    secondCells.resize(msize, std::vector<Cell>(msize,Cell::HEALTHY_R));
    actualCells = &firstCells;
    nextCells = &secondCells;
    cell_lifespan = 48;
    infect_r_lifespan = 6;
    infect_s_lifespan = 4;
    inverse_rate1 = 0.001;
    inverse_rate2 = 0.001;
    infect_rate_r = 0.5;
    infect_rate_s = 0.6;
    replenishment = 0.99;
    infection = 0.001;
    hbv = 0.05;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0,1.0);
}
bool CA::checkProbability(double chance)
{
    assert(chance >= 0.0 && chance <= 1.0);
    return distribution(generator) <= chance;
}
bool CA::checkCellLife(Cell::State state, int actualLength)
{
    switch(state)
    {
        case Cell::HEALTHY_R:
        case Cell::HEALTHY_S:
        case Cell::DEF_INFECTED:
            return actualLength < cell_lifespan;
        case Cell::INFECTED_R:
            return actualLength < infect_r_lifespan;
        case Cell::INFECTED_S:
            return actualLength < infect_s_lifespan;
        default:
            return true;
            break;
    }
    return true;
}
void CA::generateLattice()
{
    Cell::State cell_state;
    //int lifeTime;
    std::map<Cell::State,int> map;
    std::uniform_int_distribution<int> randLifeTime(0,cell_lifespan);
   // std::uniform_int_distribution<int> randR(0,infect_r_lifespan);
   // std::uniform_int_distribution<int> randS(0,infect_s_lifespan);
    for(int i = 0; i<msize; i++)
        for(int j = 0; j<msize; j++)
        {
            // Generate resistant cell
            if(checkProbability(r_rate))
            {
                // Propability HBV for infected cell
                if(checkProbability(hbv))
                    cell_state = Cell::INFECTED_R;
                else
                    cell_state = Cell::HEALTHY_R;
            }
            // Generate susceptible cell
            else
            {
                if(checkProbability(hbv))
                    cell_state = Cell::INFECTED_S;
                else
                    cell_state = Cell::HEALTHY_S;
            }
           /* switch(cell_state)
            {
                case Cell::HEALTHY_R:
                case Cell::HEALTHY_S:
                    lifeTime = randLifeTime(generator);
                    break;
                case Cell::INFECTED_S:
                    lifeTime = randS(generator);
                    break;
                case Cell::INFECTED_R:
                    lifeTime = randR(generator);
                    break;
                case Cell::DEF_INFECTED:
                case Cell::DEAD:
                    lifeTime = 0;
                    break;
            }*/
            map[cell_state]++;
            firstCells[i][j].state = cell_state;
            firstCells[i][j].lifeTime = randLifeTime(generator);
           // firstCells[i][j] = Cell(cell_state,lifeTime);
        }
    ratioRS = (double)(map[Cell::HEALTHY_R]+map[Cell::INFECTED_R])/(map[Cell::HEALTHY_S]+map[Cell::INFECTED_S]);
    statistics.push_back(map);

}
struct CA::infectedCells CA::countInfectedNeighbors(int x, int y)
{
    assert(x>=0 && x<msize);
    assert(y>=0 && y<msize);
    int a,b;
    int infR = 0;
    int infS = 0;
    for(int i=x-1; i<=x+1; i++)
    {
        for(int j=y-1; j<=y+1; j++)
        {
            // Count only neighbors, skip cell itself
            if(i == x && j == y)
                continue;

            // Mapping besides boundaries
            if(i == -1)
                a = msize-1;
            else if(i == msize)
                a = 0;
            else
                a = i;
            if(j == -1)
                b = msize-1;
            else if(j == msize)
                b = 0;
            else
                b = j;

            // Counting infected cells
            switch((*actualCells)[a][b].state)
            {
                case Cell::INFECTED_S:
                    infS++;
                    break;
                case Cell::INFECTED_R:
                    infR++;
                    break;
                default:
                    break;
            }
        }
    }
    return infectedCells(infR,infS);
}
void CA::gen(int i, int j)
{
    Cell *cell = &((*actualCells)[i][j]);
    Cell *nextCell = &((*nextCells)[i][j]);
    // Gel statistic from previous step
    std::map<Cell::State,int> &map = statistics[step-1];
    double ratio,s,r;
    // Increase length of the life
    cell->lifeTime++;
    // First check lifespan
    if(cell->state != Cell::DEAD && !checkCellLife(cell->state,cell->lifeTime))
    {
        counter[cell->state]--;
        nextCell->state = Cell::DEAD;
        nextCell->lifeTime = 0;
        return;
        //return Cell(Cell::DEAD);
    }
    // Rules
    switch(cell->state)
    {
        case Cell::HEALTHY_R:
            //Rule 4b
           /* if(cell->revived && checkProbability(infection))
            {
                counter[Cell::HEALTHY_R]--;
                counter[Cell::INFECTED_R]++;
                nextCell->state = Cell::INFECTED_R;
                nextCell->revived = false;
                nextCell->lifeTime = 0;
                return;
                //return Cell(Cell::INFECTED_R);
            }*/
            // Rule 1c
            // Compute ratio used to maintain original ratio between R and S
            r = map[Cell::HEALTHY_R] + map[Cell::INFECTED_R] + counter[Cell::HEALTHY_R] + counter[Cell::INFECTED_R];
            s = map[Cell::HEALTHY_S] + map[Cell::INFECTED_S] + counter[Cell::HEALTHY_S] + counter[Cell::INFECTED_S];
            ratio = r/s;
            if(ratio  > ratioRS)
            {
                counter[Cell::HEALTHY_R]--;
                counter[Cell::HEALTHY_S]++;
                nextCell->state = Cell::HEALTHY_S;
                nextCell->lifeTime = cell->lifeTime;
                return;
               // return Cell(Cell::HEALTHY_S);
            }

        case Cell::HEALTHY_S:
            {
                // Rule 1b
                struct CA::infectedCells infected = countInfectedNeighbors(i,j);
                double prob = (infect_rate_r * (infected.R/8.0)) + (infect_rate_s * (infected.S/8.0));

                if(checkProbability(prob))
                {
                    if(cell->state == Cell::HEALTHY_R)
                    {
                        counter[Cell::HEALTHY_R]--;
                        counter[Cell::INFECTED_R]++;
                    }
                    else
                    {
                        counter[Cell::HEALTHY_S]--;
                        counter[Cell::INFECTED_S]++;
                    }
                    // HEALTHY_R to INFECTED_R and HEALTHY_S to INFECTED_S
                    nextCell->state = Cell::State(cell->state + 2);
                    nextCell->lifeTime = 0;
                    return;
                    //return Cell(Cell::State(cell->state + 2));
                }
            }
            break;
        case Cell::INFECTED_R:
        case Cell::INFECTED_S:
            // Rule 2b
            if(checkProbability(inverse_rate1))
            {
                if(cell->state == Cell::INFECTED_R)
                {
                    counter[Cell::HEALTHY_R]++;
                    counter[Cell::INFECTED_R]--;
                }
                else
                {
                    counter[Cell::HEALTHY_S]++;
                    counter[Cell::INFECTED_S]--;
                }
                // INFEFCTED_R to HEALTHY_R and INFECTED_S to HEALTHY_S
                nextCell->state = Cell::State(cell->state - 2);
                nextCell->lifeTime = 0;
                return;
                //return Cell(Cell::State(cell->state - 2));
            }
            // Rule 2c
            if(checkProbability(inverse_rate2))
            {
                nextCell->state = Cell::DEF_INFECTED;
                nextCell->lifeTime = 0;
                return;
                //return Cell(Cell::DEF_INFECTED);
            }
            break;
        case Cell::DEAD:
            // Rule 4a
            if(checkProbability(replenishment))
            {
                if(checkProbability(infection))
                    nextCell->state = Cell::INFECTED_R;
                // Ruke 4b
                else
                    nextCell->state = Cell::HEALTHY_R;
                counter[nextCell->state]++;
                nextCell->lifeTime = 0;
                return;
                //return Cell(Cell::HEALTHY_R,0);
            }
            break;
        default:
            break;
    }

    nextCell->state = cell->state;
    nextCell->lifeTime = cell->lifeTime;
    //return Cell(cell->state,cell->lifeTime);
}
void CA::nextStep()
{
    std::map<Cell::State,int> map;
    step++;

    // Used for counting additonal cells
    counter.clear();

    for(int i = 0; i<msize; i++)
    {
        for(int j=0; j<msize; j++)
        {
            //std::cout << (*actualCells)[i][j].state << Cell *cell = &((*actualCells)[i][j]);" ";
           // (*nextCells)[i][j] = gen(i,j);
            gen(i,j);
            map[(*nextCells)[i][j].state]++;
        }
       //std::cout << std::endl;
    }
    std::swap(actualCells,nextCells);
    statistics.push_back(map);
}
void CA::generateBMP(std::string fileName)
{
   bitmap_image image(msize,msize);

   for (int x = 0; x < msize; ++x)
   {
      for (int y = 0; y < msize; ++y)
      {
        switch((*actualCells)[x][y].state)
        {
            case Cell::HEALTHY_R:
                image.set_pixel(x,y,255,255,255);
                break;
            case Cell::HEALTHY_S:
                image.set_pixel(x,y,255,255,255);
                break;
            case Cell::INFECTED_R:
                image.set_pixel(x,y,255,0,0);
                break;
            case Cell::INFECTED_S:
                image.set_pixel(x,y,255,0,0);
                break;
            case Cell::DEF_INFECTED:
                image.set_pixel(x,y,255,0,0);
                break;
            case Cell::DEAD:
                image.set_pixel(x,y,0,0,255);
                break;
            default:
                image.set_pixel(x,y,255,255,255);
                break;
        }
      }
   }
   image.save_image(fileName.c_str());
}

void CA::printActualStatistic()
{
    std::map<Cell::State,int> &map = statistics[step];
    std::cout << "Healthy = " << map[Cell::HEALTHY_R] + map[Cell::HEALTHY_S]
    << " Infected = " << map[Cell::INFECTED_R] + map[Cell::INFECTED_S] + map[Cell::DEF_INFECTED]
    << " Dead = " << map[Cell::DEAD] << std::endl;
}

void CA::saveData(std::string fileName)
{
    std::ofstream file;
    file.open(fileName);
    file << "HEALTHY_R HEALTHY_S INFECTED_R INFECTED_S DEF_INFECTED DEAD\n";
    for(auto stat: statistics)
    {
        file << stat[Cell::HEALTHY_R] << " "
            << stat[Cell::HEALTHY_S] << " "
            << stat[Cell::INFECTED_R] << " "
            << stat[Cell::INFECTED_S] << " "
            << stat[Cell::DEF_INFECTED] << " "
            << stat[Cell::DEAD] << std::endl;
    }
    file.close();
}
