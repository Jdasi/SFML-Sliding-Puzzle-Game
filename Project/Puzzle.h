#pragma once

#include "PuzzlePiece.h"

class PuzzleGame;

class Puzzle : public cocos2d::Layer
{
public:
    Puzzle();
    ~Puzzle();

    void initPuzzle(PuzzleGame* const pScene, const Coordinate &startPos);

    PuzzlePiece &getPiece(const int piece) const;
    PuzzlePiece &getPiece(const Coordinate &coords) const;
    PuzzlePiece &getPiece(const int x, const int y) const;
    PuzzlePiece **getPieces() const;

    int calculateOffset(const int x, const int y) const;
    int calculateOffset(const Coordinate &coords) const;

    int getPadding() const;
    int getTotalPieces() const;

    void swapPieces(const int fromPiece, const int toPiece) const;
    void swapPieces(const PuzzlePiece &fromRef, const PuzzlePiece &toRef) const;
    bool isPuzzleComplete() const;

private:
    void scaleImage(const cocos2d::Sprite* const spr);
    void sliceImage(PuzzleGame* const pScene, const Coordinate &startPos,
                    const float secX, const float secY);
    void configurePiece(PuzzlePiece* const piece, const Coordinate &startPos, 
                        const float secX, const float secY, const int xCycles, 
                        const int yCycles, PuzzleGame* const pScene) const;

    int segmentsX;
    int segmentsY;
    std::string file;
    int totalPieces;
    int pad;
    float imageSizeX;
    float imageSizeY;
    float scaleFactorX;
    float scaleFactorY;

    PuzzlePiece **puzzlePieces;
};