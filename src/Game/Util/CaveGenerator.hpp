#pragma once
#include <vector>
#include <random>

namespace CaveGenerator {

    /// A 2D coordinate in a cave.
    class Coordinate {
        public:
            /// X coordinate.
            int x;
            
            /// Y coordinate.
            int y;

            /// Create new coordinate.
            /**
             * @param x X position.
             * @param y Y position.
             */
            Coordinate(int x, int y) {
                this->x = x;
                this->y = y;
            }
            
            /// Create new coordinate.
            Coordinate();

            /// Comparison operator.
            /**
             * @param rhs %Coordinate to compare to.
             * @return Whether the coordinates are equal.
             */
            inline bool operator==(const Coordinate& rhs) const {
                return (this->x == rhs.x && this->y == rhs.x);
            }
    };

    /// Describes a room in a cave.
    class Room {
        public:
            /// Create new room.
            /**
             * @param map The map, used for edge coordinate generation.
             * @param coordinates A list of coordinates that compose the room.
             */
            Room(bool** map, std::vector<Coordinate> coordinates);
            
            /// Create new room.
            Room();

            /// Comparison operator.
            /**
             * @param rhs %Room to compare to.
             * @return Whether the rooms have the same coordinates.
             */
            inline bool operator==(const Room& rhs) const {
                return (this->coordinates == rhs.coordinates);
            }

            /// Checks if a room is connected to another given room.
            /**
             * @param otherRoom The given room.
             * @return Whether the room is connected to the other room.
             */
            bool IsConnected(Room& otherRoom);

            /// Sets the room as accessible from main room.
            void SetAccessibleFromMainRoom();

            /// Is the room accessible from the main room?
            bool isAccessibleFromMainRoom;
            
            /// Is the room the main room?
            bool isMainRoom;

            /// The coordinates that compose the room.
            std::vector<Coordinate> coordinates;

            /// The rooms that this room is connected to.
            std::vector<Room> connectedRooms;

            /// A set of coordintes that make up the edges of the room.
            std::vector<Coordinate> edgeCoordinates;


    };

    /// Handles generating and manipulating a map.
    class CaveMap {
        public:
            /// Create new cave map.
            /**
             * @param rowCount the amount of rows in the map.
             * @param columnCount the amount of columns in the map.
             * @param seed Seed for the random generator.
             */
            CaveMap(int rowCount, int columnCount, int seed);

            /// Destructor
            ~CaveMap();

            /// Generates a cave map.
            /**
             * @param percent The initial percentage of walls we want in the map.
             */
            void GenerateCaveMap(int& percent);

            /// Detects rooms.
            void DetectRooms();

            /// Smooths and processes cave map.
            /**
             * @param iterations The number of iterations we do.
             */
            void ProcessCaveMap(const int& iterations);

            /// Prints the current state of the map to the log.
            void PrintMapToLog();

            ///Prints a boolmap to log.
            /**
             *@param map The bool map.
             *@param rowCount The number of rows.
             *@param columnCount The number of columns.
             */
            void PrintMapToLog(bool** map, int rowCount, int columnCount);

            /// Removes rooms smaller than a certain threshold.
            /**
             * @param threshold Rooms with less tiles than this will be removed.
             */
            void RemoveSmallRooms(int threshold);

            /// Checks if a given coordinate is within map range.
            /**
             * @param coordinate The coordinate we want to check.
             * @return Whether the coordinate is contained in the map.
             */
            bool IsWithinMapRange(const Coordinate& coordinate);

            /// Creates a circle of given tiletype.
            /**
             * @param origin The circles origin.
             * @param radius The circles radius.
             * @param tileType true if we are filling with walls, false otherwise.
             */
            void CreateCircle(Coordinate origin, int radius, bool tileType);


            /// Gets the 2D array of bools that represent the map.
            /**
             * @return The 2D array of bools that represent the map.
             */
            bool** GetMap();

            /// Connects all rooms in the map.
            /**
             * @param forceAccessibilityFromMainRoom If we are forcing connection to the main room or not.
             */
            void ConnectClosestRooms(bool forceAccessibilityFromMainRoom = false);

            /// Creates a single-tile line from a starting-point to an end-point.
            /**
             * @param start Starting coordinate.
             * @param end Ending coordinate.
             * @param tileType True for walls, false for floor
             */
            void CreateTileLine(Coordinate start, Coordinate end, bool tileType);

            /// Creates a line with a radius from a starting-point to an end-point.
            /**
             * @param start Starting coordinate.
             * @param end Ending coordinate.
             * @param radius The width of the line created.
             * @param tileType True for walls, false for floor
             */
            void CreateTileCircledLine(Coordinate start, Coordinate end, int radius, bool tileType);

        private:
            // Floodfills a region of the map and returns a vector of coordinates that were contained in that region.
            /*
             * @param coordinate Coordinate to start floodfilling from.
             * @return A list of coordinates that were found during the floodfilling process.
             */
            std::vector<Coordinate> FloodFill(const Coordinate& coordinate);
            
            // Fills a set of coordinates with a tile type.
            /*
             * @param coordinates The coordinates we will be filling.
             * @param tileType True if we are filling the coordinates with walls, false if we are filling with floors.
             */
            void FillCoordinates(const std::vector<Coordinate>& coordinates, bool tileType);
            
            // Creates a passage between rooms, used by ConnectClosestRooms.
            /*
             * @param firstBestRoom The first closest room.
             * @param secondBestRoom The second closest room.
             * @param firstBestCoordinate The first closest coordinate.
             * @param secondBesCoordinate The second closest coordinate.
             */
            void CreatePassage(Room& firstBestRoom, Room& secondBestRoom, Coordinate firstBestCoordinate, Coordinate secondBestCoordinate);
            
            // Gets a set of coordinates describing a rectangle.
            /*
             * @param origin The rectangle's origin.
             * @param width The rectangle's width.
             * @param height The rectangle's height.
             * @return A set of coordinates that are contained within the rectangle.
             */
            std::vector<Coordinate> GetRectangle(int origin, int width, int height);

            // Gets a set of coordinates describing a circle.
            /*
             * @param origin The circle's origin.
             * @param radius The circle's radius.
             * @return A set of coordinates that are contained within the circle.
             */
            std::vector<Coordinate> GetCircle(Coordinate origin, int radius);

            // Gets a set of coordinates describing a Line.
            /*
             * @param start The start coordinate.
             * @param end The end coordinate.
             * @return A set of coordinates that are contained within the circle.
             */
            std::vector<Coordinate> GetLine(Coordinate start, Coordinate end);

            // Connects two rooms.
            /*
             * @param first The first room.
             * @param second The second room.
             */
            void ConnectRooms(Room& first, Room& second);

            bool** mMap;
            int mRowCount; 
            int mColumnCount;
            std::mt19937 mRNG;
            std::vector<Room> mRoomList;
    };

}
