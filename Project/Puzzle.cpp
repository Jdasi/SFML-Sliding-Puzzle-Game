#include "Puzzle.h"
#include "PuzzleGame.h"
#include "GameSettings.h"

USING_NS_CC;

Puzzle::Puzzle()
    : totalPieces(GameSettings::getSegments().x * GameSettings::getSegments().y)
    , sizeX(0)
    , sizeY(0)
    , scaleFactorX(1.0f)
    , scaleFactorY(1.0f)
{
}

void Puzzle::initPuzzle(PuzzleGame *pScene, int startPosX, int startPosY)
{
    int segmentsX = GameSettings::getSegments().x;
    int segmentsY = GameSettings::getSegments().y;
    std::string file{ GameSettings::getImageName() };

    Sprite *puzzle = Sprite::create(file);

    sizeX = puzzle->getContentSize().width;
    sizeY = puzzle->getContentSize().height;

    float secX = sizeX / segmentsX;
    float secY = sizeY / segmentsY;

    int pad = 2;

    sanityCheckImage(puzzle, pad);

    // Splice puzzle image into PuzzlePieces.
    for (int yCycles = 0; yCycles < segmentsY; ++yCycles)
    {
        for (int xCycles = 0; xCycles < segmentsX; ++xCycles)
        {
            PuzzlePiece *piece = PuzzlePiece::create(file, Rect
                (0 + (secX * xCycles), 0 + (secY * yCycles), secX, secY));

            piece->setScaleX(scaleFactorX);
            piece->setScaleY(scaleFactorY);

            // Set anchor point to top left of piece.
            piece->setAnchorPoint(Vec2(0, 1));

            piece->setPosition
                (Vec2(startPosX + (((secX * xCycles) * scaleFactorX) + (xCycles * pad)),
                (visibleSize.height - startPosY) - (((secY * yCycles) * scaleFactorY) + 
                (yCycles * pad))));

            piece->setArrayPos(calculateOffset(xCycles, yCycles));
            piece->setID(calculateOffset(xCycles, yCycles));
            piece->setCoordinates(xCycles, yCycles);

            pScene->addChild(piece, 1);
            
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan =
                CC_CALLBACK_2(PuzzleGame::interactWithPuzzle, pScene);
            _eventDispatcher->
                addEventListenerWithSceneGraphPriority(listener, piece);

            puzzlePieces.push_back(piece);
        }
    }

    // Hide bottom right puzzle piece.
    puzzlePieces[(segmentsX * segmentsY) - 1]->setBlankSpace(true);
}

void Puzzle::sanityCheckImage(cocos2d::Sprite *spr, int pad)
{
    int paddingX = (GameSettings::getSegments().x - 1) * pad;
    int paddingY = (GameSettings::getSegments().y - 1) * pad;

    if (sizeX + paddingX != 800)
    {
        scaleFactorX = (800 - paddingX) / spr->getContentSize().width;
    }

    if (sizeY + paddingY != 600)
    {
        scaleFactorY = (600 - paddingY) / spr->getContentSize().height;
    }
}

PuzzlePiece &Puzzle::getPiece(int piece)
{
    return *puzzlePieces[piece];
}

bool Puzzle::isPieceBlankSpace(int piece) const
{
    return puzzlePieces[piece]->isBlankSpace();
}

int Puzzle::findBlankSpace()
{
    for (PuzzlePiece *piece : puzzlePieces)
    {
        if (piece->isBlankSpace())
        {
            return piece->getArrayPos();
        }
    }

    return 0;
}

int Puzzle::calculateOffset(int x, int y) const
{
    return (y * GameSettings::getSegments().x) + x;
}

void Puzzle::swapPieces(int fromPiece, int toPiece)
{
    std::swap(puzzlePieces[fromPiece], puzzlePieces[toPiece]);

    // Save values temporarily.
    int fromArrayPos = puzzlePieces[fromPiece]->getArrayPos();
    int toArrayPos = puzzlePieces[toPiece]->getArrayPos();

    coordinate fromCoords = puzzlePieces[fromPiece]->getCoordinates();
    coordinate toCoords = puzzlePieces[toPiece]->getCoordinates();

    // Re-assign using temporary values.
    puzzlePieces[fromPiece]->setArrayPos(toArrayPos);
    puzzlePieces[toPiece]->setArrayPos(fromArrayPos);

    puzzlePieces[fromPiece]->setCoordinates(toCoords);
    puzzlePieces[toPiece]->setCoordinates(fromCoords);
}

bool Puzzle::isPuzzleComplete() const
{
    int correctPieces = 0;
    for (PuzzlePiece *piece : puzzlePieces)
    {
        if (piece->getArrayPos() == piece->getID())
        {
            ++correctPieces;
        }
    }

    if (correctPieces == totalPieces)
    {
        return true;
    }

    return false;
}

bool Puzzle::inBounds(int x, int y) const
{
    if (x >= GameSettings::getSegments().x || x < 0)
    {
        return false;
    }

    if (y >= GameSettings::getSegments().y || y < 0)
    {
        return false;
    }

    return true;
}

void Puzzle::hideAllPieces()
{
    for (PuzzlePiece *piece : puzzlePieces)
    {
        piece->setOpacity(0);
    }
}