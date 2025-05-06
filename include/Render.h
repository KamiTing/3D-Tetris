// Render.h
#ifndef RENDER_H
#define RENDER_H

#include "BlockField.h"
#include <vector>
#include <array>

struct RenderBlock {
    std::vector<std::array<int, 3>> coordinates;
    int colorID;
    bool isFalling;
};

class RenderModule {
public:
    RenderModule(BlockField* field);

    // 設定當前下落的方塊
    void setFallingBlock(const RenderBlock& block, int baseX, int baseY, int baseZ);

    // 繪製場景
    void render();
    
    void setGhostBlock(const RenderBlock& block, int baseX, int baseY, int baseZ);
private:
    BlockField* blockField;
    RenderBlock fallingBlock;
    int fallingBaseX, fallingBaseY, fallingBaseZ;

    RenderBlock ghostBlock;
    int ghostBaseX, ghostBaseY, ghostBaseZ;

    void drawFallingBlock();
    void drawBoundaryAndBase(); // 新增函數

    void drawGhostBlock();
};

#endif // RENDER_H
