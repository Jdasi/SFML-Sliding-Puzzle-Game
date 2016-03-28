#pragma once

#include "cocos2d.h"

class PuzzlePiece : public cocos2d::Sprite
{
public:
    PuzzlePiece();
    ~PuzzlePiece() = default;

    static PuzzlePiece *create(const std::string &file, const cocos2d::Rect &rect);

    bool isBlankSpace() const;
    void setBlankSpace(bool b);

    int getID() const;
    void setID(int value);

private:
    bool blankSpace;
    int id;
};
