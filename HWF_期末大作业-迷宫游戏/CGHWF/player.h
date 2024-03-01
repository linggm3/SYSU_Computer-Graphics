#ifndef PLAYER_H
#define PLAYER_H

#include "camera.h"
#include "maze.h"



/*###################################################
##  类: Player
##  类描述： 储存游戏信息
#####################################################*/
class Player
{

public:
    Maze* maze; //迷宫地图信息
    Camera* camera; //摄像机

    float posX, posZ;
    float targetXL, targetXR, targetZF, targetZB;       //终点范围
    float invWallSize;  //1/墙体大小
    bool isJumping = 0;
    float jumpStartTime;
    float currentTime;
    bool isEnded = false;
    bool money = false;
    Player();
    ~Player();
    void FBwardMove(Action action);
    void HorizontalMove(Action action);
    void VerticalMove(Action action);
    bool checkWin();
    bool checkTrap();
    bool checkMoney();
    void setFixed();
    bool isFixed();
};

#endif // PLAYER_H
