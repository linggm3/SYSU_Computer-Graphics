#include "myglwidget.h"
#include <GL/glew.h>
#include <algorithm>

MyGLWidget::MyGLWidget(QWidget* parent)
	:QOpenGLWidget(parent)
{
}

MyGLWidget::~MyGLWidget()
{
	delete[] render_buffer;
	delete[] temp_render_buffer;
	delete[] temp_z_buffer;
	delete[] z_buffer;
}

void MyGLWidget::resizeBuffer(int newW, int newH) {
	delete[] render_buffer;
	delete[] temp_render_buffer;
	delete[] temp_z_buffer;
	delete[] z_buffer;
	WindowSizeW = newW;
	WindowSizeH = newH;
	render_buffer = new vec3[WindowSizeH * WindowSizeW];
	temp_render_buffer = new vec3[WindowSizeH * WindowSizeW];
	temp_z_buffer = new float[WindowSizeH * WindowSizeW];
	z_buffer = new float[WindowSizeH * WindowSizeW];
}

void MyGLWidget::initializeGL()
{
	WindowSizeW = width();
	WindowSizeH = height();
	glViewport(0, 0, WindowSizeW, WindowSizeH);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	offset = vec2(WindowSizeH / 2, WindowSizeW / 2);
	// �Զ���������ʼ��
	render_buffer = new vec3[WindowSizeH * WindowSizeW];
	temp_render_buffer = new vec3[WindowSizeH * WindowSizeW];
	temp_z_buffer = new float[WindowSizeH * WindowSizeW];
	z_buffer = new float[WindowSizeH * WindowSizeW];
	for (int i = 0; i < WindowSizeH * WindowSizeW; i++) {
		render_buffer[i] = vec3(0, 0, 0);
		temp_render_buffer[i] = vec3(0, 0, 0);
		temp_z_buffer[i] = MAX_Z_BUFFER;
		z_buffer[i] = MAX_Z_BUFFER;
	}
}

void MyGLWidget::keyPressEvent(QKeyEvent* e) {

	switch (e->key()) {
	case Qt::Key_0: scene_id = 0; update(); break;
	case Qt::Key_1: scene_id = 1; update(); break;
	case Qt::Key_2: draw_id = 2;  update(); break;
	case Qt::Key_3: draw_id = 3;  update(); break;
	case Qt::Key_4: draw_id = 4;  update(); break;
	case Qt::Key_5: draw_id = 5;  update(); break;
	case Qt::Key_6: draw_id = 6;  update(); break;
	case Qt::Key_9: degree += 35; update(); break;
	}
}

void MyGLWidget::paintGL()
{
	switch (scene_id) {
	case 0:scene_0(); break;
	case 1:scene_1(); break;
	}
}
void MyGLWidget::clearBuffer(vec3* now_buffer) {
	for (int i = 0; i < WindowSizeH * WindowSizeW; i++) {
		now_buffer[i] = vec3(0, 0, 0);
	}
}

void MyGLWidget::clearBuffer(int* now_buffer) {
	memset(now_buffer, 0, WindowSizeW * WindowSizeH * sizeof(int));
}


void MyGLWidget::clearZBuffer(float* now_buffer) {
	std::fill(now_buffer, now_buffer + WindowSizeW * WindowSizeH, MAX_Z_BUFFER);
}


// ���ڴ�С�䶯����Ҫ��������render_buffer������
void MyGLWidget::resizeGL(int w, int h)
{
	resizeBuffer(w, h);
	offset = vec2(WindowSizeH / 2, WindowSizeW / 2);
	clearBuffer(render_buffer);
}



void MyGLWidget::scene_0()
{
	// ѡ��Ҫ���ص�model
	objModel.loadModel("./objs/rock.obj");

	// �������ñ任����
	camPosition = vec3(100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.y, 100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.x, 10 + objModel.centralPoint.z);
	camLookAt = objModel.centralPoint;     // ���磬������������
	camUp = vec3(0, 1, 0);         // �Ϸ�������
	projMatrix = glm::perspective(radians(20.0f), 1.0f, 0.1f, 2000.0f);

	// ��һ���Դ�����Ը�Ϊ����ʵ�ֶ��Դ
	lightPosition = objModel.centralPoint + vec3(0, 100, 100);
	clearBuffer(render_buffer);
	clearZBuffer(z_buffer);
	for (int i = 0; i < objModel.triangleCount; i++) {
		Triangle nowTriangle = objModel.getTriangleByID(i);
		drawTriangle(nowTriangle);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	renderWithTexture(render_buffer, WindowSizeH, WindowSizeW);
}


void MyGLWidget::scene_1()
{
	// ѡ��Ҫ���ص�model
	//objModel.loadModel("./objs/teapot_600.obj");
	//objModel.loadModel("./objs/teapot_8000.obj");
	//objModel.loadModel("./objs/rock.obj");
	//objModel.loadModel("./objs/cube.obj");
	objModel.loadModel("./objs/singleTriangle.obj");

	// �������ñ任����
	camPosition = vec3(100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.y, 100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.x, 10 + objModel.centralPoint.z);
	camLookAt = objModel.centralPoint;     // ���磬������������
	camUp = vec3(0, 1, 0);         // �Ϸ�������
	projMatrix = glm::perspective(radians(20.0f), 1.0f, 0.1f, 2000.0f);

	// ��һ���Դ�����Ը�Ϊ����ʵ�ֶ��Դ
	lightPosition = objModel.centralPoint + vec3(0, 100, 100);
	clearBuffer(render_buffer);
	clearZBuffer(z_buffer);
	for (int i = 0; i < objModel.triangleCount; i++) {
		Triangle nowTriangle = objModel.getTriangleByID(i);
		drawTriangle(nowTriangle);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	renderWithTexture(render_buffer, WindowSizeH, WindowSizeW);
}

void MyGLWidget::drawTriangle(Triangle triangle) {
	// ��ά����ӳ�䵽��άƽ��
	vec3* vertices = triangle.triangleVertices;
	vec3* normals = triangle.triangleNormals;
	FragmentAttr transformedVertices[3];
	clearBuffer(this->temp_render_buffer);
	clearZBuffer(this->temp_z_buffer);
	mat4 viewMatrix = glm::lookAt(camPosition, camLookAt, camUp);

	for (int i = 0; i < 3; ++i) {
		vec4 ver_mv = viewMatrix * vec4(vertices[i], 1.0f);
		float nowz = glm::length(camPosition - vec3(ver_mv));
		vec4 ver_proj = projMatrix * ver_mv;
		transformedVertices[i].x = ver_proj.x + offset.x;
		transformedVertices[i].y = ver_proj.y + offset.y;
		transformedVertices[i].z = nowz;
		transformedVertices[i].pos_mv = ver_mv;
		mat3 normalMatrix = mat3(viewMatrix);
		vec3 normal_mv = normalMatrix * normals[i];
		transformedVertices[i].normal = normal_mv;
	}

	// ����ǰ��������Ⱦ��temp_buffer��

	// HomeWork: 1����������������
	int firstChangeLine = 0;
	if (draw_id == 2) {
		DDA(transformedVertices[0], transformedVertices[1], 1);
		DDA(transformedVertices[1], transformedVertices[2], 2);
		DDA(transformedVertices[2], transformedVertices[0], 3);
		firstChangeLine = edge_walking();
	}
	else if(draw_id == 3) {
		bresenham(transformedVertices[0], transformedVertices[1], 1);
		bresenham(transformedVertices[1], transformedVertices[2], 2);
		bresenham(transformedVertices[2], transformedVertices[0], 3);
		firstChangeLine = edge_walking();
	}
	else if (draw_id == 4) {
		bresenham2(transformedVertices[0], transformedVertices[1], 1);
		bresenham2(transformedVertices[1], transformedVertices[2], 2);
		bresenham2(transformedVertices[2], transformedVertices[0], 3);
		firstChangeLine = edge_walking2();
	}
	else if (draw_id == 5) {
		bresenham3(transformedVertices[0], transformedVertices[1], 1);
		bresenham3(transformedVertices[1], transformedVertices[2], 2);
		bresenham3(transformedVertices[2], transformedVertices[0], 3);
		firstChangeLine = edge_walking3(transformedVertices);
	}
	else if (draw_id == 6) {
		bresenham4(transformedVertices[0], transformedVertices[1], 1);
		bresenham4(transformedVertices[1], transformedVertices[2], 2);
		bresenham4(transformedVertices[2], transformedVertices[0], 3);
		firstChangeLine = edge_walking4(transformedVertices);
	}
	else {
		firstChangeLine = 0;
	}



	// �ϲ�temp_buffer �� render_buffer, ��Ȳ���
	// ��firstChangeLine��ʼ�����������Կ�
	for (int h = firstChangeLine; h < WindowSizeH; h++) {
		auto render_row = &render_buffer[h * WindowSizeW];
		auto temp_render_row = &temp_render_buffer[h * WindowSizeW];
		auto z_buffer_row = &z_buffer[h * WindowSizeW];
		auto temp_z_buffer_row = &temp_z_buffer[h * WindowSizeW];
		for (int i = 0; i < WindowSizeW; i++) {
			if (z_buffer_row[i] < temp_z_buffer_row[i])
				continue;
			else
			{
				z_buffer_row[i] = temp_z_buffer_row[i];
				render_row[i] = temp_render_row[i];
			}
		}

	}
}


int MyGLWidget::edge_walking() {
	int firstchangeline = WindowSizeH;

	// ����ÿһ��
	for (int x = 0; x < WindowSizeH; x++) {
		bool inside = false;
		int start = 0;
		int end = 0;
		bool foundstart = false;
		float startDepth = 99999.0f, endDepth = 99999.0f; // �洢�߽������ֵ

		// ����ÿһ��
		for (int y = 1; y < WindowSizeW; y++) {
			// ���һ���У���������ߵı߽�
			if (!inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y-1] == vec3(0, 0, 0)) {
				inside = true;
				start = y;
				foundstart = true;
				startDepth = temp_z_buffer[x * WindowSizeW + y]; // ��ȡ��ʼ�߽�����ֵ
			}
			// ���һ���У��������ұߵı߽�
			else if (inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y-1] == vec3(0, 0, 0)) {
				end = y;
				endDepth = temp_z_buffer[x * WindowSizeW + y]; // ��ȡ�����߽�����ֵ
				break; // �ҵ�һ���е���ʼ�ͽ����߽�
			}
		}

		// ����ҵ��������εı߽�
		if (foundstart) {
			firstchangeline = firstchangeline < x ? firstchangeline : x;
	
			for (int y = start; y <= end; y++) {
				// �����ֵ���
				float depth= startDepth + (endDepth - startDepth) * ((y - start) / (end - start + 0.1));
				temp_z_buffer[x * WindowSizeW + y] = depth;
				// �����ɫ
				temp_render_buffer[x * WindowSizeW + y] = vec3(0, 255, 0);
			}
		}
	}

	return firstchangeline;
}


int MyGLWidget::edge_walking2() {
	int firstchangeline = WindowSizeH;

	// ����ÿһ��
	for (int x = 0; x < WindowSizeH; x++) {
		bool inside = false;
		int start = 0;
		int end = 0;
		bool foundstart = false;
		float startDepth = 99999.0f, endDepth = 99999.0f; // �洢�߽������ֵ
		vec3 startColor, endColor;

		// ����ÿһ��
		for (int y = 1; y < WindowSizeW; y++) {
			// ���һ���У���������ߵı߽�
			if (!inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				inside = true;
				start = y;
				foundstart = true;
				startDepth = temp_z_buffer[x * WindowSizeW + y]; // ��ȡ��ʼ�߽�����ֵ
				startColor = temp_render_buffer[x * WindowSizeW + y]; // ��ȡ��ʼ�߽����ɫ
			}
			// ���һ���У��������ұߵı߽�
			else if (inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				end = y;
				endDepth = temp_z_buffer[x * WindowSizeW + y]; // ��ȡ�����߽�����ֵ
				endColor = temp_render_buffer[x * WindowSizeW + y]; // ��ȡ�����߽����ɫ
				break; // �ҵ�һ���е���ʼ�ͽ����߽�
			}
		}

		// ����ҵ��������εı߽�
		if (foundstart) {
			firstchangeline = firstchangeline < x ? firstchangeline : x;

			for (int y = start; y <= end; y++) {
				// �����ֵ��ɫ�����
				float t = (float)(y - start) / (float)(end - start + 0.01f);
				vec3 color = vec3(startColor.x + (endColor.x - startColor.x) * t,
								  startColor.y + (endColor.y - startColor.y) * t,
								  startColor.z + (endColor.z - startColor.z) * t);
				float depth = startDepth + (endDepth - startDepth) * (y - start) / (end - start + 0.01f);
				temp_z_buffer[x * WindowSizeW + y] = depth;
				// �����ɫ
				temp_render_buffer[x * WindowSizeW + y] = color;
			}
		}
	}

	return firstchangeline;
}


vec3 InterpolateNormal(int x, int y, FragmentAttr vertices[3]) {
	vec3 p = vec3(x, y, 0); // �������x��y����ת��Ϊvec3

	// ������������
	vec3 v0 = vec3(vertices[1].x - vertices[0].x, vertices[1].y - vertices[0].y, vertices[1].z - vertices[0].z);
	vec3 v1 = vec3(vertices[2].x - vertices[0].x, vertices[2].y - vertices[0].y, vertices[2].z - vertices[0].z);
	vec3 v2 = vec3(p.x - vertices[0].x, p.y - vertices[0].y, 0); // ����zֵΪ0����Ϊx��y����Ļ����

	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;

	vec3 bary;
	bary.y = (d11 * d20 - d01 * d21) / denom;
	bary.z = (d00 * d21 - d01 * d20) / denom;
	bary.x = 1.0f - bary.y - bary.z;

	// ͨ�����������ֵ������
	vec3 interpolatedNormal = normalize(bary.x * vertices[0].normal + bary.y * vertices[1].normal + bary.z * vertices[2].normal);
	return interpolatedNormal;
}


vec3 InterpolatePosmv(int x, int y, FragmentAttr vertices[3]) {
	vec3 p = vec3(x, y, 0); // �������x��y����ת��Ϊvec3

	// ������������
	vec3 v0 = vec3(vertices[1].x - vertices[0].x, vertices[1].y - vertices[0].y, vertices[1].z - vertices[0].z);
	vec3 v1 = vec3(vertices[2].x - vertices[0].x, vertices[2].y - vertices[0].y, vertices[2].z - vertices[0].z);
	vec3 v2 = vec3(p.x - vertices[0].x, p.y - vertices[0].y, 0); // ����zֵΪ0����Ϊx��y����Ļ����

	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;

	vec3 bary;
	bary.y = (d11 * d20 - d01 * d21) / denom;
	bary.z = (d00 * d21 - d01 * d20) / denom;
	bary.x = 1.0f - bary.y - bary.z;

	// ͨ�����������ֵ������
	vec3 interpolatedNormal = normalize(bary.x * vertices[0].pos_mv + bary.y * vertices[1].pos_mv + bary.z * vertices[2].pos_mv);
	return interpolatedNormal;
}



int MyGLWidget::edge_walking3(FragmentAttr transformedVertices[3]) {
	int firstchangeline = WindowSizeH;

	// ����ÿһ��
	for (int x = 0; x < WindowSizeH; x++) {
		bool inside = false;
		int start = 0;
		int end = 0;
		bool foundstart = false;
		float startDepth = 99999.0f, endDepth = 99999.0f; // �洢�߽������ֵ

		// ����ÿһ��
		for (int y = 1; y < WindowSizeW; y++) {
			// ���һ���У���������ߵı߽�
			if (!inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				inside = true;
				start = y-1;
				foundstart = true;
				startDepth = temp_z_buffer[x * WindowSizeW + y]; // ��ȡ��ʼ�߽�����ֵ
			}
			// ���һ���У��������ұߵı߽�
			else if (inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				end = y+1;
				endDepth = temp_z_buffer[x * WindowSizeW + y]; // ��ȡ�����߽�����ֵ
				break; // �ҵ�һ���е���ʼ�ͽ����߽�
			}
		}

		// ����ҵ��������εı߽�
		if (foundstart) {
			firstchangeline = firstchangeline < x ? firstchangeline : x;

			for (int y = start; y <= end; y++) {
				// �����ֵ��ɫ�����
				FragmentAttr tmp = FragmentAttr(x, y, 0, 0);
				tmp.normal = InterpolateNormal(x, y, transformedVertices);
				tmp.pos_mv = InterpolatePosmv(x, y, transformedVertices);
				vec3 color = PhoneShading(tmp);
				float depth = startDepth + (endDepth - startDepth) * (y - start) / (end - start + 0.01f);
				temp_z_buffer[x * WindowSizeW + y] = depth;
				// �����ɫ
				temp_render_buffer[x * WindowSizeW + y] = color;
			}
		}
	}

	return firstchangeline;
}



int MyGLWidget::edge_walking4(FragmentAttr transformedVertices[3]) {
	int firstchangeline = WindowSizeH;

	// ����ÿһ��
	for (int x = 0; x < WindowSizeH; x++) {
		bool inside = false;
		int start = 0;
		int end = 0;
		bool foundstart = false;
		float startDepth = 99999.0f, endDepth = 99999.0f; // �洢�߽������ֵ

		// ����ÿһ��
		for (int y = 1; y < WindowSizeW; y++) {
			// ���һ���У���������ߵı߽�
			if (!inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				inside = true;
				start = y - 1;
				foundstart = true;
				startDepth = temp_z_buffer[x * WindowSizeW + y]; // ��ȡ��ʼ�߽�����ֵ
			}
			// ���һ���У��������ұߵı߽�
			else if (inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				end = y + 1;
				endDepth = temp_z_buffer[x * WindowSizeW + y]; // ��ȡ�����߽�����ֵ
				break; // �ҵ�һ���е���ʼ�ͽ����߽�
			}
		}

		// ����ҵ��������εı߽�
		if (foundstart) {
			firstchangeline = firstchangeline < x ? firstchangeline : x;

			for (int y = start; y <= end; y++) {
				// ������ɫ�����
				FragmentAttr tmp = FragmentAttr(x, y, 0, 0);
				tmp.normal = InterpolateNormal(x, y, transformedVertices);
				tmp.pos_mv = InterpolatePosmv(x, y, transformedVertices);
				vec3 color = BlinnPhongShading(tmp);
				float depth = startDepth + (endDepth - startDepth) * (y - start) / (end - start + 0.01f);
				temp_z_buffer[x * WindowSizeW + y] = depth;
				// �����ɫ
				temp_render_buffer[x * WindowSizeW + y] = color;
			}
		}
	}

	return firstchangeline;
}


vec3 MyGLWidget::PhoneShading(FragmentAttr& nowPixelResult) {
	vec3 ambientColor = vec3(0.1, 0.1, 0.1);  // ��������ɫ
	vec3 lightColor = vec3(1.0, 1.0, 1.0);    // ��Դ��ɫ
	vec3 materialColor = vec3(1.0, 1.0, 1.0); // ������ɫ

	// �����ⲿ��
	vec3 ambient = ambientColor * materialColor;

	// �����䲿��
	vec3 norm = normalize(nowPixelResult.normal);
	vec3 lightDir = normalize(lightPosition - vec3(nowPixelResult.pos_mv));
	float diff = dot(norm, lightDir);
	if (diff < 0) {
		diff = 0.0f;
	}
	vec3 diffuse = diff * lightColor * materialColor;

    vec3 viewDir = normalize(-vec3(nowPixelResult.pos_mv));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64); // 32Ϊ���淴��Ĺ����
    vec3 specular = spec * lightColor * materialColor;

	// �ܹ���ǿ��
	vec3 intensity = ambient + diffuse + specular;

    return intensity;
}


vec3 MyGLWidget::GauraudShading(FragmentAttr& nowPixelResult) {
	vec3 ambientColor = vec3(0.1, 0.1, 0.1);  // ��������ɫ
	vec3 lightColor = vec3(1.0, 1.0, 1.0);    // ��Դ��ɫ
	vec3 materialColor = vec3(1.0, 1.0, 1.0); // ������ɫ

	// �����ⲿ��
	vec3 ambient = ambientColor * materialColor;

	// �����䲿��
	vec3 norm = normalize(nowPixelResult.normal);
	vec3 lightDir = normalize(lightPosition - vec3(nowPixelResult.pos_mv));
	float diff = dot(norm, lightDir);
	if (diff < 0) {
		diff = 0.0f;
	}
	vec3 diffuse = diff * lightColor * materialColor;

	// �ܹ���ǿ��
	vec3 intensity = ambient + diffuse;

	return intensity;
}


vec3 MyGLWidget::BlinnPhongShading(FragmentAttr& nowPixelResult) {
	vec3 ambientColor = vec3(0.1, 0.1, 0.1);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 materialColor = vec3(1.0, 1.0, 1.0);
	vec3 norm = normalize(nowPixelResult.normal);
	vec3 lightDir = normalize(lightPosition - vec3(nowPixelResult.pos_mv));
	vec3 viewDir = normalize(-vec3(nowPixelResult.pos_mv));
	vec3 halfDir = normalize(lightDir + viewDir);
	float diff = max(dot(norm, lightDir), 0.0f);
	float spec = pow(max(dot(norm, halfDir), 0.0f), 32);

	vec3 ambient = ambientColor * materialColor;
	vec3 diffuse = diff * lightColor * materialColor;
	vec3 specular = spec * lightColor;

	return ambient + diffuse + specular;
}


void MyGLWidget::bresenham(FragmentAttr& start, FragmentAttr& end, int id) {
	int dx = abs(end.x - start.x);
	int dy = -abs(end.y - start.y);
	int sx = start.x < end.x ? 1 : -1;
	int sy = start.y < end.y ? 1 : -1;
	int error = dx + dy;  // errorֵ����������ʵerror
	int e2;  // error����ʱ����

	FragmentAttr nowV = start; // ��ʼ��

	// �����ܲ���
	int totalSteps = max(abs(end.x - start.x), abs(end.y - start.y));
	int step = 0; // ��ǰ����

	while (true) {
		// �����ֵ���
		float depth = start.z + (end.z - start.z) * (step / (float)totalSteps);

		// ��������Ⱦ��temp_render_buffer��������temp_z_buffer
		temp_render_buffer[nowV.x * WindowSizeW + nowV.y] = vec3(255, 255, 255);  // ��Ⱦ������ɫ
		temp_z_buffer[nowV.x * WindowSizeW + nowV.y] = depth;  // ���������Ϣ

		// ����Ƿ񵽴��յ�
		if (nowV.x == end.x && nowV.y == end.y) break;

		e2 = 2 * error;
		if (e2 >= dy) {  // e_xy + e_x > 0
			if (nowV.x == end.x) break;
			error += dy;
			nowV.x += sx;
		}
		if (e2 <= dx) {  // e_xy + e_y < 0
			if (nowV.y == end.y) break;
			error += dx;
			nowV.y += sy;
		}

		step++; // ���Ӳ���
	}

}


void MyGLWidget::bresenham2(FragmentAttr& start, FragmentAttr& end, int id) {
	int dx = abs(end.x - start.x);
	int dy = -abs(end.y - start.y);
	int sx = start.x < end.x ? 1 : -1;
	int sy = start.y < end.y ? 1 : -1;
	int error = dx + dy;  // errorֵ����������ʵerror
	int e2;  // error����ʱ����

	vec3 start_color = GauraudShading(start);
	vec3 end_color = GauraudShading(end);
	start.color = start_color;
	end.color = end_color;
	temp_render_buffer[start.x * WindowSizeW + start.y] = start_color;
	temp_render_buffer[end.x * WindowSizeW + end.y] = end_color;

	FragmentAttr nowV = start; // ��ʼ��

	// �����ܲ���
	int totalSteps = max(abs(end.x - start.x), abs(end.y - start.y));
	int step = 0; // ��ǰ����

	while (true) {
		// �����ֵ���
		float depth = start.z + (end.z - start.z) * (step / (float)totalSteps);
		vec3 color = start_color + (end_color - start_color) * (step / (float)totalSteps);
		if (color == vec3(0, 0, 0)) {
			color = vec3(0, 1, 0);
		}

		// ��������Ⱦ��temp_render_buffer��������temp_z_buffer
		temp_render_buffer[nowV.x * WindowSizeW + nowV.y] = color;  // ��Ⱦ������ɫ
		temp_z_buffer[nowV.x * WindowSizeW + nowV.y] = depth;  // ���������Ϣ

		// ����Ƿ񵽴��յ�
		if (nowV.x == end.x && nowV.y == end.y) break;

		e2 = 2 * error;
		if (e2 >= dy) {  // e_xy + e_x > 0
			if (nowV.x == end.x) break;
			error += dy;
			nowV.x += sx;
		}
		if (e2 <= dx) {  // e_xy + e_y < 0
			if (nowV.y == end.y) break;
			error += dx;
			nowV.y += sy;
		}

		step++; // ���Ӳ���
	}
	
}


void MyGLWidget::bresenham3(FragmentAttr& start, FragmentAttr& end, int id) {
	int dx = abs(end.x - start.x);
	int dy = -abs(end.y - start.y);
	int sx = start.x < end.x ? 1 : -1;
	int sy = start.y < end.y ? 1 : -1;
	int error = dx + dy;  // errorֵ����������ʵerror
	int e2;  // error����ʱ����

	vec3 start_color = PhoneShading(start);
	vec3 end_color = PhoneShading(end);
	start.color = start_color;
	end.color = end_color;
	temp_render_buffer[start.x * WindowSizeW + start.y] = start_color;
	temp_render_buffer[end.x * WindowSizeW + end.y] = end_color;

	FragmentAttr nowV = start; // ��ʼ��

	// �����ܲ���
	int totalSteps = max(abs(end.x - start.x), abs(end.y - start.y));
	int step = 0; // ��ǰ����

	while (true) {
		// �����ֵ���
		float depth = start.z + (end.z - start.z) * (step / (float)totalSteps);
		vec3 color = start_color + (end_color - start_color) * (step / (float)totalSteps);
		if (color == vec3(0, 0, 0)) {
			color = vec3(0, 1, 0);
		}

		// ��������Ⱦ��temp_render_buffer��������temp_z_buffer
		temp_render_buffer[nowV.x * WindowSizeW + nowV.y] = color;  // ��Ⱦ������ɫ
		temp_z_buffer[nowV.x * WindowSizeW + nowV.y] = depth;  // ���������Ϣ

		// ����Ƿ񵽴��յ�
		if (nowV.x == end.x && nowV.y == end.y) break;

		e2 = 2 * error;
		if (e2 >= dy) {  // e_xy + e_x > 0
			if (nowV.x == end.x) break;
			error += dy;
			nowV.x += sx;
		}
		if (e2 <= dx) {  // e_xy + e_y < 0
			if (nowV.y == end.y) break;
			error += dx;
			nowV.y += sy;
		}

		step++; // ���Ӳ���
	}

}


void MyGLWidget::bresenham4(FragmentAttr& start, FragmentAttr& end, int id) {
	int dx = abs(end.x - start.x);
	int dy = -abs(end.y - start.y);
	int sx = start.x < end.x ? 1 : -1;
	int sy = start.y < end.y ? 1 : -1;
	int error = dx + dy;  // errorֵ����������ʵerror
	int e2;  // error����ʱ����

	vec3 start_color = BlinnPhongShading(start);
	vec3 end_color = BlinnPhongShading(end);
	start.color = start_color;
	end.color = end_color;
	temp_render_buffer[start.x * WindowSizeW + start.y] = start_color;
	temp_render_buffer[end.x * WindowSizeW + end.y] = end_color;

	FragmentAttr nowV = start; // ��ʼ��

	// �����ܲ���
	int totalSteps = max(abs(end.x - start.x), abs(end.y - start.y));
	int step = 0; // ��ǰ����

	while (true) {
		// �����ֵ��� 
		float depth = start.z + (end.z - start.z) * (step / (float)totalSteps);
		// ����Ϊ�߸�ֵ��ʵֻ��Ϊ�˺���Ĵ���
		vec3 color = start_color + (end_color - start_color) * (step / (float)totalSteps);
		if (color == vec3(0, 0, 0)) {
			color = vec3(0, 1, 0);
		}

		// ��������Ⱦ��temp_render_buffer��������temp_z_buffer
		temp_render_buffer[nowV.x * WindowSizeW + nowV.y] = color;  // ��Ⱦ������ɫ
		temp_z_buffer[nowV.x * WindowSizeW + nowV.y] = depth;  // ���������Ϣ

		// ����Ƿ񵽴��յ�
		if (nowV.x == end.x && nowV.y == end.y) break;

		e2 = 2 * error;
		if (e2 >= dy) {  // e_xy + e_x > 0
			if (nowV.x == end.x) break;
			error += dy;
			nowV.x += sx;
		}
		if (e2 <= dx) {  // e_xy + e_y < 0
			if (nowV.y == end.y) break;
			error += dx;
			nowV.y += sy;
		}

		step++; // ���Ӳ���
	}

}


void MyGLWidget::DDA(FragmentAttr& start, FragmentAttr& end, int id) {
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy); // ��������������dx��dy

	float xIncrement = dx / (float)steps;
	float yIncrement = dy / (float)steps;

	float x = start.x;
	float y = start.y;

	for (int i = 0; i <= steps; i++) {
		temp_render_buffer[int(x) * WindowSizeW + int(y)] = vec3(255, 255, 255); // ��������
		temp_z_buffer[int(x) * WindowSizeW + int(y)] = start.z + (end.z - start.z) * (i / (float)steps); // ���㲢���������Ϣ
		x += xIncrement;
		y += yIncrement;
	}
}



