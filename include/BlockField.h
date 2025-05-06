// BlockField.h
#ifndef BLOCKFIELD_H
#define BLOCKFIELD_H

#include <vector>
#include <array>
#include "Utils.h"

// BlockField.h (示意)
enum class PositionCheckResult {
    VALID,                // 位置有效
    OUT_OF_BOUNDS_LEFT,   // x < 0
    OUT_OF_BOUNDS_RIGHT,  // x >= WIDTH
    OUT_OF_BOUNDS_BOTTOM, // y < 0
    OUT_OF_BOUNDS_TOP,    // y >= HEIGHT
    OUT_OF_BOUNDS_FRONT,  // z < 0
    OUT_OF_BOUNDS_BACK,   // z >= DEPTH
    OCCUPIED              // 該位置已被佔用
};


class BlockField {
public:
    BlockField();

    // 檢查方塊在指定位置是否有效
    bool isValidPosition(const std::vector<std::array<int, 3>>& coordinates, int baseX, int baseY, int baseZ);

    // 放置方塊到場地中
    void placeBlock(const std::vector<std::array<int, 3>>& coordinates, int baseX, int baseY, int baseZ, int colorID);

    PositionCheckResult checkPosition(const std::vector<std::array<int, 3>>& coordinates, int baseX, int baseY, int baseZ);

    // 清除滿層並返回清除的層數
    int clearFullLayers();

    // 渲染固定方塊
    void render();

    void reset();

private:
    // 場地表示，使用三維向量
    std::vector<std::vector<std::vector<int>>> field;

    // 初始化場地
    void initializeField();
};

#endif // BLOCKFIELD_H
