#pragma once

#include "Coordinate.h"

#include "cocos2d.h"

class PuzzlePiece : public cocos2d::Sprite
{
public:
    PuzzlePiece();
    ~PuzzlePiece() = default;

    static PuzzlePiece *create(const std::string &file, const cocos2d::Rect &rect);

    int getArrayPos() const;
    void setArrayPos(int value);

    coordinate getCoordinates() const;
    void setCoordinates(int x, int y);
    void setCoordinates(coordinate c);

    bool isBlankSpace() const;
    void setBlankSpace(bool b);

    int getID() const;
    void setID(int value);

private:
    int arrayPos;
    coordinate coords;
    bool blankSpace;
    int id;
};
