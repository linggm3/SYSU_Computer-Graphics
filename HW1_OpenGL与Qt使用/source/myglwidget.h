#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

/*
构造函数：初始化scene_id为0。
析构函数：留空，因为没有特别的清理工作需要在对象销毁时执行。
initializeGL：设置OpenGL的视口，清除颜色和禁用深度测试。
paintGL：根据scene_id的值决定调用scene_0还是scene_1进行绘制。
resizeGL：在窗口大小改变时更新OpenGL的视口。
keyPressEvent：处理键盘事件以在两个场景之间切换。
scene_0和scene_1：定义两个不同的场景。scene_0中画了一个旋转45度的红色“I”形状，scene_1则留给使用者自定义实现。
*/


class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
	void keyPressEvent(QKeyEvent *e);

private:
	int scene_id;
    float lookX, lookY, lookZ; // 观察点方向
	void scene_0();
};
#endif // MYGLWIDGET_H
