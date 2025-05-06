// BlockDealer.h
#ifndef BLOCKDEALER_H
#define BLOCKDEALER_H

#include <vector>
#include <array>
#include <random>
#include <chrono>
#include "Utils.h"

struct Block {
    std::vector<std::array<int, 3>> coordinates; // 相對座標
    int colorID;
};

class BlockDealer {
public:
    BlockDealer();

    Block getNextBlock();

private:
    std::vector<Block> pool;
    std::vector<Block> blockPool;

    void initializePool();
    void shufflePool();
};

#endif // BLOCKDEALER_H
