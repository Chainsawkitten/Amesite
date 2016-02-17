#include "Map.hpp"
#include "../Util/Log.hpp"

using namespace Geometry;

Map::Map(const bool **data, const float squareSize) {
    marchingSquares(data, squareSize);

    GenerateBuffers();
    GenerateVertexArray();
}

Map::~Map() {
    delete[] mVertexData;
    delete[] mIndexData;
}

Geometry3D::Vertex* Map::GetVertices() const {
    return mVertexData;
}

unsigned int Map::GetVertexCount() const {
    return mVertexNr;
}

unsigned int* Map::GetIndices() const {
    return mIndexData;
}

unsigned int Map::GetIndexCount() const {
    return mIndexNr;
}

void Map::marchingSquares(const bool ** data, const float squareSize)
{
    int rows = sizeof(data) / sizeof(data[0]);
    int columns = sizeof(data[0]) / sizeof(data[0][0]);

    mMapWidth = columns * squareSize;
    mMapHeight = rows * squareSize;

    ControlNode** controlNodes = new ControlNode*[columns];
    MSquare** mSquares = new MSquare*[columns - 1];

    for (int x = 0; x < columns; x++) {
        controlNodes[x] = new ControlNode[rows];
        for (int y = 0; y < rows; y++) {
            glm::vec3 pos = glm::vec3(-mMapWidth / 2.f + x * squareSize + squareSize / 2.f, 0.f, -mMapHeight / 2.f + y * squareSize + squareSize / 2.f);
            controlNodes[x][y] = createControlNode(pos, data[x][y], squareSize);
        }
    }

    for (int i = 0; i < columns - 1; i++) {
        mSquares[i] = new MSquare[rows - 1];
        for (int j = 0; j < rows - 1; j++) {
            mSquares[i][j] = createMSquare(controlNodes[i][j + 1], controlNodes[i + 1][j + 1], controlNodes[i + 1][j], controlNodes[i][j]);
        }
    }

    for (int k = 0; k < columns; k++) {
        for (int l = 0; l < rows; l++) {
            triangulateSquare(&mSquares[k][l]);
        }
    }
    mIndexData = &mTempIndexData[0];
    mVertexData = &mTempVertexData[0];

    for (int m = 0; m < columns-1; m++) {
        delete[] controlNodes[m];
        delete[] mSquares[m];
    }
    delete[] controlNodes[columns];
}

Map::MSquare Map::createMSquare(ControlNode topLeft, ControlNode topRight, ControlNode bottomRight, ControlNode bottomLeft)
{
    MSquare square;
    square.mTopLeft = topLeft;
    square.mTopRight = topRight;
    square.mBottomRight = bottomRight;
    square.mBottomLeft = bottomLeft;

    square.mCenterTop = topLeft.mRight;
    square.mCenterRight = bottomRight.mAbove;
    square.mCenterBottom = bottomLeft.mRight;
    square.mCenterLeft = bottomLeft.mAbove;

    if (topLeft.mActive)
        square.mType += 8;
    if (topRight.mActive)
        square.mType += 4;
    if (bottomRight.mActive)
        square.mType += 2;
    if (bottomLeft.mActive)
        square.mType += 1;

    return square;
}

Map::ControlNode Map::createControlNode(const glm::vec3 position, const bool active, const float squareSize)
{
    ControlNode node;

    node.mActive = active;
    node.mAbove = createMeshNode(position + glm::vec3(0.f, 0.f, 1.f)* squareSize / 2.f);
    node.mRight = createMeshNode(position + glm::vec3(1.f, 0.f, 0.f)* squareSize / 2.f);
    node.mPosition = position;

    return node;
}

Map::MeshNode Map::createMeshNode(const glm::vec3 position)
{
    MeshNode node;
    node.mPosition = position;

    return node;
}

void Map::createMesh(MeshNode* node)
{
    int nodeSize = sizeof(node);
    for (int i = 0; i < nodeSize; i++) {
        if (node[i].mVertexIndex == -1) {
            node[i].mVertexIndex = mVertexNr;
            Vertex vertex;
            vertex.position = node[i].mPosition;
            vertex.normal = glm::vec3(0.f, 1.f, 0.f);
            vertex.tangent = glm::vec3(0.0f, 0.0f, -1.0f);
            vertex.textureCoordinate = glm::vec2(0.f, 0.f);
            mTempVertexData.push_back(vertex);
            mVertexNr++;
        }
    }

    if (nodeSize >= 3)
        storeTriangle(node[0], node[2], node[1]);
    if (nodeSize >= 4)
        storeTriangle(node[0], node[3], node[2]);
    if (nodeSize >= 5)
        storeTriangle(node[0], node[4], node[3]);
    if (nodeSize >= 6)
        storeTriangle(node[0], node[5], node[4]);
}

void Map::triangulateSquare(MSquare* square)
{
    MeshNode* node;
    switch (square->mType) {
    case 0:
        break;

        // 1 points:
    case 1:
        createMesh(node = new MeshNode[3]{ square->mCenterBottom, square->mBottomLeft, square->mCenterLeft });
        break;
    case 2:
        createMesh(node = new MeshNode[3]{ square->mCenterRight, square->mBottomRight, square->mCenterBottom });
        break;
    case 4:
        createMesh(node = new MeshNode[3]{ square->mCenterTop, square->mTopRight, square->mCenterRight });
        break;
    case 8:
        createMesh(node = new MeshNode[3]{ square->mTopLeft, square->mCenterTop, square->mCenterLeft });
        break;

        // 2 points:
    case 3:
        createMesh(node = new MeshNode[4]{ square->mCenterRight, square->mBottomRight, square->mBottomLeft, square->mCenterLeft });
        break;
    case 6:
        createMesh(node = new MeshNode[4]{ square->mCenterTop, square->mTopRight, square->mBottomRight, square->mCenterBottom });
        break;
    case 9:
        createMesh(node = new MeshNode[4]{ square->mTopLeft, square->mCenterTop, square->mCenterBottom, square->mBottomLeft });
        break;
    case 12:
        createMesh(node = new MeshNode[4]{ square->mTopLeft, square->mTopRight, square->mCenterRight, square->mCenterLeft });
        break;
    case 5:
        createMesh(node = new MeshNode[6]{ square->mCenterTop, square->mTopRight, square->mCenterRight, square->mCenterBottom, square->mBottomLeft, square->mCenterLeft });
        break;
    case 10:
        createMesh(node = new MeshNode[6]{ square->mTopLeft, square->mCenterTop, square->mCenterRight, square->mBottomRight, square->mCenterBottom, square->mCenterLeft });
        break;

        // 3 point:
    case 7:
        createMesh(node = new MeshNode[5]{ square->mCenterTop, square->mTopRight, square->mBottomRight, square->mBottomLeft, square->mCenterLeft });
        break;
    case 11:
        createMesh(node = new MeshNode[5]{ square->mTopLeft, square->mCenterTop, square->mCenterRight, square->mBottomRight, square->mBottomLeft });
        break;
    case 13:
        createMesh(node = new MeshNode[5]{ square->mTopLeft, square->mTopRight, square->mCenterRight, square->mCenterBottom,square->mBottomLeft });
        break;
    case 14:
        createMesh(node = new MeshNode[5]{ square->mTopLeft, square->mTopRight, square->mBottomRight, square->mCenterBottom, square->mCenterLeft });
        break;

        // 4 point:
    case 15:
        createMesh(node = new MeshNode[4]{ square->mTopLeft, square->mTopRight, square->mBottomRight, square->mBottomLeft });
        break;
    }
    delete node;
}

void Geometry::Map::storeTriangle(MeshNode a, MeshNode b, MeshNode c)
{
    mTempIndexData.push_back(a.mVertexIndex);
    mTempIndexData.push_back(b.mVertexIndex);
    mTempIndexData.push_back(c.mVertexIndex);
    mIndexNr += 3;
}
