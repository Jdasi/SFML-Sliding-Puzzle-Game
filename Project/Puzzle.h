#pragma once

#include "PuzzlePiece.h"

#include "cocos2d.h"
#include <Vector>

class PuzzleGame;

class Puzzle : public cocos2d::Layer
{
public:
    Puzzle();
    ~Puzzle() = default;

    void initPuzzle(PuzzleGame *pScene, int startPosX, int startPosY);

    PuzzlePiece &getPiece(int piece);
    PuzzlePiece &getPiece(coordinate coords);
    PuzzlePiece &getPiece(int x, int y);
    std::vector<PuzzlePiece*> &getPieces();

    bool isPieceBlankSpace(int piece) const;

    int calculateOffset(int x, int y) const;
    int calculateOffset(coordinate coords) const;

    int getPadding() const;

    void swapPieces(int fromPiece, int toPiece);
    void swapPieces(const PuzzlePiece &fromRef, const PuzzlePiece &toRef);
    bool isPuzzleComplete() const;

    bool inBounds(int x, int y) const;

    void hideAllPieces() const;

private:
    void sanityCheckImage(cocos2d::Sprite *spr);

    int totalPieces;
    int pad;
    float sizeX;
    float sizeY;
    float scaleFactorX;
    float scaleFactorY;

    std::vector<PuzzlePiece*> puzzlePieces;

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};