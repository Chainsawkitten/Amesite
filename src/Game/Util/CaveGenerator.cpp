#include "CaveGenerator.hpp"
#include <random>
#include <Util/Log.hpp>
#include <vector>
#include <queue>
#include <algorithm>
#include <glm/glm.hpp>

namespace CaveGenerator {

    CaveMap::CaveMap(int rowCount, int columnCount, int seed) {
        
        Log() << "Cave Generator seed: " << seed << "\n";

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
        mRNG.seed(static_cast<uint32_t>(seed));
    }

    CaveMap::~CaveMap() {
        for (int j = 0; j < mRowCount; ++j)
            delete[] mMap[j];
        delete[] mMap;
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

    void CaveMap::PrintMapToLog(bool** map, int rowCount, int columnCount) {
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

    void CaveMap::ProcessCaveMap(const int& iterations) {
        std::vector<Coordinate> test = GetLine(Coordinate(0, 0), Coordinate(5, 5));

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
                    if (j == 0 || j == (mRowCount - 1))
                        tempMap[j][k] = true;
                    else if (k == 0 || k == (mColumnCount - 1))
                        tempMap[j][k] = true;
                }
            }

            for (int j = 0; j < mRowCount; ++j) {
                for (int k = 0; k < mColumnCount; ++k) {
                    //Do not iterate over the border columns and rows.
                    if ((j != 0 && j != (mRowCount - 1)) && (k != 0 && k != (mColumnCount - 1))) {
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
                        if (((j + 1) < mRowCount) && ((k - 1) >= 0))
                            if (mMap[j + 1][k - 1])
                                numberOfWallNeighbours++;

                        //Middle-left
                        if ((j - 1) >= 0)
                            if (mMap[j - 1][k - 0])
                                numberOfWallNeighbours++;

                        //Middle-right
                        if ((j + 1) < mRowCount)
                            if (mMap[j + 1][k - 0])
                                numberOfWallNeighbours++;

                        //Bottom-left
                        if (((j - 1) >= 0) && ((k + 1) < mColumnCount))
                            if (mMap[j - 1][k + 1])
                                numberOfWallNeighbours++;

                        //Bottom-middle
                        if (((k + 1) < mColumnCount))
                            if (mMap[j - 0][k + 1])
                                numberOfWallNeighbours++;

                        //Bottom-right
                        if (((j + 1) < mRowCount) && ((k + 1) < mColumnCount))
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
        
        //Find which rooms we have left after processing.
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

        //Floodfill in order to find rooms.
        for (int i = 0; i < mRowCount; i++) {
            for (int j = 0; j < mColumnCount; j++) {
                if (flagMap[i][j] == false && mMap[i][j] == false) {
                    Coordinate coordinate = Coordinate(i, j);
                    std::vector<Coordinate> room = FloodFill(coordinate);
                    for (auto& coordinate : room)
                        flagMap[coordinate.x][coordinate.y] = true;
                    rooms.push_back(room);
                }
            }
        }

        for (int j = 0; j < mRowCount; ++j)
            delete[] flagMap[j];
        delete[] flagMap;

        for (auto& room : rooms) {
            mRoomList.push_back(Room(mMap, room));
        }

        std::size_t largestRoomSize = 0;
        Room* largestRoom = nullptr;
        for (auto& room : mRoomList) {
            if (room.coordinates.size() > largestRoomSize) {
                largestRoomSize = int(room.coordinates.size());
                largestRoom = &room;
            }
        }
        largestRoom->isMainRoom = true;
        largestRoom->isAccessibleFromMainRoom = true;
    }

    void CaveMap::FillCoordinates(const std::vector<Coordinate>& coordinates, bool tileType) {
        for (auto &i : coordinates) {
            if (IsWithinMapRange(i))
                mMap[i.x][i.y] = tileType;
        }
    }

    void CaveMap::RemoveSmallRooms( int threshold) {
        bool** map = mMap;

        for (auto& room : mRoomList) {
            if (static_cast<int>(room.coordinates.size()) < threshold)
                FillCoordinates(room.coordinates, true);
        }

        mRoomList.erase(std::remove_if(mRoomList.begin(), mRoomList.end(),
            [&threshold, map](Room i) {
                return (static_cast<int>(i.coordinates.size()) < threshold);
        }), mRoomList.end());

        return;
    }

    std::vector<Coordinate> CaveMap::FloodFill(const Coordinate& coordinate) {
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
        bool tileType = mMap[coordinate.x][coordinate.y];

        //Stores the coordinates that make out the region.
        std::vector<Coordinate> coordinates;

        //Stores the tiles we will be looking at.
        std::queue<Coordinate> tileQueue;

        //Add the first coordinate to the queue to start the process.
        Coordinate currentCoordinate(coordinate.x, coordinate.y);
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
                    if (IsWithinMapRange(Coordinate(i, j)) && (i == currentCoordinate.x || j == currentCoordinate.y)) {
                        //If the neighboring tile is of the type we are looking for and we haven't looked at it, look at it.
                        if (flagMap[i][j] == false && mMap[i][j] == tileType) {
                            flagMap[i][j] = true;
                            tileQueue.push(Coordinate(i, j));
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
        return ((coordinate.x > 0) && (coordinate.x < mRowCount-1) && (coordinate.y > 0) && (coordinate.y < mColumnCount-1));
    }

    bool ** CaveMap::GetMap(){
        return mMap;
    }

    void CaveMap::GenerateCaveMap(unsigned int percent) {
        std::uniform_int_distribution<uint32_t> hundredDistribution(1, 100);

        //Create array.
        if (mMap == nullptr) {
            mMap = new bool*[mRowCount];
            for (int i = 0; i < mRowCount; ++i)
                mMap[i] = new bool[mColumnCount];
        } else {
            for (int j = 0; j < mRowCount; ++j)
                delete[] mMap[j];
            delete[] mMap;

            mMap = new bool*[mRowCount];
            for (int i = 0; i < mRowCount; ++i)
                mMap[i] = new bool[mColumnCount];
        }

        //Initialize array.
        for (int i = 0; i < mRowCount; ++i) {
            for (int j = 0; j < mColumnCount; j++)
                mMap[i][j] = false;
        }

        //Seed walls with percent% probability.
        for (int i = 0; i < mRowCount; ++i) {
            for (int j = 0; j < mColumnCount; ++j) {
                if (i == 0 || i == (mRowCount - 1))
                    mMap[i][j] = true;
                else if (j == 0 || j == (mColumnCount - 1))
                    mMap[i][j] = true;
                else if (hundredDistribution(mRNG) < percent)
                    mMap[i][j] = true;

            }
        }
        return;
    }

    void CaveMap::ConnectClosestRooms(bool forceAccessibilityFromMainRoom) {
        std::vector<Room*> roomListA;
        std::vector<Room*> roomListB;

        //If we are forcing accessibility from the main room, then we should divide the rooms that are connected to the main room and those which are not into separate lists.
        if (forceAccessibilityFromMainRoom) {
            for (auto& room : mRoomList) {
                if (room.isAccessibleFromMainRoom) {
                    roomListB.push_back(&room);
                } else {
                    roomListA.push_back(&room);
                }
            }
        } else {
            for (auto& room : mRoomList) {
                roomListA.push_back(&room);
                roomListB.push_back(&room);
            }
        }

        int bestDistance = 0;
        bool possibleConnection = false;

        Coordinate* firstBestCoordinate = nullptr;
        Coordinate* secondBestCoordinate = nullptr;
        Room* firstBestRoom = nullptr;
        Room* secondBestRoom = nullptr;
        
        for (auto& firstRoom : roomListA) {
            if (!forceAccessibilityFromMainRoom) {
                possibleConnection = false;
                if (firstRoom->connectedRooms.size() > 0) {
                    continue;
                }
            }

            for (auto& secondRoom : roomListB) {
                //Abort early if possible
                if (*firstRoom == *secondRoom || firstRoom->IsConnected(*secondRoom)) {
                    continue;
                }

                //Do magic stuff, watch rooms connect!
                //Look at both rooms edge-tiles and connect the two closest tiles.
                for (auto& coordinateA : firstRoom->edgeCoordinates) {
                    for (auto& coordinateB : secondRoom->edgeCoordinates) {
                        int xDifference = (coordinateA.x - coordinateB.x);
                        int yDifference = (coordinateA.y - coordinateB.y);
                        int distance = static_cast<float>(glm::pow<float>(static_cast<float>(xDifference), 2.f) + glm::pow<float>(static_cast<float>(yDifference), 2.f));

                        if ((distance < bestDistance) || !possibleConnection) {
                            bestDistance = distance;
                            possibleConnection = true;
                            firstBestCoordinate = &coordinateA;
                            secondBestCoordinate = &coordinateB;
                            firstBestRoom = firstRoom;
                            secondBestRoom = secondRoom;
                        }
                    }
                }

            }
            if (possibleConnection && !forceAccessibilityFromMainRoom) {
                CreatePassage(*firstBestRoom, *secondBestRoom, *firstBestCoordinate, *secondBestCoordinate);
            }
        }

        if (possibleConnection && forceAccessibilityFromMainRoom) {
            CreatePassage(*firstBestRoom, *secondBestRoom, *firstBestCoordinate, *secondBestCoordinate);
            ConnectClosestRooms(true);
        }

        if (!forceAccessibilityFromMainRoom) {
            ConnectClosestRooms(true);
        }

        return;
    }

    void CaveMap::CreatePassage(Room& firstBestRoom, Room& secondBestRoom, Coordinate firstBestCoordinate, Coordinate secondBestCoordinate) {
        ConnectRooms(firstBestRoom, secondBestRoom);
        CreateTileCircledLine(firstBestCoordinate, secondBestCoordinate, 2, false);
    }

    std::vector<Coordinate> CaveMap::GetLine(Coordinate start, Coordinate end) {
        std::vector<Coordinate> LineCoordinates;
        int x = start.x;
        int y = start.y;

        int dx = end.x - start.x;
        int dy = end.y - start.y;

        bool inverted = false;

        int step = glm::sign(dx);
        int gradientStep = glm::sign(dy);

        int longest = abs(dx);
        int shortest = abs(dy);

        if (longest < shortest) {
            inverted = true;
            longest = abs(dy);
            shortest = abs(dx);

            step = glm::sign(dy);
            gradientStep = glm::sign(dx);
        }

        int gradientAccumulation = longest / 2;
        for (int i = 0; i < longest; i++) {
            LineCoordinates.push_back(Coordinate(x, y));

            if (inverted)
                y += step;
            else
                x += step;

            gradientAccumulation += shortest;
            if (gradientAccumulation >= longest) {
                if (inverted)
                    x += gradientStep;
                else
                    y += gradientStep;
                gradientAccumulation -= longest;
            }
        }
        return LineCoordinates;
    }

    void CaveMap::CreateTileLine(Coordinate start, Coordinate end, bool tileType) {
        std::vector<Coordinate> lineCoordinates = GetLine(start, end);
        FillCoordinates(lineCoordinates, tileType);
    }

    void CaveMap::CreateCircle(Coordinate origin, int radius, bool tileType) {
        std::vector<Coordinate> circleCoordinates = GetCircle(origin, radius);
        FillCoordinates(circleCoordinates, tileType);
    }

    void CaveMap::CreateTileCircledLine(Coordinate start, Coordinate end, int radius, bool tileType) {
        std::vector<Coordinate> lineCoordinates = GetLine(start, end);
        for (auto& coordinate : lineCoordinates) {
            std::vector<Coordinate> circleCoordinates = GetCircle(coordinate, radius);
            FillCoordinates(circleCoordinates, false);
        }
    }

    std::vector<Coordinate> CaveMap::GetCircle(Coordinate origin, int radius) {
        std::vector<Coordinate> circleCoordinates;

        for (int x = -radius; x <= radius; x++) {
            for (int y = -radius; y <= radius; y++) {
                if (x*x + y*y <= radius*radius) {
                    Coordinate circlePoint(origin.x + x, origin.y + y);
                    circleCoordinates.push_back(circlePoint);
                }
            }
        }
        return circleCoordinates;
    }

    std::vector<Coordinate> CaveMap::GetRectangle(int origin, int width, int height) {
        std::vector<Coordinate> rectangleCoordinates;
        //TODO!
        return rectangleCoordinates;
    }

    void CaveMap::ConnectRooms(Room& first, Room& second) {
        
        if (first.isAccessibleFromMainRoom) {
            second.SetAccessibleFromMainRoom();
        } else if (second.isAccessibleFromMainRoom) {
            first.SetAccessibleFromMainRoom();
        }

        first.connectedRooms.push_back(second);
        second.connectedRooms.push_back(first);
    }

    bool Room::IsConnected(Room& otherRoom) {
        for (auto& room : connectedRooms) {
            if (otherRoom == room)
                return true;
        }
        return false;
    }

    Room::Room(bool** map, std::vector<Coordinate> coordinates) {
        this->isAccessibleFromMainRoom = false;
        this->isMainRoom = false;
        this->coordinates = coordinates;

        for (auto& coordinate : coordinates){
            for (int i = (coordinate.x - 1); i <= (coordinate.x + 1); i++) {
                for (int j = (coordinate.y - 1); j <= (coordinate.y + 1); j++) {
                    //If the coordinate has a wall in any adjacent space, then it is an edge coordinate.
                    if (i == coordinate.x || j == coordinate.y) {
                        if (map[i][j] == true)
                            edgeCoordinates.push_back(coordinate);
                    }
                }
            }
        }
    }

    void Room::SetAccessibleFromMainRoom() {
        if (!isAccessibleFromMainRoom) {
            isAccessibleFromMainRoom = true;
            for (auto& room : connectedRooms) {
                room.SetAccessibleFromMainRoom();
            }
        }
    }

    Room::Room() {

    }

    Coordinate::Coordinate() {

    }
}
