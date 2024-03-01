#include "maze.h"
/*###################################################
##  函数: Maze
##  函数描述： 初始化迷宫参数，随机生成迷宫
##  参数描述： height, width 迷宫宽高
#####################################################*/
Maze::Maze(int height, int width)
{
    Height = height;
    Width = width;
    xOrigin = 0;
    yOrigin = 1;
    xEnd = Height - 2;
    yEnd = Width - 1;  
    wallsize = 70.0f;
    std::fill(maze[0], maze[0] + Height * Width, 1);
    std::fill(blockHeight[0], blockHeight[0] + Height * Width, 10.0f * wallsize);   
    blockHeight[xOrigin][yOrigin] = 0.0f;
    //随机生成迷宫
    randGo(1, 1);

    // 以下代码为生成动态墙壁的代码，每个块有 dynamicWallProportion 的概率成为动态的墙壁
    // 当且仅当 该块是墙壁 且 对应数组元素为1，这一块是动态墙壁
    // 初始化随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(dynamicWallProportion);

    // 填充数组
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            isDynamicWall[i][j] = d(gen);
        }
    }
}
Maze::~Maze()
{

}

/*###################################################
##  函数: isArea
##  函数描述： 判断位置是否在迷宫内
##  参数描述： x, y 目标位置
#####################################################*/
bool Maze::isArea(int x, int y)
{
    return x >= 0 && x < Height && y >= 0 && y < Width;
}

/*###################################################
##  函数: isWall
##  函数描述： 判断位置是否是墙体
##  参数描述： x, y 目标位置
#####################################################*/
bool Maze::isWall(int x, int y)
{
    return isArea(x, y) && maze[x][y] == 1;
}

/*###################################################
##  函数: isTrap
##  函数描述： 判断位置是否是陷阱
##  参数描述： x, y 目标位置
#####################################################*/
bool Maze::isTrap(int x, int y)
{
    return isArea(x, y) && maze[x][y] == 2;
}

/*###################################################
##  函数: isLowWall
##  函数描述： 判断位置是否是矮墙
##  参数描述： x, y 目标位置
#####################################################*/
bool Maze::isLowWall(int x, int y)
{
    return isArea(x, y) && maze[x][y] == 3;
}

/*###################################################
##  函数: isMoney
##  函数描述： 判断位置是否是陷阱
##  参数描述： x, y 目标位置
#####################################################*/
bool Maze::isMoney(int x, int y)
{
    return isArea(x, y) && maze[x][y] == 4;
}

/*###################################################
##  函数: isValid
##  函数描述： 判断位置是否是可行路径（包括通道，矮墙上方，陷阱上方）
##  参数描述： x, y 目标位置
#####################################################*/
bool Maze::isValid(int x, int y, int z )
{
    return isArea(x, y) && (maze[x][y] != 1 && z >= 1.6f * wallsize + blockHeight[x][y]);
}

/*###################################################
##  函数: randGo
##  函数描述： 随机迷宫生成算法
##  参数描述： x, y 迷宫起点
#####################################################*/
void Maze::randGo(int x, int y)
{
    //设置一个中间的二维数组保存迷宫信息
    int tmpMaze[100][100];
    //将迷宫的边缘初始化为路，防止出现多个开口
    //其余地方全部初始化为墙
    for (int i = 0; i < Height+2; i++)
    {
        for (int j = 0; j < Width+2; j++)
        {
            tmpMaze[i][j] = 1;
            if (i == 0 || i == Height+1) tmpMaze[i][j] = 0;
            if (j == 0 || j == Width+1) tmpMaze[i][j] = 0;
        }
    }

    std::vector<std::pair<int, int>> wall;          //设置墙队列
    wall.push_back(std::pair<int, int>(x+1, y+1));  //将起点添加进墙队列
    srand(time(NULL));
    while(!wall.empty())    //如果墙队列不为空
    {
        //从墙队列中随机选择一个墙体出来
        int index = rand() % wall.size();
        std::pair<int, int> pos = wall[index];
        //统计该墙体上下左右是路的个数
        int count = 0;
        for (int i = 0; i < 4; i++)
        {
            int newX = pos.first + dir[i][0];
            int newY = pos.second + dir[i][1];
            if (tmpMaze[newX][newY] == 0) count++;
        }
        //如果该位置上下左右中是路的个数小于1，则可以将该位置设为路
        //同时将该位置上下左右中是墙的位置添加进墙队列
        if (count <= 1)
        {
            tmpMaze[pos.first][pos.second] = 0;
            for(int i = 0; i < 4; i++)
            {
                int newX = pos.first + dir[i][0];
                int newY = pos.second + dir[i][1];
                if (tmpMaze[newX][newY] != 0) wall.push_back(std::pair<int, int>(newX, newY));
            }
        }
        //将该墙体从墙队列中移除
        wall.erase(wall.begin() + index);
    }
    //由于tmpMaze比实际的迷宫大了一圈无用的边缘
    //我们将tmpMaze边缘一圈去掉，得到实际生成的迷宫
    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            maze[i][j] = tmpMaze[i+1][j+1];
        }
    }

    //krustal方法生成迷宫
    /*MazeGeneratorhhh mazeGenerator2(Height, Width);
    std::vector<std::vector<int>> MAZE = mazeGenerator2.getMazeMatrix();
    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            maze[i][j] = MAZE[i][j];
        }
    }*/

    // 收集所有通道的位置
    std::vector<std::pair<int, int>> paths;
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            if (maze[i][j] == 0) {
                paths.push_back(std::make_pair(i, j));
                blockHeight[i][j] = 0.0f;  // 设置该块的高度为0.0
            }
        }
    }
    
    for (int i = 0; i < Height * Width / 4; i++) {
        // 如果存在通道，则随机选择一个设置为矮墙
        if (!paths.empty()) {
            // srand(time(NULL)); // 初始化随机数生成器
            int randomIndex = rand() % paths.size(); // 随机选择一个索引
            std::pair<int, int> trapPos = paths[randomIndex];
            maze[trapPos.first][trapPos.second] = 3; // 设置选中的通道为矮墙
            blockHeight[trapPos.first][trapPos.second] = float(rand() % 4 + 1) / 4 * wallsize * 2.0f;  // 随机生成矮墙的高度\
        }
    }

    for (int i = 0; i < Height * Width / 12; i++) {
        // 如果存在通道，则随机选择一个设置为陷阱
        if (!paths.empty()) {
            int randomIndex = rand() % paths.size(); // 随机选择一个索引
            std::pair<int, int> trapPos = paths[randomIndex];
            maze[trapPos.first][trapPos.second] = 2; // 设置选中的通道为陷阱
            blockHeight[trapPos.first][trapPos.second] = 0.0f;  // 陷阱高度为0
        }
    }

    for (int i = 0; i < Height * Width / 12; i++) {
        // 如果存在通道，则随机选择一个设置为金币
        if (!paths.empty()) {
            int randomIndex = rand() % paths.size(); // 随机选择一个索引
            std::pair<int, int> trapPos = paths[randomIndex];
            maze[trapPos.first][trapPos.second] = 4; // 设置选中的通道为金币
            blockHeight[trapPos.first][trapPos.second] = 0.0f;  // 金币高度为0
            break;
        }
    }

    //计算起点和终点位置
    maze[xOrigin][yOrigin] = 0;
    for (int i = Height-1; i >= 0; i--)
    {
        if (maze[i][Width-2] == 0)
        {
            maze[i][Width-1] = 0;
            xEnd = i;
            yEnd = Width - 1;
            break;
        }
    }

    maze[xEnd][yEnd] = 0;
    blockHeight[xEnd][yEnd] = 0.0f;
}

