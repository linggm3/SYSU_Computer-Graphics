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
	// 对定义的数组初始化
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


// 窗口大小变动后，需要重新生成render_buffer等数组
void MyGLWidget::resizeGL(int w, int h)
{
	resizeBuffer(w, h);
	offset = vec2(WindowSizeH / 2, WindowSizeW / 2);
	clearBuffer(render_buffer);
}



void MyGLWidget::scene_0()
{
	// 选择要加载的model
	objModel.loadModel("./objs/rock.obj");

	// 自主设置变换矩阵
	camPosition = vec3(100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.y, 100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.x, 10 + objModel.centralPoint.z);
	camLookAt = objModel.centralPoint;     // 例如，看向物体中心
	camUp = vec3(0, 1, 0);         // 上方向向量
	projMatrix = glm::perspective(radians(20.0f), 1.0f, 0.1f, 2000.0f);

	// 单一点光源，可以改为数组实现多光源
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
	// 选择要加载的model
	//objModel.loadModel("./objs/teapot_600.obj");
	//objModel.loadModel("./objs/teapot_8000.obj");
	//objModel.loadModel("./objs/rock.obj");
	//objModel.loadModel("./objs/cube.obj");
	objModel.loadModel("./objs/singleTriangle.obj");

	// 自主设置变换矩阵
	camPosition = vec3(100 * sin(degree * 3.14 / 180.0) + objModel.centralPoint.y, 100 * cos(degree * 3.14 / 180.0) + objModel.centralPoint.x, 10 + objModel.centralPoint.z);
	camLookAt = objModel.centralPoint;     // 例如，看向物体中心
	camUp = vec3(0, 1, 0);         // 上方向向量
	projMatrix = glm::perspective(radians(20.0f), 1.0f, 0.1f, 2000.0f);

	// 单一点光源，可以改为数组实现多光源
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
	// 三维顶点映射到二维平面
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

	// 将当前三角形渲染在temp_buffer中

	// HomeWork: 1、绘制三角形三边
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



	// 合并temp_buffer 到 render_buffer, 深度测试
	// 从firstChangeLine开始遍历，可以稍快
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

	// 遍历每一行
	for (int x = 0; x < WindowSizeH; x++) {
		bool inside = false;
		int start = 0;
		int end = 0;
		bool foundstart = false;
		float startDepth = 99999.0f, endDepth = 99999.0f; // 存储边界点的深度值

		// 遍历每一列
		for (int y = 1; y < WindowSizeW; y++) {
			// 检测一行中，三角形左边的边界
			if (!inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y-1] == vec3(0, 0, 0)) {
				inside = true;
				start = y;
				foundstart = true;
				startDepth = temp_z_buffer[x * WindowSizeW + y]; // 获取起始边界的深度值
			}
			// 检测一行中，三角形右边的边界
			else if (inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y-1] == vec3(0, 0, 0)) {
				end = y;
				endDepth = temp_z_buffer[x * WindowSizeW + y]; // 获取结束边界的深度值
				break; // 找到一行中的起始和结束边界
			}
		}

		// 如果找到了三角形的边界
		if (foundstart) {
			firstchangeline = firstchangeline < x ? firstchangeline : x;
	
			for (int y = start; y <= end; y++) {
				// 计算插值深度
				float depth= startDepth + (endDepth - startDepth) * ((y - start) / (end - start + 0.1));
				temp_z_buffer[x * WindowSizeW + y] = depth;
				// 填充颜色
				temp_render_buffer[x * WindowSizeW + y] = vec3(0, 255, 0);
			}
		}
	}

	return firstchangeline;
}


int MyGLWidget::edge_walking2() {
	int firstchangeline = WindowSizeH;

	// 遍历每一行
	for (int x = 0; x < WindowSizeH; x++) {
		bool inside = false;
		int start = 0;
		int end = 0;
		bool foundstart = false;
		float startDepth = 99999.0f, endDepth = 99999.0f; // 存储边界点的深度值
		vec3 startColor, endColor;

		// 遍历每一列
		for (int y = 1; y < WindowSizeW; y++) {
			// 检测一行中，三角形左边的边界
			if (!inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				inside = true;
				start = y;
				foundstart = true;
				startDepth = temp_z_buffer[x * WindowSizeW + y]; // 获取起始边界的深度值
				startColor = temp_render_buffer[x * WindowSizeW + y]; // 获取起始边界的颜色
			}
			// 检测一行中，三角形右边的边界
			else if (inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				end = y;
				endDepth = temp_z_buffer[x * WindowSizeW + y]; // 获取结束边界的深度值
				endColor = temp_render_buffer[x * WindowSizeW + y]; // 获取结束边界的颜色
				break; // 找到一行中的起始和结束边界
			}
		}

		// 如果找到了三角形的边界
		if (foundstart) {
			firstchangeline = firstchangeline < x ? firstchangeline : x;

			for (int y = start; y <= end; y++) {
				// 计算插值颜色和深度
				float t = (float)(y - start) / (float)(end - start + 0.01f);
				vec3 color = vec3(startColor.x + (endColor.x - startColor.x) * t,
								  startColor.y + (endColor.y - startColor.y) * t,
								  startColor.z + (endColor.z - startColor.z) * t);
				float depth = startDepth + (endDepth - startDepth) * (y - start) / (end - start + 0.01f);
				temp_z_buffer[x * WindowSizeW + y] = depth;
				// 填充颜色
				temp_render_buffer[x * WindowSizeW + y] = color;
			}
		}
	}

	return firstchangeline;
}


vec3 InterpolateNormal(int x, int y, FragmentAttr vertices[3]) {
	vec3 p = vec3(x, y, 0); // 将输入的x和y坐标转换为vec3

	// 计算重心坐标
	vec3 v0 = vec3(vertices[1].x - vertices[0].x, vertices[1].y - vertices[0].y, vertices[1].z - vertices[0].z);
	vec3 v1 = vec3(vertices[2].x - vertices[0].x, vertices[2].y - vertices[0].y, vertices[2].z - vertices[0].z);
	vec3 v2 = vec3(p.x - vertices[0].x, p.y - vertices[0].y, 0); // 假设z值为0，因为x和y是屏幕坐标

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

	// 通过重心坐标插值法向量
	vec3 interpolatedNormal = normalize(bary.x * vertices[0].normal + bary.y * vertices[1].normal + bary.z * vertices[2].normal);
	return interpolatedNormal;
}


vec3 InterpolatePosmv(int x, int y, FragmentAttr vertices[3]) {
	vec3 p = vec3(x, y, 0); // 将输入的x和y坐标转换为vec3

	// 计算重心坐标
	vec3 v0 = vec3(vertices[1].x - vertices[0].x, vertices[1].y - vertices[0].y, vertices[1].z - vertices[0].z);
	vec3 v1 = vec3(vertices[2].x - vertices[0].x, vertices[2].y - vertices[0].y, vertices[2].z - vertices[0].z);
	vec3 v2 = vec3(p.x - vertices[0].x, p.y - vertices[0].y, 0); // 假设z值为0，因为x和y是屏幕坐标

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

	// 通过重心坐标插值法向量
	vec3 interpolatedNormal = normalize(bary.x * vertices[0].pos_mv + bary.y * vertices[1].pos_mv + bary.z * vertices[2].pos_mv);
	return interpolatedNormal;
}



int MyGLWidget::edge_walking3(FragmentAttr transformedVertices[3]) {
	int firstchangeline = WindowSizeH;

	// 遍历每一行
	for (int x = 0; x < WindowSizeH; x++) {
		bool inside = false;
		int start = 0;
		int end = 0;
		bool foundstart = false;
		float startDepth = 99999.0f, endDepth = 99999.0f; // 存储边界点的深度值

		// 遍历每一列
		for (int y = 1; y < WindowSizeW; y++) {
			// 检测一行中，三角形左边的边界
			if (!inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				inside = true;
				start = y-1;
				foundstart = true;
				startDepth = temp_z_buffer[x * WindowSizeW + y]; // 获取起始边界的深度值
			}
			// 检测一行中，三角形右边的边界
			else if (inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				end = y+1;
				endDepth = temp_z_buffer[x * WindowSizeW + y]; // 获取结束边界的深度值
				break; // 找到一行中的起始和结束边界
			}
		}

		// 如果找到了三角形的边界
		if (foundstart) {
			firstchangeline = firstchangeline < x ? firstchangeline : x;

			for (int y = start; y <= end; y++) {
				// 计算插值颜色和深度
				FragmentAttr tmp = FragmentAttr(x, y, 0, 0);
				tmp.normal = InterpolateNormal(x, y, transformedVertices);
				tmp.pos_mv = InterpolatePosmv(x, y, transformedVertices);
				vec3 color = PhoneShading(tmp);
				float depth = startDepth + (endDepth - startDepth) * (y - start) / (end - start + 0.01f);
				temp_z_buffer[x * WindowSizeW + y] = depth;
				// 填充颜色
				temp_render_buffer[x * WindowSizeW + y] = color;
			}
		}
	}

	return firstchangeline;
}



int MyGLWidget::edge_walking4(FragmentAttr transformedVertices[3]) {
	int firstchangeline = WindowSizeH;

	// 遍历每一行
	for (int x = 0; x < WindowSizeH; x++) {
		bool inside = false;
		int start = 0;
		int end = 0;
		bool foundstart = false;
		float startDepth = 99999.0f, endDepth = 99999.0f; // 存储边界点的深度值

		// 遍历每一列
		for (int y = 1; y < WindowSizeW; y++) {
			// 检测一行中，三角形左边的边界
			if (!inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				inside = true;
				start = y - 1;
				foundstart = true;
				startDepth = temp_z_buffer[x * WindowSizeW + y]; // 获取起始边界的深度值
			}
			// 检测一行中，三角形右边的边界
			else if (inside && temp_render_buffer[x * WindowSizeW + y] != vec3(0, 0, 0) && temp_render_buffer[x * WindowSizeW + y - 1] == vec3(0, 0, 0)) {
				end = y + 1;
				endDepth = temp_z_buffer[x * WindowSizeW + y]; // 获取结束边界的深度值
				break; // 找到一行中的起始和结束边界
			}
		}

		// 如果找到了三角形的边界
		if (foundstart) {
			firstchangeline = firstchangeline < x ? firstchangeline : x;

			for (int y = start; y <= end; y++) {
				// 计算颜色和深度
				FragmentAttr tmp = FragmentAttr(x, y, 0, 0);
				tmp.normal = InterpolateNormal(x, y, transformedVertices);
				tmp.pos_mv = InterpolatePosmv(x, y, transformedVertices);
				vec3 color = BlinnPhongShading(tmp);
				float depth = startDepth + (endDepth - startDepth) * (y - start) / (end - start + 0.01f);
				temp_z_buffer[x * WindowSizeW + y] = depth;
				// 填充颜色
				temp_render_buffer[x * WindowSizeW + y] = color;
			}
		}
	}

	return firstchangeline;
}


vec3 MyGLWidget::PhoneShading(FragmentAttr& nowPixelResult) {
	vec3 ambientColor = vec3(0.1, 0.1, 0.1);  // 环境光颜色
	vec3 lightColor = vec3(1.0, 1.0, 1.0);    // 光源颜色
	vec3 materialColor = vec3(1.0, 1.0, 1.0); // 材料颜色

	// 环境光部分
	vec3 ambient = ambientColor * materialColor;

	// 漫反射部分
	vec3 norm = normalize(nowPixelResult.normal);
	vec3 lightDir = normalize(lightPosition - vec3(nowPixelResult.pos_mv));
	float diff = dot(norm, lightDir);
	if (diff < 0) {
		diff = 0.0f;
	}
	vec3 diffuse = diff * lightColor * materialColor;

    vec3 viewDir = normalize(-vec3(nowPixelResult.pos_mv));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 64); // 32为镜面反射的光泽度
    vec3 specular = spec * lightColor * materialColor;

	// 总光照强度
	vec3 intensity = ambient + diffuse + specular;

    return intensity;
}


vec3 MyGLWidget::GauraudShading(FragmentAttr& nowPixelResult) {
	vec3 ambientColor = vec3(0.1, 0.1, 0.1);  // 环境光颜色
	vec3 lightColor = vec3(1.0, 1.0, 1.0);    // 光源颜色
	vec3 materialColor = vec3(1.0, 1.0, 1.0); // 材料颜色

	// 环境光部分
	vec3 ambient = ambientColor * materialColor;

	// 漫反射部分
	vec3 norm = normalize(nowPixelResult.normal);
	vec3 lightDir = normalize(lightPosition - vec3(nowPixelResult.pos_mv));
	float diff = dot(norm, lightDir);
	if (diff < 0) {
		diff = 0.0f;
	}
	vec3 diffuse = diff * lightColor * materialColor;

	// 总光照强度
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
	int error = dx + dy;  // error值是两倍的真实error
	int e2;  // error的临时变量

	FragmentAttr nowV = start; // 初始点

	// 计算总步数
	int totalSteps = max(abs(end.x - start.x), abs(end.y - start.y));
	int step = 0; // 当前步数

	while (true) {
		// 计算插值深度
		float depth = start.z + (end.z - start.z) * (step / (float)totalSteps);

		// 将像素渲染到temp_render_buffer，并更新temp_z_buffer
		temp_render_buffer[nowV.x * WindowSizeW + nowV.y] = vec3(255, 255, 255);  // 渲染像素颜色
		temp_z_buffer[nowV.x * WindowSizeW + nowV.y] = depth;  // 更新深度信息

		// 检查是否到达终点
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

		step++; // 增加步数
	}

}


void MyGLWidget::bresenham2(FragmentAttr& start, FragmentAttr& end, int id) {
	int dx = abs(end.x - start.x);
	int dy = -abs(end.y - start.y);
	int sx = start.x < end.x ? 1 : -1;
	int sy = start.y < end.y ? 1 : -1;
	int error = dx + dy;  // error值是两倍的真实error
	int e2;  // error的临时变量

	vec3 start_color = GauraudShading(start);
	vec3 end_color = GauraudShading(end);
	start.color = start_color;
	end.color = end_color;
	temp_render_buffer[start.x * WindowSizeW + start.y] = start_color;
	temp_render_buffer[end.x * WindowSizeW + end.y] = end_color;

	FragmentAttr nowV = start; // 初始点

	// 计算总步数
	int totalSteps = max(abs(end.x - start.x), abs(end.y - start.y));
	int step = 0; // 当前步数

	while (true) {
		// 计算插值深度
		float depth = start.z + (end.z - start.z) * (step / (float)totalSteps);
		vec3 color = start_color + (end_color - start_color) * (step / (float)totalSteps);
		if (color == vec3(0, 0, 0)) {
			color = vec3(0, 1, 0);
		}

		// 将像素渲染到temp_render_buffer，并更新temp_z_buffer
		temp_render_buffer[nowV.x * WindowSizeW + nowV.y] = color;  // 渲染像素颜色
		temp_z_buffer[nowV.x * WindowSizeW + nowV.y] = depth;  // 更新深度信息

		// 检查是否到达终点
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

		step++; // 增加步数
	}
	
}


void MyGLWidget::bresenham3(FragmentAttr& start, FragmentAttr& end, int id) {
	int dx = abs(end.x - start.x);
	int dy = -abs(end.y - start.y);
	int sx = start.x < end.x ? 1 : -1;
	int sy = start.y < end.y ? 1 : -1;
	int error = dx + dy;  // error值是两倍的真实error
	int e2;  // error的临时变量

	vec3 start_color = PhoneShading(start);
	vec3 end_color = PhoneShading(end);
	start.color = start_color;
	end.color = end_color;
	temp_render_buffer[start.x * WindowSizeW + start.y] = start_color;
	temp_render_buffer[end.x * WindowSizeW + end.y] = end_color;

	FragmentAttr nowV = start; // 初始点

	// 计算总步数
	int totalSteps = max(abs(end.x - start.x), abs(end.y - start.y));
	int step = 0; // 当前步数

	while (true) {
		// 计算插值深度
		float depth = start.z + (end.z - start.z) * (step / (float)totalSteps);
		vec3 color = start_color + (end_color - start_color) * (step / (float)totalSteps);
		if (color == vec3(0, 0, 0)) {
			color = vec3(0, 1, 0);
		}

		// 将像素渲染到temp_render_buffer，并更新temp_z_buffer
		temp_render_buffer[nowV.x * WindowSizeW + nowV.y] = color;  // 渲染像素颜色
		temp_z_buffer[nowV.x * WindowSizeW + nowV.y] = depth;  // 更新深度信息

		// 检查是否到达终点
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

		step++; // 增加步数
	}

}


void MyGLWidget::bresenham4(FragmentAttr& start, FragmentAttr& end, int id) {
	int dx = abs(end.x - start.x);
	int dy = -abs(end.y - start.y);
	int sx = start.x < end.x ? 1 : -1;
	int sy = start.y < end.y ? 1 : -1;
	int error = dx + dy;  // error值是两倍的真实error
	int e2;  // error的临时变量

	vec3 start_color = BlinnPhongShading(start);
	vec3 end_color = BlinnPhongShading(end);
	start.color = start_color;
	end.color = end_color;
	temp_render_buffer[start.x * WindowSizeW + start.y] = start_color;
	temp_render_buffer[end.x * WindowSizeW + end.y] = end_color;

	FragmentAttr nowV = start; // 初始点

	// 计算总步数
	int totalSteps = max(abs(end.x - start.x), abs(end.y - start.y));
	int step = 0; // 当前步数

	while (true) {
		// 计算插值深度 
		float depth = start.z + (end.z - start.z) * (step / (float)totalSteps);
		// 这里为边赋值其实只是为了后面的处理
		vec3 color = start_color + (end_color - start_color) * (step / (float)totalSteps);
		if (color == vec3(0, 0, 0)) {
			color = vec3(0, 1, 0);
		}

		// 将像素渲染到temp_render_buffer，并更新temp_z_buffer
		temp_render_buffer[nowV.x * WindowSizeW + nowV.y] = color;  // 渲染像素颜色
		temp_z_buffer[nowV.x * WindowSizeW + nowV.y] = depth;  // 更新深度信息

		// 检查是否到达终点
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

		step++; // 增加步数
	}

}


void MyGLWidget::DDA(FragmentAttr& start, FragmentAttr& end, int id) {
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy); // 步骤数基于最大的dx或dy

	float xIncrement = dx / (float)steps;
	float yIncrement = dy / (float)steps;

	float x = start.x;
	float y = start.y;

	for (int i = 0; i <= steps; i++) {
		temp_render_buffer[int(x) * WindowSizeW + int(y)] = vec3(255, 255, 255); // 绘制像素
		temp_z_buffer[int(x) * WindowSizeW + int(y)] = start.z + (end.z - start.z) * (i / (float)steps); // 计算并更新深度信息
		x += xIncrement;
		y += yIncrement;
	}
}



