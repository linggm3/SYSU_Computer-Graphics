#ifndef TEXTURES_H
#define TEXTURES_H
#include <QOpenGLFunctions>
#include <vector>
#include <iostream>
#include <algorithm>
#include <GL/glut.h>
/*###################################################
##  类: Textures
##  类描述： 记录纹理信息
#####################################################*/
class Textures
{
public:
    GLuint textures2D[20];  //2D纹理信息
    GLuint texturesCube[20];    //立方体纹理信息
    int count2D;    //2D纹理个数
    int countCube;  //立方体纹理的个数
    void LoadTexture2D(const char* filename);
    void LoadTextureCube(const char (*filenames)[100]);
    Textures();
};

#endif // TEXTURES_H
