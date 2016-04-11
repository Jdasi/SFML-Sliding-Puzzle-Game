#pragma once

#include "PuzzlePiece.h"

#include <Vector>

class PuzzleGame;

class Puzzle : public cocos2d::Layer
{
public:
    Puzzle();
    ~Puzzle() = default;

    void initPuzzle(PuzzleGame* const pScene, const Coordinate &startPos);

    PuzzlePiece &getPiece(const int piece);
    PuzzlePiece &getPiece(const Coordinate &coords);
    PuzzlePiece &getPiece(const int x, const int y);
    std::vector<PuzzlePiece*> &getPieces();

    bool isPieceBlankSpace(const int piece) const;

    int calculateOffset(const int x, const int y) const;
    int calculateOffset(const Coordinate &coords) const;

    int getPadding() const;

    void swapPieces(const int fromPiece, const int toPiece);
    void swapPieces(const PuzzlePiece &fromRef, const PuzzlePiece &toRef);
    bool isPuzzleComplete() const;

    bool inBounds(const int x, const int y) const;

private:
    void sanityCheckImage(const cocos2d::Sprite* const spr);

    int totalPieces;
    int pad;
    float sizeX;
    float sizeY;
    float scaleFactorX;
    float scaleFactorY;

    std::vector<PuzzlePiece*> puzzlePieces;
};