#pragma once

#include "Coordinate.h"
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
    enum class SlideDirection
    {
        up = 0,
        right,
        down,
        left
    };

public:
    BoardManager(Puzzle &ref);
    ~BoardManager() = default;

    bool sanityCheckMove(cocos2d::Rect &rect, cocos2d::Touch &touch, 
                         PuzzlePiece* const piece) const;
    bool generateTileMoves(MoveSequence &seq, PuzzlePiece* const piece);
    void generateMove(MoveSequence &seq, const SlideDirection dir);
    bool pushBackTilesToBeMoved(MoveSequence &seq, const Coordinate pos) const;

    void generateRandomMoves(const int times);
    bool tryComputerMove(const int fromPiece, const Coordinate &coords) const;
    void computerMovePiece(PuzzlePiece &fromPieceRef, PuzzlePiece &toPieceRef) const;
    void computerMovePieceLabel(const PuzzlePiece &fromPieceRef, const Coordinate &coords)
                                const;

    int findBlankSpace() const;
    void updateBlankspaceInfo();
    void moveBlankSpaceToStart();

    void swapPieces(const int fromPiece, const int toPiece) const;
    bool isPuzzleComplete() const;
    void hideAllPieces(const bool hide) const;
    void enableAllLabels(const bool enable) const;

private:
    int blankSpace;
    Coordinate blankSpaceCoords;
    Puzzle &puzzle;
    int currentPieceArrayPos;
};