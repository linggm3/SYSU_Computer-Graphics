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
���캯������ʼ��scene_idΪ0��
�������������գ���Ϊû���ر����������Ҫ�ڶ�������ʱִ�С�
initializeGL������OpenGL���ӿڣ������ɫ�ͽ�����Ȳ��ԡ�
paintGL������scene_id��ֵ��������scene_0����scene_1���л��ơ�
resizeGL���ڴ��ڴ�С�ı�ʱ����OpenGL���ӿڡ�
keyPressEvent����������¼�������������֮���л���
scene_0��scene_1������������ͬ�ĳ�����scene_0�л���һ����ת45�ȵĺ�ɫ��I����״��scene_1������ʹ�����Զ���ʵ�֡�
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
    float lookX, lookY, lookZ; // �۲�㷽��
	void scene_0();
};
#endif // MYGLWIDGET_H
