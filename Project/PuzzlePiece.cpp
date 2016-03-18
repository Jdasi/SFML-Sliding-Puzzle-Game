#include "Project/PuzzlePiece.h"
USING_NS_CC;

PuzzlePiece::PuzzlePiece()
    : id(0)
    , blankSpace(false)
{
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

void PuzzlePiece::setBlankSpace(bool v)
{
    blankSpace = v;
}