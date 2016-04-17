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
    void setArrayPos(const int value);

    Coordinate getCoordinates() const;
    void setCoordinates(const int x, const int y);
    void setCoordinates(const Coordinate &c);

    bool isBlankSpace() const;
    void setBlankSpace(const bool value);

    int getID() const;
    void setID(const int value);

    void initNumLabel(PuzzleGame* const pScene);
    cocos2d::Label *getNumLabel() const;
    void enableLabel(const bool enable) const;

    void initListener(PuzzleGame* const pScene);

private:
    void setNumLabelPos() const;

    int arrayPos;
    Coordinate coords;
    bool blankSpace;
    int id;
    cocos2d::Label *numLabel;
};
