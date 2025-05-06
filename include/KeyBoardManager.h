// KeyBoardManager.h
#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H

#include <functional>
#include <unordered_map>
#include "Utils.h"

enum ActionType {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_DOWN,
    ROTATE_X_CLOCKWISE,
    ROTATE_X_COUNTERCLOCKWISE,
    ROTATE_Y_CLOCKWISE,
    ROTATE_Y_COUNTERCLOCKWISE,
    ROTATE_Z_CLOCKWISE,
    ROTATE_Z_COUNTERCLOCKWISE,
    DROP,
    PAUSE,
    RESTART,
    HOLD,
    QUIT
};

class KeyBoardManager {
public:
    KeyBoardManager();

    // 設定按鍵對應的動作
    void setKeyMapping(unsigned char key, ActionType action);
    void setSpecialKeyMapping(int key, ActionType action);

    // 設定動作的回調函數
    void setActionCallback(std::function<void(ActionType)> callback);

    // 處理按鍵事件
    void handleKeyPress(unsigned char key, int x, int y);
    void handleKeyRelease(unsigned char key, int x, int y);
    void handleSpecialKeyPress(int key, int x, int y);
    void handleSpecialKeyRelease(int key, int x, int y);

    static bool isCtrlDown;

private:
    std::function<void(ActionType)> actionCallback;

    // 鍵位映射
    std::unordered_map<unsigned char, ActionType> keyMap;
    std::unordered_map<int, ActionType> specialKeyMap;
};

#endif // KEYBOARDMANAGER_H
