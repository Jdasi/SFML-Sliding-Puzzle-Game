#pragma once

#include "Coordinate.h"
#include <vector>

class PuzzlePiece;

namespace cocos2d {
    class Touch;
    class Rect;
}

class Puzzle;
struct MoveSequence;

class BoardManager
{
    enum class SlideDirection
    {
        up = 0,
        right,
        down,
        left
    };

public:
    BoardManager(Puzzle &ref, int &blankSpaceRef);
    ~BoardManager() = default;

    bool sanityCheckMove(cocos2d::Rect &rect, cocos2d::Touch &touch, PuzzlePiece *piece);
    bool generateTileMoves(MoveSequence &seq, PuzzlePiece *piece);
    void generateMove(MoveSequence &seq, SlideDirection dir);
    bool pushBackTilesToBeMoved(std::vector<PuzzlePiece*> &container, coordinate pos);

    void generateRandomMoves(int times);
    bool tryComputerMove(int fromPiece, int toX, int toY);

    void updateBlankspaceInfo();
    void moveBlankSpaceToStart();

private:
    int &blankSpace;
    coordinate blankSpaceCoords;
    Puzzle &puzzle;
    int currentPieceArrayPos;
};