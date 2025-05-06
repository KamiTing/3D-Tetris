// main.cpp
#include "GameProgress.h"
#include <cstring>    // 引入 strcmp
#include <string>     // 引入 std::string

bool useTextureForCubes = false;
GLuint textureID;

int main(int argc, char** argv) {
    // 檢查是否有傳入 "--os" 參數
    for(int i = 1; i < argc; ++i) {
        // 使用 strcmp 比較 C 字串
        if(std::string(argv[i]) == "--os") {
            useTextureForCubes = true;
        }
    }

    GameProgress game;
    game.init();
    game.start();
    return 0;
}
