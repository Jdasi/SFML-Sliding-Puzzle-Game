#pragma once

#include "PuzzlePiece.h"

#include <vector>

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

    int calculateOffset(const int x, const int y) const;
    int calculateOffset(const Coordinate &coords) const;

    int getPadding() const;

    void swapPieces(const int fromPiece, const int toPiece);
    void swapPieces(const PuzzlePiece &fromRef, const PuzzlePiece &toRef);
    bool isPuzzleComplete() const;

private:
    void scaleImage(const cocos2d::Sprite* const spr);
    void configurePiece(PuzzlePiece* const piece, const Coordinate &startPos, 
                        const float secX, const float secY, const int xCycles, 
                        const int yCycles, PuzzleGame* const pScene) const;

    int totalPieces;
    int pad;
    float sizeX;
    float sizeY;
    float scaleFactorX;
    float scaleFactorY;

    std::vector<PuzzlePiece*> puzzlePieces;
};