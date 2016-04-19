#pragma once

#include "Coordinate.h"
#include "SlideDirection.h"

namespace cocos2d
{
    class Touch;
    class Rect;
}

class Puzzle;
class PuzzlePiece;
class MoveSequence;

class BoardManager
{
public:
    BoardManager(Puzzle &ref);
    ~BoardManager() = default;

    bool sanityCheckMove(cocos2d::Rect &rect, cocos2d::Touch &touch, 
                         PuzzlePiece* const piece) const;
    bool identifyMoves(MoveSequence &seq, PuzzlePiece* const piece);
    bool generateMove(MoveSequence &seq, const SlideDirection &dir) const;
    bool generateRandomMove(MoveSequence &seq);

    int findBlankSpace() const;
    void updateBlankspaceInfo();

    void swapPieces(const int fromPiece, const int toPiece) const;
    bool isPuzzleComplete() const;
    void hideAllPieces(const bool hide) const;
    void enableAllLabels(const bool enable) const;

private:
    bool pushBackTilesToBeMoved(MoveSequence &seq, const Coordinate pos) const;
    bool inBounds(const int x, const int y) const;

    int blankSpace;
    Coordinate blankSpaceCoords;
    Puzzle &puzzle;
    int currentPieceArrayPos;
    SlideDirection lastDir;
};