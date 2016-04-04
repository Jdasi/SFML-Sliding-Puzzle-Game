#pragma once

#include "Coordinate.h"

#include "cocos2d.h"

class PuzzleGame;

class PuzzlePiece : public cocos2d::Sprite
{
public:
    PuzzlePiece();
    ~PuzzlePiece() = default;

    static PuzzlePiece *create(const std::string &file, const cocos2d::Rect &rect);

    int getArrayPos() const;
    void setArrayPos(int value);

    Coordinate getCoordinates() const;
    void setCoordinates(int x, int y);
    void setCoordinates(Coordinate c);

    bool isBlankSpace() const;
    void setBlankSpace(bool b);

    int getID() const;
    void setID(int value);

    void initNumLabel(PuzzleGame *pScene);
    void setNumLabelPos() const;
    cocos2d::Label *getNumLabel() const;
    void enableLabel(bool enable) const;

private:
    int arrayPos;
    Coordinate coords;
    bool blankSpace;
    int id;
    cocos2d::Label *numLabel;
};
