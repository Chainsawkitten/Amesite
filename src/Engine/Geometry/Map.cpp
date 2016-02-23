#include "Map.hpp"
#include "../Util/Log.hpp"

using namespace Geometry;

Map::Map(bool **data, const float squareSize, glm::uvec2 dataDimensions) {
    mDataDimensions = dataDimensions;
    MarchingSquares(data, squareSize);

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

void Map::MarchingSquares(bool ** data, const float squareSize) {
    mMapWidth = mDataDimensions.x * squareSize;
    mMapHeight = mDataDimensions.y * squareSize;

    ControlNode** controlNodes = new ControlNode*[mDataDimensions.x];
    MSquare** mSquares = new MSquare*[mDataDimensions.x - 1];

    for (unsigned int x = 0; x < mDataDimensions.x; x++) {
        controlNodes[x] = new ControlNode[mDataDimensions.y];
        for (unsigned int y = 0; y < mDataDimensions.y; y++) {
            glm::vec3 pos = glm::vec3(-mMapWidth / 2.f + x * squareSize + squareSize / 2.f, 0.f, -mMapHeight / 2.f + y * squareSize + squareSize / 2.f);
            controlNodes[x][y] = CreateControlNode(pos, data[y][x], squareSize, glm::uvec2(x, y));
        }
    }

    for (unsigned int x = 0; x < mDataDimensions.x - 1; x++) {
        mSquares[x] = new MSquare[mDataDimensions.y - 1];
        for (unsigned int y = 0; y < mDataDimensions.y - 1; y++) {
            mSquares[x][y] = CreateMSquare(controlNodes[x][y + 1], controlNodes[x + 1][y + 1], controlNodes[x + 1][y], controlNodes[x][y]);
        }
    }

    for (unsigned int x = 0; x < mDataDimensions.x-1; x++) {
        for (unsigned int y = 0; y < mDataDimensions.y-1; y++) {
            TriangulateSquare(&mSquares[x][y]);
        }
    }
    mIndexData = new unsigned int[mTempIndexData.size()];
    mVertexData = new Vertex[mTempVertexData.size()];

    std::copy(mTempIndexData.begin(), mTempIndexData.end(), mIndexData);
    std::copy(mTempVertexData.begin(), mTempVertexData.end(), mVertexData);
    
    // Free used memory.
    for (unsigned int m = 0; m < mDataDimensions.x-1; m++) {
        delete[] controlNodes[m];
        delete[] mSquares[m];
    }
    delete[] controlNodes[mDataDimensions.x-1];
    delete[] controlNodes;
    delete[] mSquares;
    
}

Map::MSquare Map::CreateMSquare(ControlNode topLeft, ControlNode topRight, ControlNode bottomRight, ControlNode bottomLeft) {
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

Map::ControlNode Map::CreateControlNode(const glm::vec3 position, const bool active, const float squareSize, glm::uvec2 index) {
    ControlNode node;

    node.mActive = active;
    node.mTexCoords = glm::vec2((float)index.x / (float)mDataDimensions.x, (float)index.y / (float)mDataDimensions.y);
    node.mAbove = CreateMeshNode(position, index, true, squareSize, node.mTexCoords);
    node.mRight = CreateMeshNode(position, index, false, squareSize, node.mTexCoords);
    node.mPosition = position;
    node.mVertexIndex = -1;


    return node;
}

Map::MeshNode Map::CreateMeshNode(const glm::vec3 position, glm::uvec2 index, bool above, const float squareSize, glm::vec2 texCoords) {
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

void Map::CreateMesh(MeshNode* node, unsigned int size) {
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
        StoreTriangle(node[0], node[1], node[2]);
    if (size >= 4)
        StoreTriangle(node[0], node[2], node[3]);
    if (size >= 5)
        StoreTriangle(node[0], node[3], node[4]);
    if (size >= 6)
        StoreTriangle(node[0], node[4], node[5]);
}

void Map::TriangulateSquare(MSquare* square) {
    MeshNode* node = nullptr;
    switch (square->mType) {
    case 0:
        break;
        // 1 points:
    case 1:
        CreateMesh(node = new MeshNode[3]{ square->mCenterBottom, square->mBottomLeft, square->mCenterLeft }, 3);
        break;
    case 2:
        CreateMesh(node = new MeshNode[3]{ square->mCenterRight, square->mBottomRight, square->mCenterBottom }, 3);
        break;
    case 4:
        CreateMesh(node = new MeshNode[3]{ square->mCenterTop, square->mTopRight, square->mCenterRight }, 3);
        break;
    case 8:
        CreateMesh(node = new MeshNode[3]{ square->mTopLeft, square->mCenterTop, square->mCenterLeft }, 3);
        break;

        // 2 points:
    case 3:
        CreateMesh(node = new MeshNode[4]{ square->mCenterRight, square->mBottomRight, square->mBottomLeft, square->mCenterLeft }, 4);
        break;
    case 6:
        CreateMesh(node = new MeshNode[4]{ square->mCenterTop, square->mTopRight, square->mBottomRight, square->mCenterBottom }, 4);
        break;
    case 9:
        CreateMesh(node = new MeshNode[4]{ square->mTopLeft, square->mCenterTop, square->mCenterBottom, square->mBottomLeft }, 4);
        break;
    case 12:
        CreateMesh(node = new MeshNode[4]{ square->mTopLeft, square->mTopRight, square->mCenterRight, square->mCenterLeft }, 4);
        break;
    case 5:
        CreateMesh(node = new MeshNode[6]{ square->mCenterTop, square->mTopRight, square->mCenterRight, square->mCenterBottom, square->mBottomLeft, square->mCenterLeft }, 6);
        break;
    case 10:
        CreateMesh(node = new MeshNode[6]{ square->mTopLeft, square->mCenterTop, square->mCenterRight, square->mBottomRight, square->mCenterBottom, square->mCenterLeft }, 6);
        break;

        // 3 point:
    case 7:
        CreateMesh(node = new MeshNode[5]{ square->mCenterTop, square->mTopRight, square->mBottomRight, square->mBottomLeft, square->mCenterLeft }, 5);
        break;
    case 11:
        CreateMesh(node = new MeshNode[5]{ square->mTopLeft, square->mCenterTop, square->mCenterRight, square->mBottomRight, square->mBottomLeft }, 5);
        break;
    case 13:
        CreateMesh(node = new MeshNode[5]{ square->mTopLeft, square->mTopRight, square->mCenterRight, square->mCenterBottom, square->mBottomLeft }, 5);
        break;
    case 14:
        CreateMesh(node = new MeshNode[5]{ square->mTopLeft, square->mTopRight, square->mBottomRight, square->mCenterBottom, square->mCenterLeft }, 5);
        break;

        // 4 point:
    case 15:
        CreateMesh(node = new MeshNode[4]{ square->mTopLeft, square->mTopRight, square->mBottomRight, square->mBottomLeft }, 4);
        break;
    }
    if (node != nullptr)
        delete[] node;
}

void Geometry::Map::StoreTriangle(MeshNode a, MeshNode b, MeshNode c)
{
    mTempIndexData.push_back(a.mVertexIndex);
    mTempIndexData.push_back(b.mVertexIndex);
    mTempIndexData.push_back(c.mVertexIndex);
    mIndexNr += 3;
}
