#include "Project/Puzzle.h"
#include "Project/PuzzleScene.h"
#include "Project/GameSettings.h"
#include <complex>

USING_NS_CC;

Puzzle::Puzzle()
    : sizeX(0)
    , sizeY(0)
    , scaleFactorX(1.0f)
    , scaleFactorY(1.0f)
{
}

Puzzle::~Puzzle()
{
}

void Puzzle::initPuzzle(PuzzleScene *pScene, int startPosX, int startPosY)
{
    int segmentsX = GameSettings::getSegments().x;
    int segmentsY = GameSettings::getSegments().y;
    std::string file{ GameSettings::getImageName() };

    auto puzzle = Sprite::create(file);

    sizeX = puzzle->getContentSize().width;
    sizeY = puzzle->getContentSize().height;

    float secX = sizeX / segmentsX;
    float secY = sizeY / segmentsY;

    int pad = 2;

    sanityCheckImage(pad);

    // Splice puzzle image into PuzzlePieces.
    for (int yCycles = 0; yCycles < segmentsY; ++yCycles)
    {
        for (int xCycles = 0; xCycles < segmentsX; ++xCycles)
        {
            PuzzlePiece *piece = PuzzlePiece::create(file, Rect
                (0 + (secX * xCycles), 0 + (secY * yCycles), secX, secY));

            piece->setScaleX(scaleFactorX);
            piece->setScaleY(scaleFactorY);

            // Set anchor point to top left of image.
            piece->setAnchorPoint(Vec2(0, 1));

            piece->setPosition
                (Vec2(startPosX + (((secX * xCycles) * scaleFactorX) + (xCycles * pad)),
                (visibleSize.height - startPosY) - (((secY * yCycles) * scaleFactorY) + 
                (yCycles * pad))));

            piece->setTag((yCycles * segmentsX) + xCycles);
            piece->setID((yCycles * segmentsX) + xCycles);

            pScene->addChild(piece, (yCycles * segmentsX) + xCycles);
            
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan =
                CC_CALLBACK_2(PuzzleScene::interactWithPuzzle, pScene);
            _eventDispatcher->
                addEventListenerWithSceneGraphPriority(listener, piece);

            puzzlePieces.push_back(piece);
        }
    }

    // Hide bottom right puzzle piece.
    int affectedPiece = (segmentsX * segmentsY) - 1;
    puzzlePieces[affectedPiece]->setBlankSpace(true);
}

void Puzzle::sanityCheckImage(int pad)
{
    int paddingX = GameSettings::getSegments().x * pad;
    int paddingY = GameSettings::getSegments().y * pad;

    // Scale down image.
    if (sizeX + paddingX > 800)
    {
        for (float i = scaleFactorX; i > 0; i -= 0.001f)
        {
            if ((sizeX * i) + paddingX <= 800)
            {
                scaleFactorX = i;
                break;
            }
        }
    }

    if (sizeY + paddingY > 600)
    {
        for (float i = scaleFactorY; i > 0; i -= 0.001f)
        {
            if ((sizeY * i) + paddingY <= 600)
            {
                scaleFactorY = i;
                break;
            }
        }
    }

    // Scale up image.
    if (sizeX + paddingX < 800)
    {
        float i = scaleFactorX;
        while ((sizeX * i) + paddingX <= 800)
        {
            i += 0.001f;
            scaleFactorX = i;
        }
    }

    if (sizeY + paddingY < 600)
    {
        float i = scaleFactorY;
        while ((sizeY * i) + paddingY <= 600)
        {
            i += 0.001f;
            scaleFactorY = i;
        }
    }
}

PuzzlePiece &Puzzle::getPiece(int piece)
{
    return *puzzlePieces[piece];
}

bool Puzzle::isPieceBlankSpace(int piece)
{
    coordinate pieceCoords = calculateCoordinates(piece);

    return puzzlePieces[piece]->isBlankSpace();
}

int Puzzle::findBlankSpace()
{
    for (unsigned int i = 0; i < puzzlePieces.size(); ++i)
    {
        if (puzzlePieces[i]->isBlankSpace())
        {
            return i;
        }
    }

    return 0;
}

void Puzzle::swapPieces(int fromPiece, int toPiece)
{
    std::swap(puzzlePieces[fromPiece], puzzlePieces[toPiece]);

    int fromTag = puzzlePieces[fromPiece]->getTag();
    int toTag = puzzlePieces[toPiece]->getTag();

    puzzlePieces[fromPiece]->setTag(toTag);
    puzzlePieces[toPiece]->setTag(fromTag);
}

bool Puzzle::isPuzzleComplete()
{
    int totalPieces = GameSettings::getSegments().x * GameSettings::getSegments().y;

    int correctPieces = 0;
    for (unsigned int i = 0; i < puzzlePieces.size(); ++i)
    {
        if (puzzlePieces[i]->getTag() == puzzlePieces[i]->getID())
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

int Puzzle::calculateOffset(int x, int y)
{
    return (y * GameSettings::getSegments().x) + x;
}

coordinate Puzzle::calculateCoordinates(int piece)
{
    int x = piece % GameSettings::getSegments().x;
    int y = piece / GameSettings::getSegments().x;

    return { x, y };
}

bool Puzzle::inBounds(int x, int y)
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