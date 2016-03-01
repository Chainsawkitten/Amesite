#include "Map.hpp"
#include "../Util/Log.hpp"

using namespace Geometry;

Map::Map(bool **data, glm::uvec2 dataDimensions, float wallHeight) {
    mDataDimensions = dataDimensions;
    mWallHeight = wallHeight;
    mTextureRepeat = glm::vec2(60.f, 60.f);
    MarchingSquares(data, 1.f);

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

int** Map::GetTypeMap() const {
    return mTypeMap;
}

glm::vec2 Geometry::Map::TextureRepeat() const {
    return mTextureRepeat;
}

void Geometry::Map::SetTextureRepeat(const glm::vec2 repeat) {
    mTextureRepeat = repeat;
}

void Map::MarchingSquares(bool ** data, const float squareSize) {
    mMapWidth = mDataDimensions.x * squareSize;
    mMapHeight = mDataDimensions.y * squareSize;

    ControlNode** controlNodes = new ControlNode*[mDataDimensions.x];
    MSquare** mSquares = new MSquare*[mDataDimensions.x - 1];

    // Node creation for marching squares
    for (unsigned int x = 0; x < mDataDimensions.x; x++) {
        controlNodes[x] = new ControlNode[mDataDimensions.y];
        for (unsigned int y = 0; y < mDataDimensions.y; y++) {
            glm::vec3 pos = glm::vec3(-mMapWidth / 2.f + x * squareSize + squareSize / 2.f, 0.f, -mMapHeight / 2.f + y * squareSize + squareSize / 2.f);
            controlNodes[x][y] = CreateControlNode(pos, data[y][x], squareSize, glm::uvec2(x, y));
        }
    }

    mTypeMap = new int*[mDataDimensions.x - 1];

    // Node creation for marching squares
    for (unsigned int x = 0; x < mDataDimensions.x - 1; x++) {
        mSquares[x] = new MSquare[mDataDimensions.y - 1];
        mTypeMap[x] = new int[mDataDimensions.y - 1];
        for (unsigned int y = 0; y < mDataDimensions.y - 1; y++) {
            mSquares[x][y] = CreateMSquare(controlNodes[x][y + 1], controlNodes[x + 1][y + 1], controlNodes[x + 1][y], controlNodes[x][y]);
            mTypeMap[x][y] = mSquares[x][y].mType;
        }
    }

    // Top mesh generation.
    for (unsigned int x = 0; x < mDataDimensions.x-1; x++) {
        for (unsigned int y = 0; y < mDataDimensions.y-1; y++) {
            TriangulateSquare(&mSquares[x][y]);
        }
    }

    // Wall mesh generation
    CreateWallMesh();

    // Initialize index and vertex data and copy from temp.
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

Map::MapTriangle Map::CreateMapTriangle(int a, int b, int c) {
    MapTriangle triangle;
    triangle.indexA = a;
    triangle.indexB = b;
    triangle.indexC = c;

    triangle.indices[0] = a;
    triangle.indices[1] = b;
    triangle.indices[2] = c;
    return triangle;
}

Map::ControlNode Map::CreateControlNode(const glm::vec3 position, const bool active, const float squareSize, glm::uvec2 index) {
    ControlNode node;

    node.mActive = active;
    node.mTexCoords = glm::vec2(((float)index.x / (float)mDataDimensions.x) * mTextureRepeat.x, ((float)index.y / ((float)mDataDimensions.y))* mTextureRepeat.y);
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
        node.mTexCoords = texCoords + glm::vec2(0.f, (0.5f / ((float)mDataDimensions.y / mTextureRepeat.y)));
    } else {
        node.mPosition = position + glm::vec3(1.f, 0.f, 0.f)* squareSize / 2.f;
        node.mTexCoords = texCoords + glm::vec2((0.5f / ((float)mDataDimensions.x / mTextureRepeat.x), 0.f));
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

void Map::CreateWallMesh() {
    CalculateMeshOutlines();

    for (auto outline : mOutlines) {
        for (unsigned int j = 0; j < outline.size()-1; j++) {
            int startIndex = mTempVertexData.size();

            Vertex vertex[4];

            // Left.
            vertex[0].position = mTempVertexData[outline[j]].position;
            vertex[0].textureCoordinate = glm::vec2(0.f, 1.f);

            // Right.
            vertex[1].position = mTempVertexData[outline[j+1]].position;
            vertex[1].textureCoordinate = glm::vec2(1.f, 1.f);

            // Bottom Left.
            vertex[2].position = mTempVertexData[outline[j]].position - glm::vec3(0.f, 1.f, 0.f) * mWallHeight;
            vertex[2].textureCoordinate = glm::vec2(0.f, 0.f);

            // Bottom Right.
            vertex[3].position = mTempVertexData[outline[j+1]].position - glm::vec3(0.f, 1.f, 0.f) * mWallHeight;
            vertex[3].textureCoordinate = glm::vec2(1.f, 0.f);

            // Normal of quad.
            glm::vec3 ab = vertex[2].position - vertex[0].position;
            glm::vec3 ac = vertex[3].position - vertex[0].position;
            glm::vec3 normal = glm::normalize(glm::cross(ab, ac));

            vertex[0].normal = normal;
            vertex[1].normal = normal;
            vertex[2].normal = normal;
            vertex[3].normal = normal;


            mTempVertexData.push_back(vertex[0]);
            mTempVertexData.push_back(vertex[1]);
            mTempVertexData.push_back(vertex[2]);
            mTempVertexData.push_back(vertex[3]);

            mVertexNr += 4;

            mTempIndexData.push_back(startIndex + 0);
            mTempIndexData.push_back(startIndex + 2);
            mTempIndexData.push_back(startIndex + 3);

            mTempIndexData.push_back(startIndex + 3);
            mTempIndexData.push_back(startIndex + 1);
            mTempIndexData.push_back(startIndex + 0);

            mIndexNr += 6;
        }
    }

}

void Map::CalculateMeshOutlines() {
    for (unsigned int i = 0; i < mVertexNr; i++) {
        if (mVertexChecked.count(i) == 0) {
            int outLineVertex = GetConnectedVertex(i);
            if (outLineVertex != -1) {
                mVertexChecked.insert(i);
                std::vector<int> outline;
                outline.push_back(i);
                mOutlines.push_back(outline);
                FollowOutline(outLineVertex, mOutlines.size() - 1);
                mOutlines[mOutlines.size()-1].push_back(i);
            }
        }
    }
}

bool Map::IsOutline(int vertexA, int vertexB) {
    std::vector<MapTriangle> containingVertexA = mTriangleDictionary[vertexA];
    int triangleCount = 0;

    for (unsigned int i = 0; i < containingVertexA.size() && triangleCount <= 1; i++) {
        if (containingVertexA[i].Contains(vertexB))
            triangleCount++;
    }
    return triangleCount == 1;
}

int Map::GetConnectedVertex(int index) {
    std::vector<MapTriangle> containingVertex = mTriangleDictionary[index];

    for (unsigned int i = 0; i < containingVertex.size(); i++) {
        MapTriangle triangle = containingVertex[i];
        
        for (int j = 0; j < 3; j++) {
            int vertex = triangle.indices[j];

            if (vertex != index && mVertexChecked.count(vertex) == 0) {
                if (IsOutline(index, vertex))
                    return vertex;
            }
        }
    }
    return -1;
}

void Map::AddToDictionary(int indexKey, MapTriangle triangle) {
    if (mTriangleDictionary.count(indexKey) == 1)
        mTriangleDictionary[indexKey].push_back(triangle);
    else
        mTriangleDictionary[indexKey].push_back(triangle);
}

void Map::FollowOutline(int index, int outlineIndex) {
    mOutlines[outlineIndex].push_back(index);
    mVertexChecked.insert(index);
    int nextVertex = GetConnectedVertex(index);

    if (nextVertex != -1)
        FollowOutline(nextVertex, outlineIndex);

}


void Map::TriangulateSquare(MSquare* square) {
    MeshNode* node = nullptr;
    switch (square->mType) {
    case 0:
        break;
        // 1 points:
    case 1:
        CreateMesh(node = new MeshNode[3]{ square->mCenterLeft, square->mCenterBottom, square->mBottomLeft }, 3);
        break;
    case 2:
        CreateMesh(node = new MeshNode[3]{ square->mBottomRight, square->mCenterBottom, square->mCenterRight }, 3);
        break;
    case 4:
        CreateMesh(node = new MeshNode[3]{ square->mTopRight, square->mCenterRight, square->mCenterTop }, 3);
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
        mVertexChecked.insert(node[0].mVertexIndex);
        mVertexChecked.insert(node[1].mVertexIndex);
        mVertexChecked.insert(node[2].mVertexIndex);
        mVertexChecked.insert(node[3].mVertexIndex);
        break;
    }
    if (node != nullptr)
        delete[] node;
}

void Map::StoreTriangle(MeshNode a, MeshNode b, MeshNode c) {
    mTempIndexData.push_back(a.mVertexIndex);
    mTempIndexData.push_back(b.mVertexIndex);
    mTempIndexData.push_back(c.mVertexIndex);
    mIndexNr += 3;

    int tempIndexA, tempIndexB, tempIndexC;
    std::vector<int> indexPositionA, indexPositionB, indexPositionC;
    indexPositionA.push_back((int)(a.mPosition.x * 10.f));
    indexPositionA.push_back((int)(a.mPosition.y * 10.f));
    indexPositionA.push_back((int)(a.mPosition.z * 10.f));

    indexPositionB.push_back((int)(b.mPosition.x * 10.f));
    indexPositionB.push_back((int)(b.mPosition.y * 10.f));
    indexPositionB.push_back((int)(b.mPosition.z * 10.f));

    indexPositionC.push_back((int)(c.mPosition.x * 10.f));
    indexPositionC.push_back((int)(c.mPosition.y * 10.f));
    indexPositionC.push_back((int)(c.mPosition.z * 10.f));


    if (mVertexIndexMap.count(indexPositionA) == 0)
        mVertexIndexMap[indexPositionA] = a.mVertexIndex;

    if (mVertexIndexMap.count(indexPositionB) == 0)
        mVertexIndexMap[indexPositionB] = b.mVertexIndex;

    if (mVertexIndexMap.count(indexPositionC) == 0)
        mVertexIndexMap[indexPositionC] = c.mVertexIndex;

    tempIndexA = mVertexIndexMap[indexPositionA];
    tempIndexB = mVertexIndexMap[indexPositionB];
    tempIndexC = mVertexIndexMap[indexPositionC];

    MapTriangle triangle = CreateMapTriangle(tempIndexA, tempIndexB, tempIndexC);
    AddToDictionary(triangle.indexA, triangle);
    AddToDictionary(triangle.indexB, triangle);
    AddToDictionary(triangle.indexC, triangle);
}

bool Map::MapTriangle::Contains(int index) {
    return (index == indexA || index == indexB || index == indexC);
}
