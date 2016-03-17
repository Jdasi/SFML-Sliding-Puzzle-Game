#include "Project/PuzzlePiece.h"
USING_NS_CC;

PuzzlePiece::PuzzlePiece()
    : sprite(nullptr)
    , sizeX(0)
    , sizeY(0)
    , id(0)
    , blankSpace(false)
{
}

void PuzzlePiece::setSprite(cocos2d::Sprite *spr)
{
    sprite = spr;
}

cocos2d::Sprite* PuzzlePiece::getSprite()
{
    return sprite;
}

Size PuzzlePiece::getSize()
{
    return sprite->getContentSize();
}

cocos2d::Vec2 PuzzlePiece::getPosition()
{
    return sprite->getPosition();
}

void PuzzlePiece::setPosition(cocos2d::Vec2 pos)
{
    sprite->setPosition(pos.x, pos.y);
}

void PuzzlePiece::setAnchorPoint(int x, int y)
{
    sprite->setAnchorPoint(Vec2(x, y));
}

void PuzzlePiece::setTag(int v)
{
    sprite->setTag(v);
}

void PuzzlePiece::setOpacity(int v)
{
    sprite->setOpacity(v);
}

bool PuzzlePiece::isBlankSpace()
{
    return blankSpace;
}

void PuzzlePiece::setBlankSpace(bool v)
{
    blankSpace = v;
}