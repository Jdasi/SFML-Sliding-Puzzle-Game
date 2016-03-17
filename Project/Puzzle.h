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

    cocos2d::Size getSize();

    cocos2d::Vec2 getPosition();
    void setPosition(cocos2d::Vec2 pos);

    int calculateOffset(int x, int y);

private:
    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };

    cocos2d::Sprite puzzleSprite;

    float sizeX;
    float sizeY;
    cocos2d::Vec2 position;

    std::vector<PuzzlePiece> puzzlePieces;
};