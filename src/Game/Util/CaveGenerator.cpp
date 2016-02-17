#include "CaveGenerator.hpp"
#include <random>
#include <Util/Log.hpp>
#include <vector>
#include <queue>
#include <algorithm>

namespace CaveGenerator {

    CaveMap::CaveMap(int rowCount, int columnCount, int seed) {
        
        mRowCount = rowCount;
        mColumnCount = columnCount;

        //Create array.
        mMap = new bool*[mRowCount];
        for (int i = 0; i < mRowCount; ++i)
            mMap[i] = new bool[mColumnCount];

        //Initialize array.
        for (int i = 0; i < mRowCount; ++i) {
            for (int j = 0; j < mColumnCount; j++)
                mMap[i][j] = false;
        }

        //Initialize Mersenne Twister RNG
        //TODO: Actually seed the RNG with a proper seed.
        mRNG.seed(static_cast<uint32_t>(seed));
    }


    void CaveMap::PrintMapToLog() {
        for (int i = 0; i < mRowCount; ++i) {
            for (int j = 0; j < mColumnCount; ++j) {
                if (mMap[i][j] == true)
                    Log() << "#";
                else if (mMap[i][j] == false)
                    Log() << ".";
                else
                    Log() << "o";
            }
            Log() << "\n";
        }
    }

    void CaveMap::ProcessCaveMap(const int& iterations) {
        //Iterate over n times over map.
        //Rule: if a cell was a wall and has 4 or more wall neighbours, then it stays a wall.
        //Otherwise, if the cell wasn't a wall and there are 5 or more wall neighbours, then it becomes a wall.
        for (int i = 0; i < iterations; i++) {

            //Create temporary map.
            bool** tempMap = new bool*[mRowCount];
            for (int j = 0; j < mRowCount; ++j)
                tempMap[j] = new bool[mColumnCount];

            //Initialize temporary map.
            for (int j = 0; j < mRowCount; ++j) {
                for (int k = 0; k < mColumnCount; ++k)
                    tempMap[j][k] = false;
            }

            //Add borders
            for (int j = 0; j < mRowCount; ++j) {
                for (int k = 0; k < mColumnCount; ++k) {
                    if (j == 0 || j == (mColumnCount - 1))
                        tempMap[j][k] = true;
                    else if (k == 0 || k == (mRowCount - 1))
                        tempMap[j][k] = true;
                }
            }
            
            for (int j = 0; j < mRowCount; ++j) {
                for (int k = 0; k < mColumnCount; ++k) {
                    //Do not iterate over the border columns and rows.
                    if ((j != 0 && j != (mColumnCount - 1)) && (k != 0 && k != (mRowCount - 1))) {
                        //Check number of wall neighbours.
                        int numberOfWallNeighbours = 0;

                        //Check 3x3 grid for walls.
                        //Top-left
                        if (((j - 1) >= 0) && ((k - 1) >= 0))
                            if (mMap[j - 1][k - 1])
                                numberOfWallNeighbours++;

                        //Top-middle
                        if ((k - 1) >= 0)
                            if (mMap[j - 0][k - 1])
                                numberOfWallNeighbours++;

                        //Top-right
                        if (((j + 1) < mColumnCount) && ((k - 1) >= 0))
                            if (mMap[j + 1][k - 1])
                                numberOfWallNeighbours++;

                        //Middle-left
                        if ((j - 1) >= 0)
                            if (mMap[j - 1][k - 0])
                                numberOfWallNeighbours++;

                        //Middle-right
                        if ((j + 1) < mColumnCount)
                            if (mMap[j + 1][k - 0])
                                numberOfWallNeighbours++;

                        //Bottom-left
                        if (((j - 1) >= 0) && ((k + 1) < mRowCount))
                            if (mMap[j - 1][k + 1])
                                numberOfWallNeighbours++;

                        //Bottom-middle
                        if (((k + 1) < mRowCount))
                            if (mMap[j - 0][k + 1])
                                numberOfWallNeighbours++;

                        //Bottom-right
                        if (((j + 1) < mColumnCount) && ((k + 1) < mRowCount))
                            if (mMap[j + 1][k + 1])
                                numberOfWallNeighbours++;
                    
                        //Here is the implementation of the cellular automata rule.
                        if ((mMap[j][k] && numberOfWallNeighbours >= 4) || (!mMap[j][k] && numberOfWallNeighbours >= 5))
                            tempMap[j][k] = true;
                        else
                            tempMap[j][k] = false;
                    }

                }
            }

            //Copy results to map to prepare for another iteration.
            for (int j = 0; j < mRowCount; ++j) {
                for (int k = 0; k < mColumnCount; ++k) {
                    mMap[j][k] = tempMap[j][k];
                }
            }

            //Delete tempMap.
            for (int j = 0; j < mRowCount; ++j)
                delete[] tempMap[j];
            delete[] tempMap;
        }
    }

    void CaveMap::DetectRooms(){
        std::vector<std::vector<Coordinate>> rooms;

        //Create array to keep track of which tiles we have looked at.
        bool** flagMap = new bool*[mRowCount];
        for (int i = 0; i < mRowCount; ++i)
            flagMap[i] = new bool[mColumnCount];

        //Initialize array.
        for (int i = 0; i < mRowCount; ++i) {
            for (int j = 0; j < mColumnCount; j++)
                flagMap[i][j] = false;
        }

        for (int i = 0; i < mRowCount; i++) {
            for (int j = 0; j < mColumnCount; j++) {
                if (flagMap[i][j] == false && mMap[i][j] == false) {
                    std::vector<Coordinate> room = FloodFill(i, j);
                    for (auto& coordinate : room)
                        flagMap[coordinate.x][coordinate.y] = true;
                    rooms.push_back(room);
                }
            }
        }

        for (int j = 0; j < mRowCount; ++j)
            delete[] flagMap[j];
        delete[] flagMap;
        mRooms = rooms;
    }

    void CaveMap::FillCoordinates(const std::vector<Coordinate>& coordinates) {
        for (auto &i : coordinates)
            mMap[i.x][i.y] = true;
    }

    void CaveMap::RemoveSmallRooms(std::vector<std::vector<Coordinate>>& rooms, int threshold) {
        bool** map = mMap;

        for (auto& room : rooms) {
            if(room.size() < threshold)
            FillCoordinates(room);
        }

        rooms.erase(std::remove_if(rooms.begin(), rooms.end(), 
            [&threshold, map](std::vector<Coordinate> i) {
                return (static_cast<int>(i.size()) < threshold);
            }), rooms.end());

        return;
    }


    std::vector<Coordinate> CaveMap::FloodFill(const int& startX, const int& startY) {
        //Create array to keep track of which tiles we have looked at.
        bool** flagMap = new bool*[mRowCount];
        for (int i = 0; i < mRowCount; ++i)
            flagMap[i] = new bool[mColumnCount];

        //Initialize array.
        for (int i = 0; i < mRowCount; ++i) {
            for (int j = 0; j < mColumnCount; j++)
                flagMap[i][j] = false;
        }

        //Stores wheter we are looking for floors or walls
        bool tileType = mMap[startX][startY];

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
                    if (IsWithinMapRange(Coordinate(i,j)) && (i == currentCoordinate.x || j == currentCoordinate.y)) {
                        //If the neighboring tile is of the type we are looking for and we haven't looked at it, look at it.
                        if (flagMap[i][j] == false && mMap[i][j] == tileType) {
                            flagMap[i][j] = true;
                            tileQueue.push(Coordinate(i,j));
                        }
                    }
                }
            }
        }

        for (int j = 0; j < mRowCount; ++j)
            delete[] flagMap[j];
        delete[] flagMap;

        return coordinates;
    }

    bool CaveMap::IsWithinMapRange(const Coordinate & coordinate) {
        return ((coordinate.x > 0) && (coordinate.x < mColumnCount) && (coordinate.y > 0) && (coordinate.y < mRowCount));
    }

    bool ** CaveMap::GetMap(){
        return mMap;
    }

    void CaveMap::GenerateCaveMap(int& percent, int& iterations)
    {
        std::uniform_int_distribution<uint32_t> hundredDistribution(1, 100);

        //Create array.
        mMap = new bool*[mRowCount];
        for (int i = 0; i < mRowCount; ++i)
            mMap[i] = new bool[mColumnCount];

        //Initialize array.
        for (int i = 0; i < mRowCount; ++i) {
            for (int j = 0; j < mColumnCount; j++)
                mMap[i][j] = false;
        }

        PrintMapToLog();

        //Seed walls with percent% probability.
        for (int i = 0; i < mRowCount; ++i) {
            for (int j = 0; j < mColumnCount; ++j) {
                if (i == 0 || i == (mColumnCount - 1))
                    mMap[i][j] = true;
                else if (j == 0 || j == (mRowCount - 1))
                    mMap[i][j] = true;
                else if (hundredDistribution(mRNG) < percent)
                    mMap[i][j] = true;

            }
        }

        PrintMapToLog();

        ProcessCaveMap(iterations);

        PrintMapToLog();

        DetectRooms();

        return;
    }
}