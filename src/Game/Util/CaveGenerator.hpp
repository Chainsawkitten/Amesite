#pragma once
#include <vector>

///TODO: COMMENT THESE
namespace CaveGenerator {
    struct Coordinate {
        int x;
        int y;
        Coordinate(int X, int Y) { 
            x = X; 
            y = Y; 
        };
        inline bool operator==(const Coordinate& rhs) {
            return (this->x == rhs.x && this->y == rhs.x);
        };
    };

    bool** GenerateCaveMap(const int& rowCount, const int& columnCount, const int& iterations);

    void PrintMapToLog(bool** map, const int& rowCount, const int& columnCount);

    void ProcessCaveMap(bool** map, const int& rowCount, const int& columnCount, const int& iterations);

    std::vector<std::vector<Coordinate>> DetectRooms(bool** map, const int& rowCount, const int& columnCount);

    void FillCoordinates(bool** map, const std::vector<Coordinate>& coordinates);

    void RemoveSmallRooms(bool** map, std::vector<std::vector<Coordinate>>& rooms, int threshold);

    std::vector<Coordinate> FloodFill(bool** map, const int& startX, const int& startY, const int& rowCount, const int& columnCount );

    bool IsWithinMapRange(bool** map, const Coordinate & coordinate, const int& rowCount, const int& columnCount);
}