#include "PuzzlePiece.h"
#include "GameSettings.h"
#include "PuzzleGame.h"

USING_NS_CC;

PuzzlePiece::PuzzlePiece()
    : arrayPos(0)
    , coords{0, 0}
    , blankSpace(false)
    , id(0)
    , numLabel(nullptr)
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

Coordinate PuzzlePiece::getCoordinates() const
{
    return coords;
}

void PuzzlePiece::setCoordinates(int x, int y)
{
    coords = { x, y };
}

void PuzzlePiece::setCoordinates(Coordinate c)
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

    if (isBlankSpace())
    {
        setOpacity(0);
        enableLabel(false);
    }
    else
    {
        setOpacity(255);
        enableLabel(true);
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

void PuzzlePiece::initNumLabel(PuzzleGame *pScene)
{
    Size rectSize = this->getBoundingBox().size;
    float fontSize = (rectSize.width * 0.10) + (rectSize.height * 0.15);

    numLabel = Label::createWithTTF
        (std::to_string(this->getID() + 1), GameSettings::getFontName(), fontSize);
    numLabel->enableGlow(Color4B::BLACK);
    setNumLabelPos();
    enableLabel(false);

    pScene->addChild(numLabel, 2);
}

void PuzzlePiece::setNumLabelPos() const
{
    Size rectSize = this->getBoundingBox().size;
    Vec2 rectPos = this->getPosition();

    numLabel->setPosition(Vec2(rectPos.x + (rectSize.width * 0.2), 
                               rectPos.y - (rectSize.height * 0.25)));
}

cocos2d::Label* PuzzlePiece::getNumLabel() const
{
    return numLabel;
}

void PuzzlePiece::enableLabel(bool enable) const
{
    if (enable)
    {
        numLabel->setOpacity(255);
    }
    else
    {
        numLabel->setOpacity(0);
    }
}