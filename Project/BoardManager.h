#pragma once

#include "Coordinate.h"
#include <vector>

class PuzzlePiece;

namespace cocos2d {
    class Touch;
    class Rect;
}

class Puzzle;

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
    void generateMove(std::vector<PuzzlePiece*> &piecesToMove, SlideDirection dir);
    bool generateTileMoves(std::vector<PuzzlePiece*> &piecesToMove, PuzzlePiece* piece, 
                           float &xMoveDist, float &yMoveDist);
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