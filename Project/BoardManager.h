#pragma once

#include "Coordinate.h"
#include "SlideDirection.h"

#include <vector>

namespace cocos2d
{
    class Label;
};

class PuzzlePiece;

namespace cocos2d
{
    class Touch;
    class Rect;
}

class Puzzle;
struct MoveSequence;

class BoardManager
{
public:
    BoardManager(Puzzle &ref);
    ~BoardManager() = default;

    bool sanityCheckMove(cocos2d::Rect &rect, cocos2d::Touch &touch, 
                         PuzzlePiece* const piece) const;
    bool generateTileMoves(MoveSequence &seq, PuzzlePiece* const piece);
    bool generateMove(MoveSequence &seq, const SlideDirection &dir);
    bool generateRandomMove(MoveSequence &seq);
    bool pushBackTilesToBeMoved(MoveSequence &seq, const Coordinate pos) const;

    int findBlankSpace() const;
    void updateBlankspaceInfo();

    void swapPieces(const int fromPiece, const int toPiece) const;
    bool isPuzzleComplete() const;
    void hideAllPieces(const bool hide) const;
    void enableAllLabels(const bool enable) const;

private:
    int blankSpace;
    Coordinate blankSpaceCoords;
    Puzzle &puzzle;
    int currentPieceArrayPos;
    SlideDirection lastDir;
};