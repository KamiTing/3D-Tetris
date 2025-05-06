// Utils.h
#ifndef UTILS_H
#define UTILS_H

#include <GL/freeglut.h>
#include <cmath>

// 常用常數定義
constexpr int WIDTH = 7;
constexpr int DEPTH = 7;
constexpr int HEIGHT = 12;

// 顏色結構體
struct Color {
    float r, g, b, a;
};

// 根據顏色編號返回對應的RGBA顏色
inline Color getColor(int colorID) {
    switch (colorID) {
        case 1: // I
            return {0.0f, 1.0f, 1.0f, 1.0f};
        case 2: // O
            return {1.0f, 1.0f, 0.0f, 1.0f};
        case 3: // T
            return {0.5f, 0.0f, 0.5f, 1.0f};
        case 4: // S
            return {0.0f, 1.0f, 0.0f, 1.0f};
        case 5: // Z
            return {1.0f, 0.0f, 0.0f, 1.0f};
        case 6: // J
            return {0.0f, 0.0f, 1.0f, 1.0f};
        case 7: // L
            return {1.0f, 0.5f, 0.0f, 1.0f};
        default:
            return {1.0f, 1.0f, 1.0f, 1.0f}; // 白色
    }
}

// 根據方塊顏色亮度返回對應的描邊顏色（白色或黑色）
inline Color getOutlineColor(const Color& cubeColor) {
    // 計算亮度（根據人眼對不同顏色的感知權重）
    float brightness = 0.299f * cubeColor.r + 0.587f * cubeColor.g + 0.114f * cubeColor.b;
    
    if (brightness < 0.5f) {
        // 方塊較暗，使用白色描邊
        return Color{1.0f, 1.0f, 1.0f, 1.0f};
    } else {
        // 方塊較亮，使用黑色描邊
        return Color{0.0f, 0.0f, 0.0f, 1.0f};
    }
}

inline unsigned char convertCtrlCode(unsigned char c) {
    // 如果是 1~26，就對應到 'a'~'z'
    if (c >= 1 && c <= 26) {
        return (c - 1) + 'a'; // 1→'a', 2→'b', ... 26→'z'
    }
    return c;
}


#endif // UTILS_H
