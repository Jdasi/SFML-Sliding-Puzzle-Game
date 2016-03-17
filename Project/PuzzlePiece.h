#pragma once
#include "cocos2d.h"

class PuzzlePiece
{
public:
    PuzzlePiece();
    ~PuzzlePiece() = default;

    void PuzzlePiece::setSprite(cocos2d::Sprite *spr);
    cocos2d::Sprite *getSprite();

    cocos2d::Size getSize();

    cocos2d::Vec2 getPosition();
    void setPosition(cocos2d::Vec2 pos);

    void setAnchorPoint(int x, int y);
    void setTag(int v);
    void setOpacity(int v);

    bool isBlankSpace();
    void setBlankSpace(bool v);

private:
    cocos2d::Sprite *sprite;

    float sizeX;
    float sizeY;
    cocos2d::Vec2 position;

    int id;
    bool blankSpace;
};