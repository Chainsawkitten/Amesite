#pragma once
#include "../Entity/Entity.cpp"

namespace Caves{
    class CaveSystem {
        public:
            void GenerateCaveSystem();

        private:
            Entity mWalls[100];
        
            bool map[10][10] = {
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
                { 1, 0, 1, 1, 0, 0, 0, 0, 0, 1 },
                { 1, 0, 1, 0, 0, 1, 1, 1, 0, 1 },
                { 1, 0, 0, 0, 0, 1, 0, 1, 0, 1 },
                { 1, 1, 1, 1, 0, 1, 0, 0, 0, 1 },
                { 1, 0, 0, 1, 0, 1, 1, 1, 0, 1 },
                { 1, 0, 0, 1, 0, 0, 0, 0, 1, 1 },
                { 1, 0, 0, 1, 1, 1, 1, 0, 1, 1 },
                { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
                { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
            };
    };
}