//TextureLoader.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "TextureLoader.h"
#include <iostream>

GLuint loadTexture(const char* path){
    int width, height, channels;

    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load image: " << path << std::endl;
        return 0;
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // 把影像資料交給OpenGL
    glTexImage2D(GL_TEXTURE_2D,
                 0,             // MIP map level
                 GL_RGB,       // 內部存儲格式
                 width, height, // 寬高
                 0,             // 邊框(一般用0)
                 GL_RGB,       // 畫面像素的格式
                 GL_UNSIGNED_BYTE,
                 data);

    // 設定貼圖縮放/放大時的濾波方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 若要重複方式貼圖可加:
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 讀完後釋放 CPU 上的影像暫存
    stbi_image_free(data);

    std::cout << "Loading " << path 
            << " width=" << width 
            << " height=" << height
            << " channels=" << channels 
            << std::endl;
    std::cout << "Bind textureID=" << texID << " error=" << glGetError() << std::endl;


    return texID;
}
