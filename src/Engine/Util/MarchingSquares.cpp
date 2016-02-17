#include "MarchingSquares.hpp"
#include "../Util/Log.hpp"

using namespace MarchingSquares;

std::vector<glm::vec2> MarchingSquares::marchingSquares(const bool ** data, const float squareSize)
{
    int rows = sizeof(data) / sizeof(data[0]);
    int columns = sizeof(data[0]) / sizeof(data[0][0]);

    float mapWidth = columns * squareSize;
    float mapHeight = rows * squareSize;

    ControlNode** controlNodes = new ControlNode*[columns];
    MSquare** mSquares = new MSquare*[columns-1];

    for (int x = 0; x < columns; x++) {
        controlNodes[x] = new ControlNode[rows];
        for (int y = 0; y < rows; y++) {
            glm::vec3 pos = glm::vec3(-mapWidth / 2.f + x * squareSize + squareSize / 2.f, 0.f, -mapHeight / 2.f + y * squareSize + squareSize / 2.f);
            controlNodes[x][y] = createControlNode(pos, data[x][y], squareSize);
        }
    }

    for (int i = 0; i < columns - 1; i++) {
        mSquares[i] = new MSquare[rows - 1];
        for (int j = 0; j < rows - 1; j++) {
            mSquares[i][j] = createMSquare(controlNodes[i][j + 1], controlNodes[i + 1][j + 1], controlNodes[i + 1][j], controlNodes[i][j]);
        }
    }


}

MSquare MarchingSquares::createMSquare(ControlNode topLeft, ControlNode topRight, ControlNode bottomRight, ControlNode bottomLeft)
{
    MSquare square;

    square.topLeft = topLeft;
    square.topRight = topRight;
    square.bottomRight = bottomRight;
    square.bottomLeft = bottomLeft;

    square.centerTop = topLeft.right;
    square.centerRight = bottomRight.above;
    square.centerBottom = bottomLeft.right;
    square.centerLeft = bottomLeft.above;

    if (topLeft.active)
        square.type += 8;
    if (topRight.active)
        square.type += 4;
    if (bottomRight.active)
        square.type += 2;
    if (bottomLeft.active)
        square.type += 1;

    return square;
}

ControlNode MarchingSquares::createControlNode(const glm::vec3 position, const bool active, const float squareSize)
{
    ControlNode node;
    node.active = active;
    node.above = createMeshNode(position + glm::vec3(0.f, 0.f, 1.f)* squareSize / 2.f);
    node.right = createMeshNode(position + glm::vec3(1.f, 0.f, 0.f)* squareSize / 2.f);
}

MeshNode MarchingSquares::createMeshNode(const glm::vec3 position)
{
    MeshNode node;
    node.position = position;
    return node;
}

glm::vec2 MarchingSquares::getTriangle(MSquare square)
{
    switch (square.type) {
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
