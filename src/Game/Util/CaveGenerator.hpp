#pragma once
#include <vector>
#include <random>

///TODO: COMMENT THESE
namespace CaveGenerator {

    class Coordinate {
        public:
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

    class Room {
        public:
            std::vector<Coordinate> coordinates;
            std::vector<int> connected;
    };

    class CaveMap{
        public:
            CaveMap::CaveMap(int rowCount, int columnCount, int seed);

            void GenerateCaveMap(int& percent, int& iterations);

            void PrintMapToLog();

            void FillCoordinates(const std::vector<Coordinate>& coordinates);

            void RemoveSmallRooms(std::vector<std::vector<Coordinate>>& rooms, int threshold);

            std::vector<Coordinate> FloodFill(const int& startX, const int& startY);

            bool IsWithinMapRange(const Coordinate & coordinate);

            bool** GetMap();

        private:
            
            void DetectRooms();
            void ProcessCaveMap(const int& iterations);

            bool** mMap;
            int mRowCount; 
            int mColumnCount;
            std::mt19937 mRNG;
            std::vector<std::vector<Coordinate>> mRooms;
    };

}