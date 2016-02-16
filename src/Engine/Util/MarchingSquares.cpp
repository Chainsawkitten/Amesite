#include "MarchingSquares.hpp"
#include "../Util/Log.hpp"

std::vector<glm::vec2> MarchingSquares::marchingSquares(const bool ** data)
{
    int rows = sizeof(data) / sizeof(data[0]);
    int columns = sizeof(data[0]) / sizeof(data[0][0]);
    std::vector<glm::vec2> lines;

    for (int i = 0; i < rows - 1; i++) {
        for (int j = 0; j < columns - 1; j++) {
            int collisionType = getType(glm::bvec4(data[i][j], data[i + 1][j], data[i][j + 1], data[i + 1][j + 1]));
            lines.push_back(getLine(glm::vec2(i, j), glm::vec2(i, j), glm::vec2(i, j), glm::vec2(i, j),collisionType));
        }
    }
}

int MarchingSquares::getType(const glm::bvec4& points)
{
    int type = 0;
    if (points[0]) type += 1; //Up, left
    if (points[1]) type += 2; //Up, right
    if (points[2]) type += 4; //Down, left
    if (points[3]) type += 8; //Down, right
    return type;
}

glm::vec2 MarchingSquares::getLine(const glm::vec2 points[4], const int type)
{
    switch (type) {
    case 0:
        //No line
        break;
    case 1:
        break;
    case 2:
        break;
    case 4:
        break;
    case 7:
        break;
    case 8:
        break;
    case 11:
        break;
    case 13:
        break;
    case 14:
        break;
    case 5:
        break;
    case 6:
        break;
    case 9:
        break;
    case 10:
        break;
    case 15:
        break;
    default:
        break;
    }
}
