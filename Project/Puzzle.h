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

    int calculateOffset(int x, int y);
    coordinate calculateCoordinates(int piece);

    void swapPieces(int fromPiece, int toPiece);

private:
    bool inBounds(int x, int y);

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };

    float sizeX;
    float sizeY;
    cocos2d::Vec2 position;

    std::vector<PuzzlePiece*> puzzlePieces;
};