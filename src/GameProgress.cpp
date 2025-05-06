// GameProgress.cpp
#include "GameProgress.h"
#include <GL/freeglut.h>
#include <iostream>

extern GLint textureID;

// 靜態指針指向當前遊戲實例，用於回調
static GameProgress* currentGame = nullptr;

// 定時器間隔（毫秒）
const int DROP_INTERVAL = 500;

// Constructor
GameProgress::GameProgress()
    : renderModule(&blockField), 
      keyBoardManager(),
      gameOver(false), 
      isPaused(false), 
      score(0), 
      running(false),
      hasHoldBlock(false),   // Hold 區初始為空
      canHold(true)          // 新遊戲第一顆方塊可使用 Hold
{
    currentGame = this;
}

// Destructor
GameProgress::~GameProgress() {
    running = false;
}

// 初始化遊戲
void GameProgress::init() {
    setupOpenGL();
    textureID = loadTexture("../texture/tex.jpg");
    if (textureID != 0) {
        std::cout << "Loading texture " << textureID << std::endl;
    }
    keyBoardManager.setActionCallback([this](ActionType action) {
        handleKeyPress(action);
    });
}

// 開始遊戲
void GameProgress::start() {
    currentBlock = blockDealer.getNextBlock();

    // 計算當前方塊的最大 Y 值
    int maxY = 0;
    for (const auto& coord : currentBlock.coordinates) {
        if (coord[1] > maxY) {
            maxY = coord[1];
        }
    }
    baseX = WIDTH / 2;
    baseY = HEIGHT - 1 - maxY;
    baseZ = DEPTH / 2;

    std::cout << "Spawning first block at (" << baseX << ", " << baseY << ", " << baseZ << ")\n";

    RenderBlock renderBlock{ currentBlock.coordinates, currentBlock.colorID, true };
    renderModule.setFallingBlock(renderBlock, baseX, baseY, baseZ);

    // 檢查遊戲是否結束
    if (!blockField.isValidPosition(currentBlock.coordinates, baseX, baseY, baseZ)) {
        gameOver = true;
        running = false;
        std::cout << "Game Over! Your score: " << score << std::endl;
    } else {
        std::cout << "First block spawned successfully.\n";
    }

    running = true;
    resetTimer();        // 開始計時器
    updateGhostBlockPosition();
    glutMainLoop();
}

// 重置計時器
void GameProgress::resetTimer() {
    if (running && !gameOver && !isPaused) {
        std::cout << "Resetting timer with interval: " << DROP_INTERVAL << "ms\n";
        glutTimerFunc(DROP_INTERVAL, timerCallback, 0);
    }
}

// 設置 OpenGL 初始化
void GameProgress::setupOpenGL() {
    int argc = 1;
    char* argv[] = { (char*)"3DTetris" };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("3D Tetris");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1200.0 / 800.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);

    // ★★★ 改用自己「包一層」回調來處理按下/放開
    glutKeyboardFunc([](unsigned char key, int x, int y){
        currentGame->keyBoardManager.handleKeyPress(key, x, y);
    });
    glutKeyboardUpFunc([](unsigned char key, int x, int y){
        currentGame->keyBoardManager.handleKeyRelease(key, x, y);
    });

    glutSpecialFunc([](int key, int x, int y){
        currentGame->keyBoardManager.handleSpecialKeyPress(key, x, y);
    });
    glutSpecialUpFunc([](int key, int x, int y){
        currentGame->keyBoardManager.handleSpecialKeyRelease(key, x, y);
    });

    glutDisplayFunc(displayCallback);
    glutIdleFunc(idleCallback);
}


// 顯示回調函數
void GameProgress::displayCallback() {
    if (currentGame) {
        currentGame->display();
    }
}

// 空閒回調函數
void GameProgress::idleCallback() {
    if (currentGame) {
        currentGame->idle();
    }
}

// 顯示函數
void GameProgress::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    renderModule.render();

    if (gameOver) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        displayGameOver();

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    glutSwapBuffers();
}

// 空閒函數
void GameProgress::idle() {
    glutPostRedisplay();
}

void clearScreen() {
    // 切换到正确的投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // 设置 2D 正交投影

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1200.0 / 800.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glutSwapBuffers(); // 切换缓冲区
}

// 處理按鍵
void GameProgress::handleKeyPress(ActionType action) {
    if (gameOver) {
        if (action == ActionType::RESTART) {
            blockField.reset();
            score = 0;
            gameOver = false;
            running = true;

            clearScreen();

            holdBlock = {};      // 若是 struct / class，可以用 {} 初始化
            hasHoldBlock = false;
            canHold = true;
            
            resetGhost();

            currentBlock = blockDealer.getNextBlock();
            int maxY = 0;
            for (const auto& coord : currentBlock.coordinates) {
                if (coord[1] > maxY) {
                    maxY = coord[1];
                }
            }

            baseX = WIDTH / 2;
            baseY = HEIGHT - 1 - maxY;
            baseZ = DEPTH / 2;

            RenderBlock renderBlock{ currentBlock.coordinates, currentBlock.colorID, true };
            renderModule.setFallingBlock(renderBlock, baseX, baseY, baseZ);

            resetTimer(); // 重置計時器
            updateGhostBlockPosition();
            display();
        }
        return;
    }

    if (isPaused) {
        if (action == ActionType::PAUSE) {
            isPaused = false;
            std::cout << "Game Resumed." << std::endl;
            resetTimer(); // 重置計時器
        }
        return;
    }

    switch(action){
        case ActionType::MOVE_LEFT:
            if(blockField.isValidPosition(currentBlock.coordinates, baseX -1, baseY, baseZ)){
                baseX -=1;
                std::cout << "Moved left to (" << baseX << ", " << baseY << ", " << baseZ << ")\n";
            }
            break;
        case ActionType::MOVE_RIGHT:
            if(blockField.isValidPosition(currentBlock.coordinates, baseX +1, baseY, baseZ)){
                baseX +=1;
                std::cout << "Moved right to (" << baseX << ", " << baseY << ", " << baseZ << ")\n";
            }
            break;
        case ActionType::MOVE_FORWARD:
            if(blockField.isValidPosition(currentBlock.coordinates, baseX, baseY, baseZ -1)){
                baseZ -=1;
                std::cout << "Moved forward to (" << baseX << ", " << baseY << ", " << baseZ << ")\n";
            }
            break;
        case ActionType::MOVE_BACKWARD:
            if(blockField.isValidPosition(currentBlock.coordinates, baseX, baseY, baseZ +1)){
                baseZ +=1;
                std::cout << "Moved backward to (" << baseX << ", " << baseY << ", " << baseZ << ")\n";
            }
            break;
        case ActionType::MOVE_DOWN:
            if(blockField.isValidPosition(currentBlock.coordinates, baseX, baseY -1, baseZ)){
                baseY -=1;
                std::cout << "Moved down to (" << baseX << ", " << baseY << ", " << baseZ << ")\n";
            }
            break;
        case ActionType::ROTATE_X_CLOCKWISE:
            rotateBlock('x', CLOCKWISE);
            break;
        case ActionType::ROTATE_X_COUNTERCLOCKWISE:
            rotateBlock('x', COUNTER_CLOCKWISE);
            break;
        case ActionType::ROTATE_Y_CLOCKWISE:
            rotateBlock('y', CLOCKWISE);
            break;
        case ActionType::ROTATE_Y_COUNTERCLOCKWISE:
            rotateBlock('y', COUNTER_CLOCKWISE);
            break;
        case ActionType::ROTATE_Z_CLOCKWISE:
            rotateBlock('z', CLOCKWISE);
            break;
        case ActionType::ROTATE_Z_COUNTERCLOCKWISE:
            rotateBlock('z', COUNTER_CLOCKWISE);
            break;
        case ActionType::DROP:
            // 快速下落
            while(blockField.isValidPosition(currentBlock.coordinates, baseX, baseY -1, baseZ)){
                baseY -=1;
            }
            dropBlock();
            std::cout << "Block dropped quickly.\n";
            break;
        case ActionType::PAUSE:
            isPaused = true;
            std::cout << "Game Paused." << std::endl;
            break;
        case ActionType::HOLD:
            handleHold();
            break;
        case ActionType::QUIT:
            exit(0);
        default:
            break;
    }

    // 每次鍵盤操作後，都更新 Ghost & 畫面
    updateGhostBlockPosition();
    RenderBlock renderBlock{ currentBlock.coordinates, currentBlock.colorID, true };
    renderModule.setFallingBlock(renderBlock, baseX, baseY, baseZ);
}

// 定時器回調函數
void GameProgress::timerCallback(int value) {
    if (currentGame && currentGame->running && !currentGame->isPaused && !currentGame->gameOver) {
        std::cout << "Timer triggered: Dropping block...\n";
        currentGame->dropBlock();
        currentGame->resetTimer(); // 重置計時器
    }
}

// 掉落方塊邏輯
void GameProgress::dropBlock() {
    if (gameOver || isPaused) return;
    if (skipDrop) {
        skipDrop = false;
        return;
    }

    // 檢查能否往下
    if (blockField.isValidPosition(currentBlock.coordinates, baseX, baseY - 1, baseZ)) {
        baseY -= 1;
        std::cout << "Block moved down to (" << baseX << ", " << baseY << ", " << baseZ << ")\n";
    } else {
        // 放置方塊
        blockField.placeBlock(currentBlock.coordinates, baseX, baseY, baseZ, currentBlock.colorID);
        int cleared = blockField.clearFullLayers();
        score += cleared * 100;
        if (cleared > 0) {
            std::cout << "Cleared " << cleared << " layer(s)! Score: " << score << std::endl;
        }

        // 生成新方塊
        currentBlock = blockDealer.getNextBlock();
        skipDrop = true;

        // 新方塊出現後，就可以再次 Hold
        canHold = true;

        // 計算新方塊的最大 Y
        int maxY = 0;
        for (const auto& coord : currentBlock.coordinates) {
            if (coord[1] > maxY) {
                maxY = coord[1];
            }
        }
        baseX = WIDTH / 2;
        baseY = HEIGHT - 1 - maxY;
        baseZ = DEPTH / 2;

        if (!blockField.isValidPosition(currentBlock.coordinates, baseX, baseY, baseZ)) {
            gameOver = true;
            running = false;
            std::cout << "Game Over! Your score: " << score << std::endl;
        }
    }

    updateGhostBlockPosition();
    RenderBlock renderBlock{ currentBlock.coordinates, currentBlock.colorID, !gameOver };
    renderModule.setFallingBlock(renderBlock, baseX, baseY, baseZ);

    glutPostRedisplay();
}

// 旋轉方塊
void GameProgress::rotateBlock(char axis, RotationDirection direction) {
    std::cout << "Attempting to rotate around axis " << axis 
              << " in direction " 
              << (direction == CLOCKWISE ? "CLOCKWISE" : "COUNTER_CLOCKWISE") 
              << std::endl;

    std::cout << "Original Coordinates:\n";
    for(const auto& coord : currentBlock.coordinates) {
        std::cout << "(" << coord[0] << ", " << coord[1] << ", " << coord[2] << ") ";
    }
    std::cout << std::endl;

    // ------------------------------
    // 1. 執行旋轉 
    // ------------------------------
    std::vector<std::array<int,3>> rotated = currentBlock.coordinates;
    for(auto &coord : rotated) {
        int x = coord[0];
        int y = coord[1];
        int z = coord[2];
        switch(axis){
            case 'x':
                if(direction == CLOCKWISE){
                    coord[1] = -z;
                    coord[2] = y;
                } else {
                    coord[1] = z;
                    coord[2] = -y;
                }
                break;
            case 'y':
                if(direction == CLOCKWISE){
                    coord[0] = z;
                    coord[2] = -x;
                } else {
                    coord[0] = -z;
                    coord[2] = x;
                }
                break;
            case 'z':
                if(direction == CLOCKWISE){
                    coord[0] = -y;
                    coord[1] = x;
                } else {
                    coord[0] = y;
                    coord[1] = -x;
                }
                break;
            default:
                break;
        }
    }

    std::cout << "Rotated Coordinates:\n";
    for(const auto& coord : rotated) {
        std::cout << "(" << coord[0] << ", " << coord[1] << ", " << coord[2] << ") ";
    }
    std::cout << std::endl;

    // ------------------------------
    // 2. 使用 BlockField::checkPosition 得到更詳細的狀態
    // ------------------------------
    PositionCheckResult result = blockField.checkPosition(rotated, baseX, baseY, baseZ);

    if(result == PositionCheckResult::VALID) {
        // 直接成功
        currentBlock.coordinates = rotated;
        std::cout << "Block rotated successfully around axis " << axis << ".\n";
    } else {
        std::cout << "Rotation invalid, reason: " << static_cast<int>(result) 
                  << ". Attempting wall-kick...\n";

        // 記錄旋轉前的位置
        int originalX = baseX;
        int originalY = baseY;
        int originalZ = baseZ;

        bool kicked = false;

        // 這裡示範依失敗原因做不同的補償
        // (你可依需求增減、或直接全部嘗試 offset)
        switch(result) {
            case PositionCheckResult::OUT_OF_BOUNDS_LEFT:
                // x < 0 => 嘗試往 +X 推一格
                if(blockField.checkPosition(rotated, baseX + 1, baseY, baseZ) 
                   == PositionCheckResult::VALID) 
                {
                    baseX += 1;
                    currentBlock.coordinates = rotated;
                    kicked = true;
                    std::cout << "Wall-kick success: moved +1 in X.\n";
                }
                break;

            case PositionCheckResult::OUT_OF_BOUNDS_RIGHT:
                // x >= WIDTH => 嘗試往 -X 推一格
                if(blockField.checkPosition(rotated, baseX - 1, baseY, baseZ) 
                   == PositionCheckResult::VALID) 
                {
                    baseX -= 1;
                    currentBlock.coordinates = rotated;
                    kicked = true;
                    std::cout << "Wall-kick success: moved -1 in X.\n";
                }
                break;

            case PositionCheckResult::OUT_OF_BOUNDS_FRONT:
                // z < 0 => 嘗試往 +Z 推一格
                if(blockField.checkPosition(rotated, baseX, baseY, baseZ + 1) 
                   == PositionCheckResult::VALID)
                {
                    baseZ += 1;
                    currentBlock.coordinates = rotated;
                    kicked = true;
                    std::cout << "Wall-kick success: moved +1 in Z.\n";
                }
                break;

            case PositionCheckResult::OUT_OF_BOUNDS_BACK:
                // z >= DEPTH => 嘗試往 -Z 推一格
                if(blockField.checkPosition(rotated, baseX, baseY, baseZ - 1) 
                   == PositionCheckResult::VALID)
                {
                    baseZ -= 1;
                    currentBlock.coordinates = rotated;
                    kicked = true;
                    std::cout << "Wall-kick success: moved -1 in Z.\n";
                }
                break;

            case PositionCheckResult::OCCUPIED:
                // 如果是被佔用，也可嘗試上下或左右的 offset
                // 這裡只是示範往上推 1 格
                if(blockField.checkPosition(rotated, baseX, baseY + 1, baseZ) 
                   == PositionCheckResult::VALID)
                {
                    baseY += 1;
                    currentBlock.coordinates = rotated;
                    kicked = true;
                    std::cout << "Wall-kick success: moved +1 in Y.\n";
                }
                break;

            // 其他 case (TOP, BOTTOM, ... 看需求自行處理)
            default:
                break;
        }

        // 如果沒踢成功，就還原
        if(!kicked) {
            std::cout << "All wall-kick attempts failed, revert rotation.\n";
            baseX = originalX;
            baseY = originalY;
            baseZ = originalZ;
            // 不改 currentBlock.coordinates
        }else skipDrop = true;
    }

    // ------------------------------
    // 3. 更新渲染
    // ------------------------------
    RenderBlock renderBlock{ currentBlock.coordinates, currentBlock.colorID, true };
    renderModule.setFallingBlock(renderBlock, baseX, baseY, baseZ);
}


// 顯示遊戲結束畫面
void GameProgress::displayGameOver() {
    std::string gameOverText = "Game Over!";
    std::string scoreText = "Score: " + std::to_string(score);

    glLineWidth(5.0f);

    glPushMatrix();
    glTranslatef(300.0f, 400.0f, 0.0f);
    glScalef(0.3f, 0.3f, 0.3f);
    glColor4f(1.0f, 0.0f, 0.0f,1.0f);
    for (char c : gameOverText) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(300.0f, 350.0f, 0.0f);
    glScalef(0.2f, 0.2f, 0.2f);
    glColor4f(0.8f, 0.0f, 0.0f,1.0f);
    for (char c : scoreText) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();
}

// 更新 Ghost 方塊位置
void GameProgress::updateGhostBlockPosition() {
    // 先從當前方塊拷貝一份
    ghostCoordinates = currentBlock.coordinates;
    ghostBaseX = baseX;
    ghostBaseY = baseY;
    ghostBaseZ = baseZ;

    // 不斷往下測試，看能不能再往下
    while (blockField.isValidPosition(ghostCoordinates, ghostBaseX, ghostBaseY - 1, ghostBaseZ)) {
        ghostBaseY--;
    }

    RenderBlock ghostRenderBlock {
        ghostCoordinates,
        currentBlock.colorID,  // 跟 falling block 同色
        false                  // ghost 不算真正 isFalling
    };
    renderModule.setGhostBlock(ghostRenderBlock, ghostBaseX, ghostBaseY, ghostBaseZ);
}

// 重置 Ghost
void GameProgress::resetGhost() {
    RenderBlock emptyGhostBlock;
    emptyGhostBlock.coordinates.clear();
    emptyGhostBlock.colorID = 0;
    emptyGhostBlock.isFalling = false;
    renderModule.setGhostBlock(emptyGhostBlock, 0, 0, 0);
}

// ★★ 這裡是「Hold」的核心邏輯 ★★
void GameProgress::handleHold() {
    // 若此顆方塊已用過 Hold，就不能再用
    if (!canHold) {
        std::cout << "Hold already used this turn.\n";
        return;
    }

    // 如果 hold 區是空的
    if (!hasHoldBlock) {
        // 把當前方塊存進 holdBlock
        holdBlock = currentBlock;
        hasHoldBlock = true;
        std::cout << "[HOLD] Stored current block.\n";

        // 拿下一個方塊
        currentBlock = blockDealer.getNextBlock();
    }
    // 否則 hold 區已經有方塊 -> 跟當前方塊互換
    else {
        Block temp = currentBlock;
        currentBlock = holdBlock;
        holdBlock = temp;
        std::cout << "[HOLD] Swapped current block with hold block.\n";
    }

    // 這顆方塊已經使用過 Hold，不可再用
    canHold = false;

    // 重設當前方塊座標
    int maxY = 0;
    for (const auto& coord : currentBlock.coordinates) {
        if (coord[1] > maxY) {
            maxY = coord[1];
        }
    }
    baseX = WIDTH / 2;
    baseY = HEIGHT - 1 - maxY;
    baseZ = DEPTH / 2;

    // 檢查是否能放得下
    if (!blockField.isValidPosition(currentBlock.coordinates, baseX, baseY, baseZ)) {
        gameOver = true;
        running = false;
        std::cout << "Game Over! Your score: " << score << std::endl;
        return;
    }

    // 更新 Ghost / 渲染
    updateGhostBlockPosition();
    RenderBlock renderBlock{ currentBlock.coordinates, currentBlock.colorID, true };
    renderModule.setFallingBlock(renderBlock, baseX, baseY, baseZ);
}
