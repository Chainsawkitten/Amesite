#include "CaveGenerator.hpp"
#include <random>
#include <Util/Log.hpp>
#include <vector>
#include <queue>
#include <algorithm>

using namespace CaveGenerator;

bool** CaveGenerator::GenerateCaveMap(const int& rowCount, const int& columnCount, const int& iterations) {
    //Initialize Mersenne Twister RNG
    std::mt19937 RNG;

    //TODO: Actually seed the RNG with a proper seed.
    uint32_t seed = 0;
    RNG.seed(seed);
    std::uniform_int_distribution<uint32_t> hundredDistribution(1,100);

    //Create array.
    bool** map = new bool*[rowCount];
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

    std::vector<std::vector<Coordinate>> rooms = DetectRooms(map, rowCount, columnCount);

    RemoveSmallRooms(map, rooms, 40);

    Log() << "\n";

    PrintMapToLog(map, rowCount, columnCount);

    Log() << int(rooms.size());

    return map;
}


void CaveGenerator::PrintMapToLog(bool** map, const int& rowCount, const int& columnCount) {
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; ++j) {
            if (map[i][j] == true)
                Log() << "#";
            else if (map[i][j] == false)
                Log() << ".";
            else
                Log() << "o";
        }
        Log() << "\n";
    }
}

void CaveGenerator::ProcessCaveMap(bool ** map, const int& rowCount, const int& columnCount, const int& iterations) {
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

std::vector<std::vector<Coordinate>> CaveGenerator::DetectRooms(bool ** map, const int & rowCount, const int & columnCount){
    std::vector<std::vector<Coordinate>> rooms;

    //Create array to keep track of which tiles we have looked at.
    bool** flagMap = new bool*[rowCount];
    for (int i = 0; i < rowCount; ++i)
        flagMap[i] = new bool[columnCount];

    //Initialize array.
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; j++)
            flagMap[i][j] = false;
    }

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < columnCount; j++) {
            if (flagMap[i][j] == false && map[i][j] == false) {
                std::vector<Coordinate> room = FloodFill(map, i, j, rowCount, columnCount);
                for (auto& coordinate : room)
                    flagMap[coordinate.x][coordinate.y] = true;
                rooms.push_back(room);
            }
        }
    }

    for (int j = 0; j < rowCount; ++j)
        delete[] flagMap[j];
    delete[] flagMap;

    return rooms;
}

void CaveGenerator::FillCoordinates(bool ** map, const std::vector<Coordinate>& coordinates) {
    for (auto &i : coordinates)
        map[i.x][i.y] = true;
}

void CaveGenerator::RemoveSmallRooms(bool ** map, std::vector<std::vector<Coordinate>>& rooms, int threshold) {
    rooms.erase(std::remove_if(rooms.begin(), rooms.end(), 
        [&threshold, map](std::vector<Coordinate> i) {
        if (static_cast<int>(i.size()) < threshold) {
            FillCoordinates(map, i);
            return true;
        } else {
            return false;
        }
    }), rooms.end());

    return;
}


std::vector<Coordinate> CaveGenerator::FloodFill(bool** map, const int& startX, const int& startY, const int& rowCount, const int& columnCount) {
    //Create array to keep track of which tiles we have looked at.
    bool** flagMap = new bool*[rowCount];
    for (int i = 0; i < rowCount; ++i)
        flagMap[i] = new bool[columnCount];

    //Initialize array.
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; j++)
            flagMap[i][j] = false;
    }

    //Stores wheter we are looking for floors or walls
    bool tileType = map[startX][startY];

    //Stores the coordinates that make out the region.
    std::vector<Coordinate> coordinates;

    //Stores the tiles we will be looking at.
    std::queue<Coordinate> tileQueue;

    //Add the first coordinate to the queue to start the process.
    Coordinate currentCoordinate(startX, startY);
    tileQueue.push(currentCoordinate);

    while (!tileQueue.empty()) {
        //Set currentCoordinate to the first element
        currentCoordinate = tileQueue.front();
        //Remove the coordinate from the queue
        tileQueue.pop();
        //Add the coordinate to our vector
        coordinates.push_back(currentCoordinate);
        //Add surrounding four non-diagonal tiles to the tileQueue
        for (int i = (currentCoordinate.x - 1); i <= (currentCoordinate.x + 1); i++) {
            for (int j = (currentCoordinate.y - 1); j <= (currentCoordinate.y + 1); j++) {
                //Check if the neighbour coordinate is within the map, and check if it is neighbouring LR/UD
                if (IsWithinMapRange(map, Coordinate(i,j), rowCount, rowCount) && (i == currentCoordinate.x || j == currentCoordinate.y)) {
                    //If the neighboring tile is of the type we are looking for and we haven't looked at it, look at it.
                    if (flagMap[i][j] == false && map[i][j] == tileType) {
                        flagMap[i][j] = true;
                        tileQueue.push(Coordinate(i,j));
                    }
                }
            }
        }
    }

    for (int j = 0; j < rowCount; ++j)
        delete[] flagMap[j];
    delete[] flagMap;

    return coordinates;
}

bool CaveGenerator::IsWithinMapRange(bool ** map, const Coordinate & coordinate, const int& rowCount, const int& columnCount) {
    return ((coordinate.x > 0) && (coordinate.x < columnCount) && (coordinate.y > 0) && (coordinate.y < rowCount));
}