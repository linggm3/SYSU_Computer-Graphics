#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent),
	scene_id(0),
	lookX(0.0),
	lookY(0.0),
	lookZ(1.0)
{
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
	/*
	功能：设置OpenGL的视口（Viewport）。
	参数：(0, 0)表示视口的左下角坐标，width()和height()从QOpenGLWidget继承，返回窗口的宽度和高度。
	作用：这告诉OpenGL渲染的区域大小和位置，这里设置为覆盖整个窗口。
	*/
	glViewport(0, 0, width(), height());
	/*
	功能：设置清除屏幕时使用的颜色。
	参数：四个浮点值分别代表红色、绿色、蓝色和透明度的值，范围从0.0到1.0。这里设置为白色，不透明。
	作用：当调用glClear(GL_COLOR_BUFFER_BIT)时，屏幕会被填充为此颜色。
	*/
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// 禁用深度测试，在2D渲染或不需要考虑物体前后关系时，通常禁用深度测试，从而提高渲染效率。
	// glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
	if (scene_id==0) {
		scene_0();
	}
}

void MyGLWidget::resizeGL(int width, int height)
{
	// 重新设置OpenGL的视口，确保OpenGL渲染区域的大小与窗口大小同步
	glViewport(0, 0, width, height);
	// 请求更新这个widget
	update();
}



void MyGLWidget::keyPressEvent(QKeyEvent *e) {

	// 调整摄像机视角
	float angleSpeed = 0.1f; // 可调整的视角移动速度
	//Press 0 or 1 to switch the scene
	if (e->key() == Qt::Key_0) {
		scene_id = 0;
		update();
	}
	else if (e->key() == Qt::Key_Up) {
		lookY += angleSpeed;
	}
	else if (e->key() == Qt::Key_Down) {
		lookY -= angleSpeed;
	}
	else if (e->key() == Qt::Key_Left) {
		lookX -= angleSpeed;
	}
	else if (e->key() == Qt::Key_Right) {
		lookX += angleSpeed;
	}
	update(); // 请求更新视图
}

GLuint LoadTexture(const char* filename, int width, int height)

{
	GLuint texture;
	unsigned char* data;
	FILE* file;

	//The following code will read in our PNG file
	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char*)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture); //generate the texture with 

	glBindTexture(GL_TEXTURE_2D, texture); //bind the texture

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		GL_MODULATE); //set texture environment parameters

	//even better quality, but this will do for now.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);


	//Here we are setting the parameter to repeat the texture 
	//to the edge of our shape. 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		GL_REPEAT);

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data); //free the texture
	return texture; //return whether it was successfull
}

GLuint skyboxTexture[6]; // 存储天空盒的6个面的纹理ID

void loadSkyboxTextures() {
	// 加载天空盒的每个面的纹理
	// 这里只是一个伪代码示例，具体实现取决于你如何处理纹理加载
	skyboxTexture[0] = LoadTexture("C:\\Users\\linggm\\Desktop\\1.bmp", 1024, 1024); // 假设loadTexture是你的纹理加载函数
	// 重复加载其他面的纹理（后、左、右、上、下）
}

void MyGLWidget::scene_0()
{
	// 清除颜色缓冲区和深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// 设置投影矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)width() / (GLfloat)height(), 1.0, 1000.0);

	// 设置模型视图矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);

	// 禁用深度写入
	glDepthMask(GL_FALSE);

	// 绘制天空盒
	// 假设skyboxTexture[]包含了加载的天空盒纹理
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]); // 绑定前面的纹理
	// [绘制一个面的代码，使用glBegin(GL_QUADS) ... glEnd()]

	// 绑定前面的纹理
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);

	// 绘制前面
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -100.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, -100.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 100.0f, 100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, 100.0f, 100.0f);
	glEnd();

	// 绑定前面的纹理
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);

	// 绘制前面
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -100.0f, -100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, -100.0f, -100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 100.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, 100.0f, -100.0f);
	glEnd();



	// glBindTexture(GL_TEXTURE_2D, skyboxTexture[1]); // 绑定后面的纹理
	// [绘制另一个面的代码]

	// 继续为其余面绑定纹理并绘制

	// 禁用纹理
	glDisable(GL_TEXTURE_2D);

	// 恢复深度写入
	glDepthMask(GL_TRUE);

	// 绘制场景的其他部分
	// ...

	// 禁用深度测试
	glDisable(GL_DEPTH_TEST);
}


