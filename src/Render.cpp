// Render.cpp
#include "Render.h"
#include <GL/freeglut.h>
#include "Utils.h"
#include "KeyBoardManager.h"
#include <iostream>

RenderModule::RenderModule(BlockField* field) 
    : blockField(field), fallingBaseX(0), fallingBaseY(0), fallingBaseZ(0) {}

void RenderModule::setFallingBlock(const RenderBlock& block, int baseX, int baseY, int baseZ) {
    fallingBlock = block;
    fallingBaseX = baseX;
    fallingBaseY = baseY;
    fallingBaseZ = baseZ;
}

void RenderModule::render() {
    glLoadIdentity();

    // 設置相機

    if (KeyBoardManager::isCtrlDown) {
        // ★★★ 俯瞰視角 ★★★
        gluLookAt(
            WIDTH / 2.0f, HEIGHT * 2.0f, DEPTH / 2.0f,
            WIDTH / 2.0f, 0.0f,          DEPTH / 2.0f,
            0.0f, 0.0f, -1.0f
        );
    } else {
        // ★★★ 一般視角（原本的視角）★★★
        gluLookAt(
            WIDTH * 1.2f, HEIGHT * 2.0f, DEPTH * 1.5f,
            WIDTH / 2.0f, 0.0f, DEPTH / 2.0f,
            0.0f, 1.0f, 0.0f
        );
    }

    // 繪製場地中的固定方塊
    blockField->render();

    // 繪製 Ghost
    drawGhostBlock();        

    // 繪製下落中的方塊
    drawFallingBlock();

    // 繪製邊界和基座
    drawBoundaryAndBase();
}

void RenderModule::drawFallingBlock() {
    if (fallingBlock.isFalling && fallingBlock.colorID != 0) {
        // 繪製填充方塊
        Color color = getColor(fallingBlock.colorID);
        glColor4f(color.r, color.g, color.b, 1.0f); // 不透明
        glBegin(GL_QUADS);
        for (const auto& coord : fallingBlock.coordinates) {
            float x = coord[0] + fallingBaseX + 0.5f;
            float y = coord[1] + fallingBaseY + 0.5f;
            float z = coord[2] + fallingBaseZ + 0.5f;
            // 前面
            glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
            glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
            glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
            // 後面
            glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
            glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
            // 左面
            glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
            glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
            glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
            // 右面
            glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
            // 上面
            glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
            glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
            // 下面
            glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
            glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
        }
        glEnd();

        // 啟用多邊形偏移來防止 Z-Fighting
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.0f, -1.0f); // 調整偏移值根據需要

        // 繪製描邊
        glLineWidth(1.0f); // 描邊線寬
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 切換到線框模式
        glBegin(GL_QUADS);
        for (const auto& coord : fallingBlock.coordinates) {
            Color cubeColor = getColor(fallingBlock.colorID);
            Color outlineColor = getOutlineColor(cubeColor);
            glColor4f(outlineColor.r, outlineColor.g, outlineColor.b, 1.0f); // 設置描邊顏色
            float x = coord[0] + fallingBaseX + 0.5f;
            float y = coord[1] + fallingBaseY + 0.5f;
            float z = coord[2] + fallingBaseZ + 0.5f;

            // 前面
            glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
            glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
            glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
            // 後面
            glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
            glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
            // 左面
            glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
            glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
            glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
            // 右面
            glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
            // 上面
            glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
            glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
            // 下面
            glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
            glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
            glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
        }
        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 恢復填充模式

        // 禁用多邊形偏移
        glDisable(GL_POLYGON_OFFSET_LINE);
    }
}

void RenderModule::drawBoundaryAndBase() {
    // 使用柔和的灰色作為邊界線顏色
    glColor4f(0.7f, 0.7f, 0.7f, 0.7f); // 淺灰色

    glLineWidth(2.0f); // 設定線寬

    glBegin(GL_LINES);
    
    // 繪製底部的 XZ 平面邊界
    // 繪製沿 X 軸的線
    for(int x = 0; x <= WIDTH; ++x){
        glVertex3f(x, 0.0f, 0.0f);
        glVertex3f(x, 0.0f, DEPTH);
    }
    // 繪製沿 Z 軸的線
    for(int z = 0; z <= DEPTH; ++z){
        glVertex3f(0.0f, 0.0f, z);
        glVertex3f(WIDTH, 0.0f, z);
    }

    // 繪製外側的垂直邊界線（XY 和 YZ 平面）
    // 四個角的垂直線
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, HEIGHT, 0.0f);

    glVertex3f(WIDTH, 0.0f, 0.0f);
    glVertex3f(WIDTH, HEIGHT, 0.0f);

    glVertex3f(0.0f, 0.0f, DEPTH);
    glVertex3f(0.0f, HEIGHT, DEPTH);

    glVertex3f(WIDTH, 0.0f, DEPTH);
    glVertex3f(WIDTH, HEIGHT, DEPTH);
    
    glEnd();

    glLineWidth(1.0f); // 恢復線寬
}


void RenderModule::setGhostBlock(const RenderBlock& block, int baseX, int baseY, int baseZ) {
    ghostBlock = block;
    ghostBaseX = baseX;
    ghostBaseY = baseY;
    ghostBaseZ = baseZ;
}

void RenderModule::drawGhostBlock() {
    // 若沒有 ghostBlock 或沒需要畫就 return
    if (ghostBlock.coordinates.empty()) return;

    glDisable(GL_BLEND);

    // 不寫入深度緩衝，只讀深度
    glDepthMask(GL_FALSE);  

    // 下面是常見的繪製 6 面立方體的程式
    Color color = getColor(ghostBlock.colorID);
    glColor4f(color.r, color.g, color.b, 0.3f); // 30% 透明

    glBegin(GL_QUADS);
    for (const auto& coord : ghostBlock.coordinates) {
        float x = coord[0] + ghostBaseX + 0.5f;
        float y = coord[1] + ghostBaseY + 0.5f;
        float z = coord[2] + ghostBaseZ + 0.5f;

        // 前面
        glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
        glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
        glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
        glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);

        // 後面
        glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
        glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
        glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
        glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);

        // 左面
        glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
        glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
        glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
        glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);

        // 右面
        glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
        glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
        glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
        glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);

        // 上面
        glVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
        glVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
        glVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
        glVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);

        // 下面
        glVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
        glVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
        glVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
        glVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
    }
    glEnd();

    // 還原
    glDepthMask(GL_TRUE); 
    
}

