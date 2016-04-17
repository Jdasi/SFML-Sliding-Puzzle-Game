#include "Puzzle.h"
#include "PuzzleGame.h"
#include "GameSettings.h"

USING_NS_CC;

Puzzle::Puzzle()
    : totalPieces(GameSettings::getSegments().x * GameSettings::getSegments().y)
    , pad(2)
    , sizeX(0)
    , sizeY(0)
    , scaleFactorX(1.0f)
    , scaleFactorY(1.0f)
{
}

// Loads the chosen image file and slices it according to the GameSettings specification.
void Puzzle::initPuzzle(PuzzleGame* const pScene, const Coordinate &startPos)
{
    int segmentsX = GameSettings::getSegments().x;
    int segmentsY = GameSettings::getSegments().y;

    std::string file{ GameSettings::getImageName() };
    Sprite *puzzle = Sprite::create(file);

    sizeX = puzzle->getContentSize().width;
    sizeY = puzzle->getContentSize().height;

    float secX = sizeX / segmentsX;
    float secY = sizeY / segmentsY;

    scaleImage(puzzle);

    // Splice puzzle image into PuzzlePieces.
    for (int yCycles = 0; yCycles < segmentsY; ++yCycles)
    {
        for (int xCycles = 0; xCycles < segmentsX; ++xCycles)
        {
            PuzzlePiece *piece = PuzzlePiece::create(file, Rect
                (0 + (secX * xCycles), 0 + (secY * yCycles), secX, secY));

            configurePiece(piece, startPos, secX, secY, xCycles, yCycles, pScene);

            pScene->addChild(piece, 1);
            puzzlePieces.push_back(piece);
        }
    }

    // Hide bottom right puzzle piece.
    puzzlePieces[(segmentsX * segmentsY) - 1]->setBlankSpace(true);
}

void Puzzle::configurePiece(PuzzlePiece* const piece, const Coordinate &startPos, 
                            const float secX, const float secY, const int xCycles, 
                            const int yCycles, PuzzleGame* const pScene) const
{
    piece->setScaleX(scaleFactorX);
    piece->setScaleY(scaleFactorY);

    // Set anchor point to top left of piece.
    piece->setAnchorPoint(Vec2(0, 1));

    piece->setPosition(Vec2(startPos.x + (((secX * xCycles) * scaleFactorX) +
                      (xCycles * pad)), startPos.y - (((secY * yCycles) *
                      scaleFactorY) + (yCycles * pad))));

    piece->setArrayPos(calculateOffset(xCycles, yCycles));
    piece->setID(calculateOffset(xCycles, yCycles));
    piece->setCoordinates(xCycles, yCycles);
    piece->initNumLabel(pScene);
    piece->initListener(pScene);
}

void Puzzle::scaleImage(const Sprite* const spr)
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

PuzzlePiece &Puzzle::getPiece(const int piece)
{
    return *puzzlePieces[piece];
}

PuzzlePiece &Puzzle::getPiece(const Coordinate &coords)
{
    return getPiece(calculateOffset(coords));
}

PuzzlePiece &Puzzle::getPiece(const int x, const int y)
{
    return getPiece({ x, y });
}

std::vector<PuzzlePiece*> &Puzzle::getPieces()
{
    return puzzlePieces;
}

int Puzzle::calculateOffset(const int x, const int y) const
{
    return (y * GameSettings::getSegments().x) + x;
}

int Puzzle::calculateOffset(const Coordinate &coords) const
{
    return calculateOffset(coords.x, coords.y);
}

int Puzzle::getPadding() const
{
    return pad;
}

// Swaps the array position and coordinates of two pieces.
void Puzzle::swapPieces(const int fromPiece, const int toPiece)
{
    std::swap(puzzlePieces[fromPiece], puzzlePieces[toPiece]);

    // Save values temporarily.
    int fromArrayPos = puzzlePieces[fromPiece]->getArrayPos();
    int toArrayPos = puzzlePieces[toPiece]->getArrayPos();

    Coordinate fromCoords = puzzlePieces[fromPiece]->getCoordinates();
    Coordinate toCoords = puzzlePieces[toPiece]->getCoordinates();

    // Re-assign from temporary values.
    puzzlePieces[fromPiece]->setArrayPos(toArrayPos);
    puzzlePieces[toPiece]->setArrayPos(fromArrayPos);

    puzzlePieces[fromPiece]->setCoordinates(toCoords);
    puzzlePieces[toPiece]->setCoordinates(fromCoords);
}

void Puzzle::swapPieces(const PuzzlePiece &fromRef, const PuzzlePiece &toRef)
{
    swapPieces(fromRef.getArrayPos(), toRef.getArrayPos());
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

