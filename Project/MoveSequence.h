#pragma once

#include <vector>

class PuzzlePiece;

namespace cocos2d
{
    class Label;
}

struct MoveSequence
{
    std::vector<PuzzlePiece*> pieceContainer;
    float xMoveDist = 0.0f;
    float yMoveDist = 0.0f;
};