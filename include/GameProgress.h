// GameProgress.h
#ifndef GAMEPROGRESS_H
#define GAMEPROGRESS_H

#include "BlockField.h"
#include "BlockDealer.h"
#include "Render.h"
#include "KeyBoardManager.h"
#include "stb_image.h"
#include "TextureLoader.h"
#include "Utils.h"

// 定義旋轉方向枚舉
enum RotationDirection {
    CLOCKWISE,
    COUNTER_CLOCKWISE
};
// 假設 ActionType 已在 KeyBoardManager.h 中定義
// 如果沒有，請在此處定義或包含相應的頭文件

class GameProgress {
public:
    GameProgress();
    ~GameProgress();

    // 初始化遊戲設置
    void init();

    // 開始遊戲
    void start();


private:
    BlockField blockField;           // 場地管理
    BlockDealer blockDealer;         // 方塊生成
    RenderModule renderModule;       // 渲染模組
    KeyBoardManager keyBoardManager; // 鍵盤管理

    std::vector<std::array<int, 3>> ghostCoordinates;
    int ghostBaseX, ghostBaseY, ghostBaseZ;

    bool gameOver;    // 遊戲是否結束
    bool isPaused;    // 遊戲是否暫停
    int score;        // 分數
    bool running;     // 遊戲是否正在運行
    bool skipDrop = false;

    int baseX, baseY, baseZ; // 方塊的基座位置
    Block currentBlock;      // 當前方塊

    Block holdBlock; // 暫存方塊
    bool hasHoldBlock = false; // 是否已經有方塊在 holdBlock
    bool canHold = true;       // 是否可以在此回合使用 Hold

    // OpenGL 回調函數
    static void displayCallback();
    static void idleCallback();
    static void timerCallback(int value); // 定時器回調函數

    // 顯示函數
    void display();

    // 空閒函數
    void idle();

    // 處理鍵盤按鍵事件
    void handleKeyPress(ActionType action);

    // 方塊下落邏輯
    void dropBlock();

    // 方塊旋轉邏輯
    void rotateBlock(char axis, RotationDirection direction);

    // 顯示遊戲結束訊息
    void displayGameOver();

    // 設置 OpenGL 環境
    void setupOpenGL();

    void resetTimer();

    void updateGhostBlockPosition();

    void resetGhost();

    void handleHold();
};


#endif // GAMEPROGRESS_H
