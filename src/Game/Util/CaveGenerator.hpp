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
			
			Coordinate();

			friend bool operator==(const Coordinate& rhs, const Coordinate& lhs);

			inline bool operator==(const Coordinate& rhs) {
                return (this->x == rhs.x && this->y == rhs.x);
            };
    };

    class Room {
        public:
			Room(bool** map, std::vector<Coordinate> coordinates);
			Room();

			friend bool operator==(const Room& rhs, const Room& lhs);

			inline bool operator==(const Room& rhs) {
				return (this->coordinates == rhs.coordinates);
			};

			bool IsConnected(Room& otherRoom);

			void SetAccessibleFromMainRoom();

			bool isAccessibleFromMainRoom;
			bool isMainRoom;
            std::vector<Coordinate> coordinates;
			std::vector<Room> connectedRooms;
			std::vector<Coordinate> edgeCoordinates;
    };

    class CaveMap{
        public:
            CaveMap::CaveMap(int rowCount, int columnCount, int seed);
			CaveMap::~CaveMap();
            void GenerateCaveMap(int& percent);

			void ProcessCaveMap(const int& iterations);

            void PrintMapToLog();

            void FillCoordinates(const std::vector<Coordinate>& coordinates);

            void RemoveSmallRooms(int threshold);

            std::vector<Coordinate> FloodFill(const int& startX, const int& startY);

            bool IsWithinMapRange(const Coordinate & coordinate);

            bool** GetMap();

			void ConnectClosestRooms(bool forceAccessibilityFromMainRoom = false);
        private:
            
            void DetectRooms();
			
			void CreatePassage(Room& firstBestRoom, Room& secondBestRoom, Coordinate firstBestCoordinate, Coordinate secondBestCoordinate);

			std::vector<Coordinate> CaveMap::GetLine(Coordinate start, Coordinate end);

			void ConnectRooms(Room& first, Room& second);

            bool** mMap;
            int mRowCount; 
            int mColumnCount;
            std::mt19937 mRNG;
			std::vector<Room> mRoomList;
    };

}