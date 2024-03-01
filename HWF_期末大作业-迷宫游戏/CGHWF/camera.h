#ifndef CAMERA_H
#define CAMERA_H
#include <GL/glut.h>
#include <cmath>
#include <algorithm>
#include "vec3.h"

/*###################################################
##  枚举变量: Action
##  变量描述：移动动作
#####################################################*/
enum Action {
    forward, backward, upward, downward, toleft, toright
};
/*###################################################
##  类: Camera
##  类描述： 摄像机
#####################################################*/
class Camera
{
private:
    float mouse_sensitivity;    //鼠标灵敏度
    float fbward_speed;         //前后移动速度
    float vertical_speed;       //垂直移动速度
    float horizontal_speed;     //水平移动速度
    float pitch_speed;      //俯仰角变换速度
    float yaw_speed;        //偏航角移动速度
    float pitch, yaw;       //俯仰角，偏航角偏移角度
    vec3 worldUp;           //世界坐标的Up向量
public:
    float zoom;     //视角大小
    vec3 position;  //位置
    vec3 in;
    vec3 right;
    vec3 front;
    vec3 up;

    Camera(const vec3& pos, const vec3& target, const vec3& r, float Pit, float Yaw);

    void FBwardMove(Action action);
    void HorizontalMove(Action action);
    void VerticalMove(Action action);
    void ProcessMouseMovement(float xOffset, float yOffset);
    void setFixed();
};
float radian(const float angle);
#endif // CAMERA_H
