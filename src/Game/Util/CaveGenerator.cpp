#include "CaveGenerator.hpp"
#include <random>
#include <Util/Log.hpp>
#include <vector>

bool** GenerateCaveMap(const int& rowCount, const int& columnCount, const int& iterations) {
    //Initialize Mersenne Twister RNG
    std::mt19937 RNG;

    //TODO: Actually seed the RNG with a proper seed.
    uint32_t seed = 0;
    RNG.seed(seed);
    std::uniform_int_distribution<uint32_t> hundredDistribution(1,100);

    bool** map = new bool*[rowCount];
    
    //Create array.
    for (int i = 0; i < rowCount; ++i)
        map[i] = new bool[columnCount];

    //Initialize array.
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; j++)
            map[i][j] = false;
    }
    //PrintMapToLog(map, rowCount, columnCount);

    //Seed walls with 45% probability.
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; ++j) {
            if (i == 0 || i == (columnCount - 1))
                map[i][j] = true;
            else if(j == 0 || j == (rowCount - 1))
                map[i][j] = true;
            else if ( hundredDistribution(RNG) < 50)
                map[i][j] = true;
            
        }
    }

    ProcessCaveMap(map, rowCount, columnCount, iterations);

    PrintMapToLog(map, rowCount, columnCount);

    return map;
}


void PrintMapToLog(bool** map, const int& rowCount, const int& columnCount) {
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; ++j) {
            if (map[i][j] == true)
                Log() << "#";
            else if (map[i][j] == false)
                Log() << ".";
            else
                Log() << "o";
            if (j == (columnCount - 1))
                Log() << "\n";
        }
    }
}

void ProcessCaveMap(bool ** map, const int& rowCount, const int& columnCount, const int& iterations) {
    //Iterate over n times over map.
    //Rule: if a cell was a wall and has 4 or more wall neighbours, then it stays a wall.
    //Otherwise, if the cell wasn't a wall and there are 5 or more wall neighbours, then it becomes a wall.
    for (int i = 0; i < iterations; i++) {

        //Create temporary map.
        bool** tempMap = new bool*[rowCount];
        for (int j = 0; j < rowCount; ++j)
            tempMap[j] = new bool[columnCount];

        //Initialize temporary map.
        for (int j = 0; j < rowCount; ++j) {
            for (int k = 0; k < columnCount; ++k)
                tempMap[j][k] = false;
        }

        //Add borders
        for (int j = 0; j < rowCount; ++j) {
            for (int k = 0; k < columnCount; ++k) {
                if (j == 0 || j == (columnCount - 1))
                    tempMap[j][k] = true;
                else if (k == 0 || k == (rowCount - 1))
                    tempMap[j][k] = true;
            }
        }
        //PrintMapToLog(tempMap, rowCount, columnCount);

        for (int j = 0; j < rowCount; ++j) {
            for (int k = 0; k < columnCount; ++k) {
                //Do not iterate over the border columns and rows.
                if ((j != 0 && j != (columnCount - 1)) && (k != 0 && k != (rowCount - 1))) {
                    //Check number of wall neighbours.
                    int numberOfWallNeighbours = 0;

                    //Check 3x3 grid for walls.
                    //Top-left
                    if (((j - 1) >= 0) && ((k - 1) >= 0))
                        if (map[j - 1][k - 1])
                            numberOfWallNeighbours++;

                    //Top-middle
                    if ((k - 1) >= 0)
                        if (map[j - 0][k - 1])
                            numberOfWallNeighbours++;

                    //Top-right
                    if (((j + 1) < columnCount) && ((k - 1) >= 0))
                        if (map[j + 1][k - 1])
                            numberOfWallNeighbours++;

                    //Middle-left
                    if ((j - 1) >= 0)
                        if (map[j - 1][k - 0])
                            numberOfWallNeighbours++;

                    //Middle-right
                    if ((j + 1) < columnCount)
                        if (map[j + 1][k - 0])
                            numberOfWallNeighbours++;

                    //Bottom-left
                    if (((j - 1) >= 0) && ((k + 1) < rowCount))
                        if (map[j - 1][k + 1])
                            numberOfWallNeighbours++;

                    //Bottom-middle
                    if (((k + 1) < rowCount))
                        if (map[j - 0][k + 1])
                            numberOfWallNeighbours++;

                    //Bottom-right
                    if (((j + 1) < columnCount) && ((k + 1) < rowCount))
                        if (map[j + 1][k + 1])
                            numberOfWallNeighbours++;
                    
                    //Here is the implementation of the cellular automata rule.
                    if ((map[j][k] && numberOfWallNeighbours >= 4) || (!map[j][k] && numberOfWallNeighbours >= 5))
                        tempMap[j][k] = true;
                    else
                        tempMap[j][k] = false;
                }

            }
        }

        //Copy results to map to prepare for another iteration.
        for (int j = 0; j < rowCount; ++j) {
            for (int k = 0; k < columnCount; ++k) {
                map[j][k] = tempMap[j][k];
            }
        }

        //Delete tempMap.
        for (int j = 0; j < rowCount; ++j)
            delete[] tempMap[j];
        delete[] tempMap;
    }
}
