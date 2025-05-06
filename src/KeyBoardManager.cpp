#include "KeyBoardManager.h"
#include <iostream>

// 預設值：起初 Ctrl 沒被按
bool KeyBoardManager::isCtrlDown = false;
extern bool useTextureForCubes;
char prev = '\0';
KeyBoardManager::KeyBoardManager() {
    // 預設鍵位映射
    setKeyMapping('w', ActionType::ROTATE_X_CLOCKWISE);
    setKeyMapping('s', ActionType::ROTATE_X_COUNTERCLOCKWISE);
    setKeyMapping('a', ActionType::ROTATE_Z_CLOCKWISE);
    setKeyMapping('d', ActionType::ROTATE_Z_COUNTERCLOCKWISE);
    setKeyMapping('q', ActionType::ROTATE_Y_CLOCKWISE);
    setKeyMapping('e', ActionType::ROTATE_Y_COUNTERCLOCKWISE);
    setKeyMapping(' ', ActionType::DROP);
    setKeyMapping('p', ActionType::PAUSE);
    setKeyMapping('t', ActionType::RESTART);
    setKeyMapping('h', ActionType::HOLD);
    setKeyMapping(27, ActionType::QUIT); // Esc

    // 特殊鍵映射
    setSpecialKeyMapping(GLUT_KEY_LEFT, ActionType::MOVE_LEFT);
    setSpecialKeyMapping(GLUT_KEY_RIGHT, ActionType::MOVE_RIGHT);
    setSpecialKeyMapping(GLUT_KEY_UP, ActionType::MOVE_FORWARD);
    setSpecialKeyMapping(GLUT_KEY_DOWN, ActionType::MOVE_BACKWARD);
}

void KeyBoardManager::setKeyMapping(unsigned char key, ActionType action) {
    keyMap[key] = action;
}

void KeyBoardManager::setSpecialKeyMapping(int key, ActionType action) {
    specialKeyMap[key] = action;
}

void KeyBoardManager::setActionCallback(std::function<void(ActionType)> callback) {
    actionCallback = callback;
}

//
// 1) 按下普通鍵
//
void KeyBoardManager::handleKeyPress(unsigned char key, int x, int y) {
    int mod = glutGetModifiers();
    isCtrlDown = (mod & GLUT_ACTIVE_CTRL);

    // 將控制字元轉回對應的小寫字母
    key = convertCtrlCode(key);

    if (key == 's' and prev == 'o' ) useTextureForCubes = true;
    prev = key;
    if (keyMap.find(key) != keyMap.end()) {
        if (actionCallback) {
            actionCallback(keyMap[key]);
        }
    }
}


//
// 2) 放開普通鍵
//
void KeyBoardManager::handleKeyRelease(unsigned char key, int x, int y) {
    // 再檢查一次修飾鍵
    int mod = glutGetModifiers();
    if (mod & GLUT_ACTIVE_CTRL) {
        isCtrlDown = true;
    } else {
        isCtrlDown = false;
    }
    // 可以視需要做一些「按鍵放開」時的動作
}

//
// 3) 按下特殊鍵
//
void KeyBoardManager::handleSpecialKeyPress(int key, int x, int y) {
    int mod = glutGetModifiers();
    if (mod & GLUT_ACTIVE_CTRL) {
        isCtrlDown = true;
    } else {
        isCtrlDown = false;
    }

    if (specialKeyMap.find(key) != specialKeyMap.end()) {
        if (actionCallback) {
            actionCallback(specialKeyMap[key]);
        }
    }
}

//
// 4) 放開特殊鍵
//
void KeyBoardManager::handleSpecialKeyRelease(int key, int x, int y) {
    int mod = glutGetModifiers();
    if (mod & GLUT_ACTIVE_CTRL) {
        isCtrlDown = true;
    } else {
        isCtrlDown = false;
    }
}
