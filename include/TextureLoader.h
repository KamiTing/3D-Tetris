#pragma once
#include <GL/freeglut.h>

// 載入一張圖片，並產生對應的 OpenGL 貼圖ID (GLuint)
// path: 影像檔相對或絕對路徑
// 回傳: 若成功，回傳貼圖ID；若失敗，回傳 0。
GLuint loadTexture(const char* path);
