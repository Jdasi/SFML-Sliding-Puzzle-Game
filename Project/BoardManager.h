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

    bool sanityCheckMove(cocos2d::Rect &rect, cocos2d::Touch &touch, PuzzlePiece *piece);
    bool generateTileMoves(MoveSequence &seq, PuzzlePiece *piece);
    void generateMove(MoveSequence &seq, SlideDirection dir);
    bool pushBackTilesToBeMoved(std::vector<PuzzlePiece*> &pieceContainer, 
                                std::vector<cocos2d::Label*> &labelContainer, 
                                Coordinate pos);

    void generateRandomMoves(int times);
    bool tryComputerMove(int fromPiece, Coordinate coords);
    void computerMovePiece(PuzzlePiece &fromPieceRef, PuzzlePiece &toPieceRef) const;
    void computerMovePieceLabel(PuzzlePiece &fromPieceRef, Coordinate coords);

    int findBlankSpace() const;
    void updateBlankspaceInfo();
    void moveBlankSpaceToStart();

    void swapPieces(int fromPiece, int toPiece);
    bool isPuzzleComplete() const;
    void hideAllPieces(bool hide) const;
    void enableAllLabels(bool enable) const;

private:
    int blankSpace;
    Coordinate blankSpaceCoords;
    Puzzle &puzzle;
    int currentPieceArrayPos;
};