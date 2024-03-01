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
	���ܣ�����OpenGL���ӿڣ�Viewport����
	������(0, 0)��ʾ�ӿڵ����½����꣬width()��height()��QOpenGLWidget�̳У����ش��ڵĿ�Ⱥ͸߶ȡ�
	���ã������OpenGL��Ⱦ�������С��λ�ã���������Ϊ�����������ڡ�
	*/
	glViewport(0, 0, width(), height());
	/*
	���ܣ����������Ļʱʹ�õ���ɫ��
	�������ĸ�����ֵ�ֱ�����ɫ����ɫ����ɫ��͸���ȵ�ֵ����Χ��0.0��1.0����������Ϊ��ɫ����͸����
	���ã�������glClear(GL_COLOR_BUFFER_BIT)ʱ����Ļ�ᱻ���Ϊ����ɫ��
	*/
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// ������Ȳ��ԣ���2D��Ⱦ����Ҫ��������ǰ���ϵʱ��ͨ��������Ȳ��ԣ��Ӷ������ȾЧ�ʡ�
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
	// ��������OpenGL���ӿڣ�ȷ��OpenGL��Ⱦ����Ĵ�С�봰�ڴ�Сͬ��
	glViewport(0, 0, width, height);
	// ����������widget
	update();
}



void MyGLWidget::keyPressEvent(QKeyEvent *e) {

	// ����������ӽ�
	float angleSpeed = 0.1f; // �ɵ������ӽ��ƶ��ٶ�
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
	update(); // ���������ͼ
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

GLuint skyboxTexture[6]; // �洢��պе�6���������ID

void loadSkyboxTextures() {
	// ������պе�ÿ���������
	// ����ֻ��һ��α����ʾ��������ʵ��ȡ��������δ����������
	skyboxTexture[0] = LoadTexture("C:\\Users\\linggm\\Desktop\\1.bmp", 1024, 1024); // ����loadTexture�����������غ���
	// �ظ���������������������ҡ��ϡ��£�
}

void MyGLWidget::scene_0()
{
	// �����ɫ����������Ȼ�����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// ����ͶӰ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)width() / (GLfloat)height(), 1.0, 1000.0);

	// ����ģ����ͼ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);

	// �������д��
	glDepthMask(GL_FALSE);

	// ������պ�
	// ����skyboxTexture[]�����˼��ص���պ�����
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]); // ��ǰ�������
	// [����һ����Ĵ��룬ʹ��glBegin(GL_QUADS) ... glEnd()]

	// ��ǰ�������
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);

	// ����ǰ��
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -100.0f, 100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, -100.0f, 100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 100.0f, 100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, 100.0f, 100.0f);
	glEnd();

	// ��ǰ�������
	glBindTexture(GL_TEXTURE_2D, skyboxTexture[0]);

	// ����ǰ��
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-100.0f, -100.0f, -100.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(100.0f, -100.0f, -100.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(100.0f, 100.0f, -100.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-100.0f, 100.0f, -100.0f);
	glEnd();



	// glBindTexture(GL_TEXTURE_2D, skyboxTexture[1]); // �󶨺��������
	// [������һ����Ĵ���]

	// ����Ϊ���������������

	// ��������
	glDisable(GL_TEXTURE_2D);

	// �ָ����д��
	glDepthMask(GL_TRUE);

	// ���Ƴ�������������
	// ...

	// ������Ȳ���
	glDisable(GL_DEPTH_TEST);
}


