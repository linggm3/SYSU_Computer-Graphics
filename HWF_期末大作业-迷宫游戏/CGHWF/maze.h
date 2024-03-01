#ifndef MAZE_H
#define MAZE_H
#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <time.h>
#include "textures.h"
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <ctime>
/*###################################################
##  类: Maze
##  类描述：迷宫信息
#####################################################*/
const int dir[4][2] = {
    {-1,0}, {0, 1},
    {1, 0}, {0, -1}
    };
class Maze
{
public:
    int Height, Width;  //迷宫大小
    int xOrigin, yOrigin;       //起点
    int xEnd, yEnd;         //终点
    int maze[100][100];     //迷宫
    float blockHeight[100][100];  //迷宫中每个元素块的高度
    bool isDynamicWall[100][100];  //墙壁是否是动态的
    float dynamicWallProportion = 0.125f; //动态墙壁的比例
    float wallsize;     //墙体大小
    Textures textures;  //纹理
    Maze(int height, int width);
    ~Maze();
    bool isArea(int x, int y);
    bool isWall(int x, int y);
    bool isTrap(int x, int y);
    bool isLowWall(int x, int y);
    bool isMoney(int x, int y);
    bool isValid(int x, int y, int z);
    void randGo(int x, int y);
    int LoadTexture(const char* filename);
};


class UnionFindxxx {
public:
    UnionFindxxx(int size) : parent(size), rank(size, 1) {
        std::iota(parent.begin(), parent.end(), 0);  // 初始化每个元素的父节点为自身
    }

    int find(int x) {
        if (x != parent[x]) {
            parent[x] = find(parent[x]);  // 路径压缩
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            }
            else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            }
            else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

class MazeGeneratorhhh {
public:
    MazeGeneratorhhh(int rows, int cols) : rows(rows), cols(cols), uf(rows* cols) {
        srand(static_cast<unsigned>(time(nullptr)));
        initializeEdges();
        generateMaze();
    }

    std::vector<std::vector<int>> getMazeMatrix() {
        std::vector<std::vector<int>> mazeMatrix(rows * 2 + 1, std::vector<int>(cols * 2 + 1, 1));

        // 打开单元格和连接的路径
        for (auto& edge : connectedEdges) {
            int x1 = edge.first / cols;
            int y1 = edge.first % cols;
            int x2 = edge.second / cols;
            int y2 = edge.second % cols;

            mazeMatrix[2 * x1 + 1][2 * y1 + 1] = 0;
            mazeMatrix[2 * x2 + 1][2 * y2 + 1] = 0;
            mazeMatrix[x1 + x2 + 1][y1 + y2 + 1] = 0;
        }

        return mazeMatrix;
    }


private:
    int rows;
    int cols;
    UnionFindxxx uf;
    std::vector<std::pair<int, int>> edges;
    std::vector<std::pair<int, int>> connectedEdges;

    void initializeEdges() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (i > 0) edges.emplace_back(i * cols + j, (i - 1) * cols + j);
                if (j > 0) edges.emplace_back(i * cols + j, i * cols + j - 1);
            }
        }
    }

    void generateMaze() {
        std::random_shuffle(edges.begin(), edges.end());

        // store the edges of the tree 
        for (const auto& edge : edges) {
            int cell1 = edge.first;
            int cell2 = edge.second;

            if (uf.find(cell1) != uf.find(cell2)) {
                uf.unite(cell1, cell2);
                connectedEdges.push_back(edge);
            }
        }
    }
};

#endif // MAZE_H
