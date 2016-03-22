#pragma once
#include "Project/PuzzlePiece.h"

#include "cocos2d.h"
#include <Vector>

class PuzzleScene;

class Puzzle : public cocos2d::Layer
{
public:
    Puzzle();
    ~Puzzle();

    void initPuzzle(PuzzleScene *pScene, int startPosX, int startPosY);

    PuzzlePiece &getPiece(int piece);

    bool isPieceBlankSpace(int piece);
    int findBlankSpace();

    void swapPieces(int fromPiece, int toPiece);
    bool isPuzzleComplete();

    int calculateOffset(int x, int y);
    coordinate calculateCoordinates(int piece);

    bool inBounds(int x, int y);

private:
    void sanityCheckImage(cocos2d::Sprite *spr, int pad);

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };

    float sizeX;
    float sizeY;

    float scaleFactorX;
    float scaleFactorY;

    std::vector<PuzzlePiece*> puzzlePieces;
};