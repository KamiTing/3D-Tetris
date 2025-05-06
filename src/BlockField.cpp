// BlockField.cpp
#include "BlockField.h"
#include <GL/freeglut.h>
#include <iostream>

extern bool useTextureForCubes; 
extern GLuint textureID;

BlockField::BlockField() {
    initializeField();
}

void BlockField::initializeField() {
    // 初始化三維場地，預設為 0（空）
    field.resize(WIDTH, std::vector<std::vector<int>>(HEIGHT, std::vector<int>(DEPTH, 0)));
}

bool BlockField::isValidPosition(const std::vector<std::array<int, 3>>& coordinates,
                                 int baseX, int baseY, int baseZ) {
    for(const auto &coord : coordinates){
        int x = coord[0] + baseX;
        int y = coord[1] + baseY;
        int z = coord[2] + baseZ;
        // 檢查邊界
        if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || z < 0 || z >= DEPTH){
            std::cout << "Invalid position: (" << x << ", " << y << ", " << z << ")\n";
            return false;
        }
        // 檢查是否已被佔用
        if(field[x][y][z] != 0){
            std::cout << "Position occupied: (" << x << ", " << y << ", " << z << ")\n";
            return false;
        }
    }
    return true;
}

PositionCheckResult BlockField::checkPosition(const std::vector<std::array<int, 3>>& coordinates,
                                              int baseX, int baseY, int baseZ) 
{
    for(const auto &coord : coordinates){
        int x = coord[0] + baseX;
        int y = coord[1] + baseY;
        int z = coord[2] + baseZ;

        // 邊界檢查
        if(x < 0)  return PositionCheckResult::OUT_OF_BOUNDS_LEFT;
        if(x >= WIDTH)  return PositionCheckResult::OUT_OF_BOUNDS_RIGHT;
        if(y < 0)  return PositionCheckResult::OUT_OF_BOUNDS_BOTTOM;
        if(y >= HEIGHT) return PositionCheckResult::OUT_OF_BOUNDS_TOP;
        if(z < 0)  return PositionCheckResult::OUT_OF_BOUNDS_FRONT;
        if(z >= DEPTH)  return PositionCheckResult::OUT_OF_BOUNDS_BACK;

        // 檢查佔用
        if(field[x][y][z] != 0) {
            return PositionCheckResult::OCCUPIED;
        }
    }
    return PositionCheckResult::VALID;
}

void BlockField::placeBlock(const std::vector<std::array<int, 3>>& coordinates,
                            int baseX, int baseY, int baseZ, int colorID) {
    for(const auto &coord : coordinates){
        int x = coord[0] + baseX;
        int y = coord[1] + baseY;
        int z = coord[2] + baseZ;
        if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && z >= 0 && z < DEPTH){
            field[x][y][z] = colorID;
            std::cout << "Placed block at (" << x << ", " << y << ", " << z << ")\n";
        }
    }
}

int BlockField::clearFullLayers() {
    int cleared = 0;
    for(int y = 0; y < HEIGHT; ++y){
        bool full = true;
        for(int x = 0; x < WIDTH; ++x){
            for(int z = 0; z < DEPTH; ++z){
                if(field[x][y][z] == 0){
                    full = false;
                    break;
                }
            }
            if(!full) break;
        }
        if(full){
            // 清除該層並將上方的層往下移
            for(int yy = y; yy < HEIGHT -1; ++yy){
                for(int x = 0; x < WIDTH; ++x){
                    for(int z = 0; z < DEPTH; ++z){
                        field[x][yy][z] = field[x][yy +1][z];
                    }
                }
            }
            // 最頂層清空
            for(int x = 0; x < WIDTH; ++x){
                for(int z = 0; z < DEPTH; ++z){
                    field[x][HEIGHT -1][z] = 0;
                }
            }
            cleared++;
            std::cout << "Layer " << y << " cleared.\n";
        }
    }
    return cleared;
}

/**
 * 自行繪製一個貼圖立方體，六面使用同一張貼圖。
 * 假設中心在 (0,0,0)，邊長=1 => 各面座標 ±0.5。
 */
static void drawTexturedCube() {
    glBegin(GL_QUADS);

    // 前面 (z = +0.5)
    // 左下 (-0.5, -0.5, +0.5) -> (0,0)
    // 右下 (+0.5, -0.5, +0.5) -> (1,0)
    // 右上 (+0.5, +0.5, +0.5) -> (1,1)
    // 左上 (-0.5, +0.5, +0.5) -> (0,1)
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, +0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(+0.5f, -0.5f, +0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(+0.5f, +0.5f, +0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, +0.5f, +0.5f);

    // 後面 (z = -0.5)
    // 左下 (+0.5, -0.5, -0.5) -> (0,0)
    // 右下 (-0.5, -0.5, -0.5) -> (1,0)
    // 右上 (-0.5, +0.5, -0.5) -> (1,1)
    // 左上 (+0.5, +0.5, -0.5) -> (0,1)
    // （也可以反過來，關鍵是同樣的貼圖朝向邏輯）
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(+0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, +0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(+0.5f, +0.5f, -0.5f);

    // 左面 (x = -0.5)
    // 左下 (-0.5, -0.5, -0.5) -> (0,0)
    // 右下 (-0.5, -0.5, +0.5) -> (1,0)
    // 右上 (-0.5, +0.5, +0.5) -> (1,1)
    // 左上 (-0.5, +0.5, -0.5) -> (0,1)
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, +0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, +0.5f, +0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, +0.5f, -0.5f);

    // 右面 (x = +0.5)
    // 左下 (+0.5, -0.5, +0.5) -> (0,0)
    // 右下 (+0.5, -0.5, -0.5) -> (1,0)
    // 右上 (+0.5, +0.5, -0.5) -> (1,1)
    // 左上 (+0.5, +0.5, +0.5) -> (0,1)
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(+0.5f, -0.5f, +0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(+0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(+0.5f, +0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(+0.5f, +0.5f, +0.5f);

    // 上面 (y = +0.5)
    // 左下 (-0.5, +0.5, +0.5) -> (0,0)
    // 右下 (+0.5, +0.5, +0.5) -> (1,0)
    // 右上 (+0.5, +0.5, -0.5) -> (1,1)
    // 左上 (-0.5, +0.5, -0.5) -> (0,1)
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, +0.5f, +0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(+0.5f, +0.5f, +0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(+0.5f, +0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, +0.5f, -0.5f);

    // 下面 (y = -0.5)
    // 左下 (-0.5, -0.5, -0.5) -> (0,0)
    // 右下 (+0.5, -0.5, -0.5) -> (1,0)
    // 右上 (+0.5, -0.5, +0.5) -> (1,1)
    // 左上 (-0.5, -0.5, +0.5) -> (0,1)
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(+0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(+0.5f, -0.5f, +0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, +0.5f);

    glEnd();
}


/**
 * 真正的 render：根據 useTextureForCubes 決定要用哪種方式畫。
 */
void BlockField::render() {
    glEnable(GL_DEPTH_TEST);
    //testRenderCube();

    for(int x = 0; x < WIDTH; ++x){
        for(int y = 0; y < HEIGHT; ++y){
            for(int z = 0; z < DEPTH; ++z){
                int cid = field[x][y][z];
                if(cid != 0) {
                    // 平移到該方塊座標
                    glPushMatrix();
                    glTranslatef(x + 0.5f, y + 0.5f, z + 0.5f);

                    if(!useTextureForCubes) {
                        // ★★★ 1) 純色模式 + Wireframe ★★★
                        Color color = getColor(cid);
                        glColor3f(color.r, color.g, color.b);

                        // 先畫實心
                        glutSolidCube(1.0f);

                        // 再畫 wireframe 輪廓
                        glLineWidth(1.0f);
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        Color outlineColor = getOutlineColor(color);
                        glColor4f(outlineColor.r, outlineColor.g, outlineColor.b, 1.0f);
                        glScalef(1.01f, 1.01f, 1.01f); // 避免 Z-fighting
                        glutWireCube(1.0f);
                        // 恢復填充模式
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                    } else {
                        // ★★★ 2) 貼圖模式 (不需要線框) ★★★
                        glEnable(GL_TEXTURE_2D);
                        glDisable(GL_BLEND);
                        glBindTexture(GL_TEXTURE_2D, textureID);

                        // 設成白色，避免顏色乘積
                        glColor3f(1.0f, 1.0f, 1.0f);

                        // 自行繪製 6 面立方體
                        drawTexturedCube();

                        // 不用 wireframe，故不做任何線框動作
                        glDisable(GL_TEXTURE_2D);
                    }

                    glPopMatrix();
                }
            }
        }
    }
}

void BlockField::reset(){
    for(int x = 0; x < WIDTH; ++x){
        for(int y = 0; y < HEIGHT; ++y){
            for(int z = 0; z < DEPTH; ++z){
                field[x][y][z] = 0; // 重置所有方塊為空
            }
        }
    }
    std::cout << "BlockField has been reset.\n";
}

