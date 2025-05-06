// BlockDealer.cpp
#include "BlockDealer.h"
#include <algorithm> // 需要引入 <algorithm> 以使用 std::shuffle

BlockDealer::BlockDealer() {
    initializePool();
    shufflePool();
}

void BlockDealer::initializePool() {
    pool = {
        // I
        Block{ { {0,0,0}, {1,0,0}, {-1,0,0}, {-2,0,0} }, 1 },
        // O
        Block{ { {0,0,0}, {1,0,0}, {0,1,0}, {1,1,0} }, 2 },
        // T
        Block{ { {0,0,0}, {1,0,0}, {-1,0,0}, {0,1,0} }, 3 },
        // S
        Block{ { {0,0,0}, {1,0,0}, {0,1,0}, {-1,1,0} }, 4 },
        // Z
        Block{ { {0,0,0}, {-1,0,0}, {0,1,0}, {1,1,0} }, 5 },
        // J
        Block{ { {0,0,0}, {-1,0,0}, {1,0,0}, {1,1,0} }, 6 },
        // L
        Block{ { {0,0,0}, {-1,0,0}, {1,0,0}, {-1,1,0} }, 7 }
    };
}

void BlockDealer::shufflePool() {
    blockPool = pool;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(blockPool.begin(), blockPool.end(), std::default_random_engine(seed));
}

Block BlockDealer::getNextBlock() {
    if (blockPool.empty()) {
        shufflePool();
    }
    Block next = blockPool.back();
    blockPool.pop_back();
    return next;
}
