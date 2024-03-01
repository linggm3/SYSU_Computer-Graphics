#include "textures.h"
/*###################################################
##  函数: Textures
##  函数描述： 初始化纹理类
##  参数描述： 无
#####################################################*/
Textures::Textures()
{
    //二维平面纹理和立方体贴图纹理个数
    count2D = countCube = 0;
}
/*###################################################
##  函数: LoadTexture2D
##  函数描述： 加载2D纹理
##  参数描述： filename 文件名
#####################################################*/
void Textures::LoadTexture2D(const char* filename)
{
    GLint width, height, i;               //位图大小
    GLubyte* image;                    //色素储存地址,中间交换量
    FILE* pf;                             //位图打开地址
    BITMAPFILEHEADER fileHeader;          //位图文件头（包含文件类型，文件信息头大小）
    BITMAPINFOHEADER infoHeader;          //位图信息头（包含位图宽高，色素总量大小）
    fopen_s(&pf, filename, "rb");             //只读方式打开图片文件
    if (pf == nullptr) {
        std::cout << "Failed to read files!" << std::endl;
        return;
    }
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pf);
    if (fileHeader.bfType != 0x4D42) {
        std::cout << "It is not the bmp file!" << std::endl;
        fclose(pf);
        return;
    }
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pf);
    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    if (infoHeader.biSizeImage==0)                 //计算图片像素总量
        infoHeader.biSizeImage = width * height * 4;
    image = (GLubyte*)malloc(sizeof(GLubyte) * infoHeader.biSizeImage);  //申请空间
    if (image == nullptr) {
        std::cout << "The space is not enough!" <<std::endl;
        fclose(pf);
        free(image);
        return;
    }
    fseek(pf, fileHeader.bfOffBits, SEEK_SET);              //将文件读写头移到文件头处
    fread(image, infoHeader.biSizeImage, 1, pf);
    for (i = 0; i < infoHeader.biSizeImage; i += 4) {          //openGL识别的是BGR,所以要置换过来
        std::swap(image[i], image[i+2]);
    }
    fclose(pf);
    glGenTextures(1, &textures2D[count2D]);
    glBindTexture(GL_TEXTURE_2D, textures2D[count2D]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //纹理设置，线性滤波
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     //纹理超出\不足则重复绘制
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image);
    count2D++;//分配纹理编号
}
/*###################################################
##  函数: LoadTextureCube
##  函数描述： 加载立方体贴图纹理
##  参数描述： filenames 文件名集合
#####################################################*/
void Textures::LoadTextureCube(const char (*filenames)[100])
{
    GLint width, height;
    GLubyte* image;
    FILE* pf;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    glGenTextures(1, &texturesCube[countCube]);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texturesCube[countCube]);
    countCube++;
    for (int j = 0; j < 6; j++)
    {
        fopen_s(&pf, filenames[j], "rb");
        if (pf == nullptr) {
            std::cout << "Failed to read files!" << std::endl;
            return;
        }
        fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, pf);
        if (fileHeader.bfType != 0x4D42) {
            std::cout << "It is not the bmp file!" << std::endl;
            fclose(pf);
            return;
        }
        fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, pf);
        width = infoHeader.biWidth;
        height = infoHeader.biHeight;
        if (infoHeader.biSizeImage)                 //计算图片像素总量
            infoHeader.biSizeImage = width * height * 3;
        image = (GLubyte*)malloc(sizeof(GLubyte) * infoHeader.biSizeImage);  //申请空间
        if (image == nullptr) {
            std::cout << "The space is not enough!" <<std::endl;
            fclose(pf);
            free(image);
            return;
        }
        fseek(pf, fileHeader.bfOffBits, SEEK_SET);              //将文件读写头移到文件头处
        fread(image, infoHeader.biSizeImage, 1, pf);
        for (int i = 0; i < infoHeader.biSizeImage; i += 3) {          //openGL识别的是BGR,所以要置换过来
            std::swap(image[i], image[i+2]);
        }
        fclose(pf);
        gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X+j, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //纹理设置，线性滤波
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}
