#include "Map.hpp"
#include "../Util/Log.hpp"

using namespace Geometry;

Map::Map(bool **data, const float squareSize, glm::uvec2 dataDimensions) {
    mDataDimensions = dataDimensions;
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

void Map::marchingSquares(bool ** data, const float squareSize)
{
    mMapWidth = mDataDimensions.x * squareSize;
    mMapHeight = mDataDimensions.y * squareSize;

    ControlNode** controlNodes = new ControlNode*[mDataDimensions.x];
    MSquare** mSquares = new MSquare*[mDataDimensions.x - 1];

    for (unsigned int x = 0; x < mDataDimensions.x; x++) {
        controlNodes[x] = new ControlNode[mDataDimensions.y];
        for (unsigned int y = 0; y < mDataDimensions.y; y++) {
            glm::vec3 pos = glm::vec3(-mMapWidth / 2.f + x * squareSize + squareSize / 2.f, 0.f, -mMapHeight / 2.f + y * squareSize + squareSize / 2.f);
            controlNodes[x][y] = createControlNode(pos, data[y][x], squareSize, glm::uvec2(x,y));
        }
    }

    for (unsigned int i = 0; i < mDataDimensions.x - 1; i++) {
        mSquares[i] = new MSquare[mDataDimensions.y - 1];
        for (unsigned int j = 0; j < mDataDimensions.y - 1; j++) {
            mSquares[i][j] = createMSquare(controlNodes[i][j + 1], controlNodes[i + 1][j + 1], controlNodes[i + 1][j], controlNodes[i][j]);
        }
    }

    for (unsigned int k = 0; k < mDataDimensions.x-1; k++) {
        for (unsigned int l = 0; l < mDataDimensions.y-1; l++) {
            triangulateSquare(&mSquares[k][l]);
        }
    }
    mIndexData = new unsigned int[mTempIndexData.size()];
    mVertexData = new  Vertex[mTempVertexData.size()];

    std::copy(mTempIndexData.begin(), mTempIndexData.end(), mIndexData);
    std::copy(mTempVertexData.begin(), mTempVertexData.end(), mVertexData);

    for (unsigned int m = 0; m < mDataDimensions.x-1; m++) {
        delete[] controlNodes[m];
        delete[] mSquares[m];
    }
    delete[] controlNodes[mDataDimensions.x-1];
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

    square.mType = 0;

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

Map::ControlNode Map::createControlNode(const glm::vec3 position, const bool active, const float squareSize, glm::uvec2 index)
{
    ControlNode node;

    node.mActive = active;
    node.mTexCoords = glm::vec2((float)index.x / (float)mDataDimensions.x, (float)index.y / (float)mDataDimensions.y);
    node.mAbove = createMeshNode(position, index, true, squareSize, node.mTexCoords);
    node.mRight = createMeshNode(position, index, false, squareSize, node.mTexCoords);
    node.mPosition = position;
    node.mVertexIndex = -1;


    return node;
}

Map::MeshNode Map::createMeshNode(const glm::vec3 position, glm::uvec2 index, bool above, const float squareSize, glm::vec2 texCoords)
{
    MeshNode node; 
    node.mVertexIndex = -1;
    if (above) {
        node.mPosition = position + glm::vec3(0.f, 0.f, 1.f)* squareSize / 2.f;
        node.mTexCoords = texCoords + glm::vec2(0.f, (0.5f / (float)mDataDimensions.y));
    } else {
        node.mPosition = position + glm::vec3(1.f, 0.f, 0.f)* squareSize / 2.f;
        node.mTexCoords = texCoords + glm::vec2((0.5f / (float)mDataDimensions.x), 0.f);
    }

    return node;
}

void Map::createMesh(MeshNode* node, unsigned int size)
{
    for (unsigned int i = 0; i < size; i++) {
        if (node[i].mVertexIndex == -1) {
            node[i].mVertexIndex = mVertexNr;
            Vertex vertex;
            vertex.position = node[i].mPosition;
            vertex.normal = glm::vec3(0.f, 1.f, 0.f);
            vertex.tangent = glm::vec3(0.0f, 0.0f, -1.0f);
            vertex.textureCoordinate = node[i].mTexCoords;
            mTempVertexData.push_back(vertex);
            mVertexNr++;
        }
    }

    if (size >= 3)
        storeTriangle(node[0], node[1], node[2]);
    if (size >= 4)
        storeTriangle(node[0], node[2], node[3]);
    if (size >= 5)
        storeTriangle(node[0], node[3], node[4]);
    if (size >= 6)
        storeTriangle(node[0], node[4], node[5]);
}

void Map::triangulateSquare(MSquare* square)
{
    MeshNode* node = nullptr;
    switch (square->mType) {
    case 0:
        break;
        // 1 points:
    case 1:
        createMesh(node = new MeshNode[3]{ square->mCenterBottom, square->mBottomLeft, square->mCenterLeft }, 3);
        break;
    case 2:
        createMesh(node = new MeshNode[3]{ square->mCenterRight, square->mBottomRight, square->mCenterBottom }, 3);
        break;
    case 4:
        createMesh(node = new MeshNode[3]{ square->mCenterTop, square->mTopRight, square->mCenterRight }, 3);
        break;
    case 8:
        createMesh(node = new MeshNode[3]{ square->mTopLeft, square->mCenterTop, square->mCenterLeft }, 3);
        break;

        // 2 points:
    case 3:
        createMesh(node = new MeshNode[4]{ square->mCenterRight, square->mBottomRight, square->mBottomLeft, square->mCenterLeft }, 4);
        break;
    case 6:
        createMesh(node = new MeshNode[4]{ square->mCenterTop, square->mTopRight, square->mBottomRight, square->mCenterBottom }, 4);
        break;
    case 9:
        createMesh(node = new MeshNode[4]{ square->mTopLeft, square->mCenterTop, square->mCenterBottom, square->mBottomLeft }, 4);
        break;
    case 12:
        createMesh(node = new MeshNode[4]{ square->mTopLeft, square->mTopRight, square->mCenterRight, square->mCenterLeft }, 4);
        break;
    case 5:
        createMesh(node = new MeshNode[6]{ square->mCenterTop, square->mTopRight, square->mCenterRight, square->mCenterBottom, square->mBottomLeft, square->mCenterLeft }, 6);
        break;
    case 10:
        createMesh(node = new MeshNode[6]{ square->mTopLeft, square->mCenterTop, square->mCenterRight, square->mBottomRight, square->mCenterBottom, square->mCenterLeft }, 6);
        break;

        // 3 point:
    case 7:
        createMesh(node = new MeshNode[5]{ square->mCenterTop, square->mTopRight, square->mBottomRight, square->mBottomLeft, square->mCenterLeft }, 5);
        break;
    case 11:
        createMesh(node = new MeshNode[5]{ square->mTopLeft, square->mCenterTop, square->mCenterRight, square->mBottomRight, square->mBottomLeft }, 5);
        break;
    case 13:
        createMesh(node = new MeshNode[5]{ square->mTopLeft, square->mTopRight, square->mCenterRight, square->mCenterBottom,square->mBottomLeft }, 5);
        break;
    case 14:
        createMesh(node = new MeshNode[5]{ square->mTopLeft, square->mTopRight, square->mBottomRight, square->mCenterBottom, square->mCenterLeft }, 5);
        break;

        // 4 point:
    case 15:
        createMesh(node = new MeshNode[4]{ square->mTopLeft, square->mTopRight, square->mBottomRight, square->mBottomLeft }, 4);
        break;
    }
    if (node != nullptr)
        delete node;
}

void Geometry::Map::storeTriangle(MeshNode a, MeshNode b, MeshNode c)
{
    mTempIndexData.push_back(a.mVertexIndex);
    mTempIndexData.push_back(b.mVertexIndex);
    mTempIndexData.push_back(c.mVertexIndex);
    mIndexNr += 3;
}
