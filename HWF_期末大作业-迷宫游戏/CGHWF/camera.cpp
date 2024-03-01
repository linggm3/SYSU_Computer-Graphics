#include "camera.h"
/*###################################################
##  函数: Camera
##  函数描述： 设置摄像机参数
##  参数描述： pos 摄像机位置
##          target 摄像机观察目标
##          r 摄像机right向量
##          pitch 俯仰角角度， yaw 偏航角角度
#####################################################*/
Camera::Camera(const vec3& pos, const vec3& target, const vec3& r, float Pit, float Yaw)
{
    //构建摄像机坐标系
    position = pos;
    right = normalize(r);
    in = normalize(pos - target);
    front = -in;
    up = normalize(cross(in, right));
    //设置速度
    fbward_speed = vertical_speed = horizontal_speed = 5.0f;
    pitch_speed = yaw_speed = 0.3f;
    //设置视角
    zoom = 55.0f;
    mouse_sensitivity = 0.3f;
    pitch = Pit;
    yaw = Yaw;
    worldUp = vec3(0.0f, 1.0f, 0.0);
}
/*###################################################
##  函数: FBwardMove
##  函数描述： 控制摄像机前后向运动
##  参数描述： action 移动动作
#####################################################*/
void Camera::FBwardMove(Action action)
{
    if (action == forward)
    {
        position += fbward_speed * vec3(front.x, 0.0f, front.z);
    }
    else
    {
        position -= fbward_speed * vec3(front.x, 0.0f, front.z);
    }
}
/*###################################################
##  函数: HorizontalMove
##  函数描述： 控制摄像机左右向运动
##  参数描述： action 移动动作
#####################################################*/
void Camera::HorizontalMove(Action action)
{
    if (action == toleft)
    {
        position -= horizontal_speed * vec3(right.x, 0.0f, right.z);
    }
    else
    {
        position += horizontal_speed * vec3(right.x, 0.0f, right.z);
    }
}
/*###################################################
##  函数: VerticalMove
##  函数描述： 控制摄像机垂直向运动
##  参数描述： action 移动动作
#####################################################*/
void Camera::VerticalMove(Action action)
{
    if (action == upward)
    {
        position.y += 10 * vertical_speed;
    }
    else
    {
        position.y -= 10 * vertical_speed;
    } 
}
/*###################################################
##  函数: ProcessMouseMovement
##  函数描述： 控制摄像机视角移动
##  参数描述： xOffset 水平方向移动距离
##           yOffset 垂直方向移动距离
#####################################################*/
void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
    //计算视角偏移量
    xOffset *= yaw_speed;
    yOffset *= pitch_speed;
    //更新俯仰角，偏航角角度
    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    //更新摄像机坐标系
    vec3 tmp;
    tmp.x = std::cos(radian(yaw)) * std::cos(radian(pitch));
    tmp.y = std::sin(radian(pitch));
    tmp.z = std::sin(radian(yaw)) * std::cos(radian(pitch));
    front = normalize(tmp);
    right = normalize(cross(front, worldUp));
    up = normalize(cross(right, front));

}
/*###################################################
##  函数: radian
##  函数描述： 角度转弧度
##  参数描述： angle 角度
#####################################################*/
float radian(const float angle)
{
    return angle * 3.1415926535897626f / 180.0f;
}

/*###################################################
##  函数: setFixed
##  函数描述： 用于固定摄像机
##  参数描述： 
#####################################################*/
void Camera::setFixed() {
    fbward_speed = vertical_speed = horizontal_speed = 0.0f;
    pitch_speed = yaw_speed = 0.0f;
}