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

void PuzzlePiece::setArrayPos(const int value)
{
    arrayPos = value;
}

Coordinate PuzzlePiece::getCoordinates() const
{
    return coords;
}

void PuzzlePiece::setCoordinates(const int x, const int y)
{
    coords = { x, y };
}

void PuzzlePiece::setCoordinates(const Coordinate &c)
{
    setCoordinates(c.x, c.y);
}

PuzzlePiece *PuzzlePiece::create(const std::string &file, const Rect &rect)
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

void PuzzlePiece::setBlankSpace(const bool value)
{
    blankSpace = value;

    setOpacity(!isBlankSpace() ? 255 : 0);
    enableLabel(!value);
}

int PuzzlePiece::getID() const
{
    return id;
}

void PuzzlePiece::setID(const int value)
{
    id = value;
}

void PuzzlePiece::initNumLabel(PuzzleGame* const pScene)
{
    Size rectSize = this->getBoundingBox().size;
    float fontSize = (rectSize.width * 0.125) + (rectSize.height * 0.125);

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
                               rectPos.y - (rectSize.height * 0.4)));
}

Label *PuzzlePiece::getNumLabel() const
{
    return numLabel;
}

void PuzzlePiece::enableLabel(const bool enable) const
{
    numLabel->setOpacity(enable ? 255 : 0);
}

void PuzzlePiece::initListener(PuzzleGame* const pScene)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan =
        CC_CALLBACK_2(PuzzleGame::interactWithPuzzle, pScene);
    _eventDispatcher->
        addEventListenerWithSceneGraphPriority(listener, this);
}