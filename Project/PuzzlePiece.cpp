#include "PuzzlePiece.h"
USING_NS_CC;

PuzzlePiece::PuzzlePiece()
    : arrayPos(0)
    , coords{0, 0}
    , blankSpace(false)
    , id(0)
{
}

int PuzzlePiece::getArrayPos() const
{
    return arrayPos;
}

void PuzzlePiece::setArrayPos(int value)
{
    arrayPos = value;
}

coordinate PuzzlePiece::getCoordinates() const
{
    return coords;
}

void PuzzlePiece::setCoordinates(int x, int y)
{
    coords = { x, y };
}

void PuzzlePiece::setCoordinates(coordinate c)
{
    coords = c;
}

PuzzlePiece *PuzzlePiece::create(const std::string &file, const cocos2d::Rect &rect)
{
    PuzzlePiece* pSprite = new PuzzlePiece();

    if (pSprite->initWithFile(file, rect))
    {
        pSprite->autorelease();

        return pSprite;
    }

    CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool PuzzlePiece::isBlankSpace() const
{
    return blankSpace;
}

void PuzzlePiece::setBlankSpace(bool b)
{
    blankSpace = b;

    if (this->isBlankSpace())
    {
        this->setOpacity(0);
    }
    else
    {
        this->setOpacity(255);
    }
}

int PuzzlePiece::getID() const
{
    return id;
}

void PuzzlePiece::setID(int value)
{
    id = value;
}