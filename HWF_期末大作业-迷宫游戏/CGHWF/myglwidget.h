#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QTime>
#include "camera.h"
#include "maze.h"
#include "textures.h"
#include "player.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>



class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();
    void paintMaze();
    void paintSkyBox();
    void paintMark();
    bool keyStates[256]; // 假设有256个可能的键值
    bool isJumping;          // 是否正在跳跃
    bool isFalling;          // 是否 不在跳跃 且 自然掉落
    bool isFlying;          // 是否正在飞行
    bool isforwarding;
    float forwardStartTime;     
    float jumpStartTime;     // 跳跃开始的时间（跳跃包括跳跃下坠）
    float jumpInitialHeight; // 跳跃开始时的高度
    float fallStartTime;  // 记录自然下坠的开始时间（不包括跳跃下坠）
    float fallInitialHeight; // 自然下坠开始时的高度
    float forwardinitial;
    float jumpPeakHeight;    // 跳跃的最高点
    float jumpVelocity; // 跳跃的初始速度
    float moveVelocity;
    float gravity = 1000;
    int updateCounter = 0;  // 记录更新次数
    int currentFrameForTrap = 0;  // 陷阱动画纹理当前帧数
    int frameSizeForTrap;  // 陷阱动画纹理总帧数
    int currentFrameForWall = 0;  // 墙壁动画纹理当前帧数
    int frameSizeForWall;  // 墙壁动画纹理总帧数
    GLfloat lightPosition[4] = { 1000.0f, 1300.0f, 1000.0f, 1.0f }; // 初始光源位置
    const float frameTime = 0.01f; // 10 毫秒转换为秒
    float skyboxAngle = 0.0f; // 天空盒旋转角度
  
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* e);
    void updateJumpState();
    void updateFallState();  // 从矮墙上往通道走，要自然下坠
    void updatespacestate(void);
    void drawMiniMap();
    void updateforward();
    void updateFlyState();
    void updateLightPosition();


private:
    QTimer* timer;  //计时器
    Player* player; //玩家
    Textures skyBox;    //天空盒纹理
    std::vector<vec3> snow; //标记终点的纹理
    float lastX, lastY;     //鼠标位置
  
    bool firstMouse;
};
#endif // MYGLWIDGET_H
