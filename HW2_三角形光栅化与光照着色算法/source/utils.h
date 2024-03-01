#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <assert.h>
#include <cmath>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

#define MAX_VERTICES_COT 8000
#define MAX_TRIANGLE_COT 8000

using namespace glm;

struct FragmentAttr {
    int x;
    int y;
    float z;
    int edgeID;
    vec3 color = vec3(1.0f, 1.0f, 1.0f);
    vec3 normal;
    vec3 pos_mv;
    FragmentAttr() {}
    FragmentAttr(int xx, int yy, float zz, int in_edgeID) :x(xx), y(yy), z(zz), edgeID(in_edgeID) {
    }
};


struct Triangle {
    vec3 triangleVertices[3];
    vec3 triangleNormals[3];
};

struct Model {

    int triangleCount;
    int verticesCount;
    int normalCot;
    vec3 centralPoint;
    vec3 vertices_data[MAX_VERTICES_COT];
    int** triangles;
    int** triangle_normals;
    vec3 normals_data[MAX_VERTICES_COT];
    std::string loadedPath = "";

    Model() {
        triangles = new int* [MAX_TRIANGLE_COT];
        for (int i = 0; i < MAX_TRIANGLE_COT; ++i) {
            triangles[i] = new int[3];
        }
        triangle_normals = new int* [MAX_TRIANGLE_COT];
        for (int i = 0; i < MAX_TRIANGLE_COT; ++i) {
            triangle_normals[i] = new int[3];
        }
    }

    Triangle getTriangleByID(int id) {
        assert(id < triangleCount);
        int* nowTirVerIDs = triangles[id];
        int* nowTriNormIDs = triangle_normals[id];

        Triangle nowTriangle;
        nowTriangle.triangleVertices[0] = vertices_data[nowTirVerIDs[0]];
        nowTriangle.triangleVertices[1] = vertices_data[nowTirVerIDs[1]];
        nowTriangle.triangleVertices[2] = vertices_data[nowTirVerIDs[2]];
        nowTriangle.triangleNormals[0] = normals_data[nowTriNormIDs[0]];
        nowTriangle.triangleNormals[1] = normals_data[nowTriNormIDs[1]];
        nowTriangle.triangleNormals[2] = normals_data[nowTriNormIDs[2]];
        return nowTriangle;
    }

    void loadModel(const std::string& path) {
        if (path == loadedPath)
            return;
        loadedPath = path;
        std::ifstream file(path);
        std::string line;
        this->triangleCount = 0;
        this->normalCot = 1;
        this->verticesCount = 1; //obj 文件顶点从1开始计数
        this->centralPoint = vec3(0, 0, 0);
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            if (type == "v") {
                if (verticesCount >= MAX_VERTICES_COT)
                    continue;
                vec3 vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices_data[verticesCount] = vertex;
                centralPoint += vertex;
                verticesCount++;
            }
            else if (type == "vn") {
                if (normalCot >= MAX_VERTICES_COT)
                    continue;
                vec3 vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                normals_data[normalCot] = vertex;
                normalCot++;
            }
            else if (type == "f") {
                if (triangleCount >= MAX_TRIANGLE_COT)
                    continue;
                for (int i = 0; i < 3; ++i) {
                    char slash;
                    int vertexIndex, normalIndex, textIndex;
                    // 读取顶点索引、忽略纹理坐标、读取法线索引
                    iss >> vertexIndex >> slash >> textIndex >> slash >> normalIndex;
                    triangles[triangleCount][i] = vertexIndex;
                    triangle_normals[triangleCount][i] = normalIndex;
                }
                triangleCount++;
            }
        }
        centralPoint /= verticesCount;
        std::cout << "read " << triangleCount << " triangles" << std::endl;
    }
};

FragmentAttr getLinearInterpolation(const FragmentAttr& a, FragmentAttr& b, int x_position);
void renderWithTexture(vec3* render_buffer, int w, int h);
#endif // UTILS.H#pragma once
