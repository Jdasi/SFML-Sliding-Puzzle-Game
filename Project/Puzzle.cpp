#include "Puzzle.h"
#include "PuzzleGame.h"
#include "GameSettings.h"

USING_NS_CC;

Puzzle::Puzzle()
    : segmentsX(GameSettings::getSegments().x)
    , segmentsY(GameSettings::getSegments().y)
    , file(GameSettings::getImageName())
    , totalPieces(segmentsX * segmentsY)
    , pad(2)
    , imageSizeX(0.0f)
    , imageSizeY(0.0f)
    , scaleFactorX(1.0f)
    , scaleFactorY(1.0f)
    , puzzlePieces(new PuzzlePiece*[totalPieces])
{
}

Puzzle::~Puzzle()
{
    delete[] puzzlePieces;
}

// Loads the chosen image file and slices it according to the GameSettings specification.
void Puzzle::initPuzzle(PuzzleGame* const pScene, const Coordinate &startPos)
{
    Sprite *puzzle = Sprite::create(file);

    imageSizeX = puzzle->getContentSize().width;
    imageSizeY = puzzle->getContentSize().height;

    float secX = imageSizeX / segmentsX;
    float secY = imageSizeY / segmentsY;

    scaleImage(puzzle);
    sliceImage(pScene, startPos, secX, secY);

    // Hide bottom right puzzle piece.
    puzzlePieces[(segmentsX * segmentsY) - 1]->setBlankSpace(true);
}

void Puzzle::sliceImage(PuzzleGame* const pScene, const Coordinate& startPos,
                        const float secX, const float secY)
{
    for (int yCycles = 0; yCycles < segmentsY; ++yCycles)
    {
        for (int xCycles = 0; xCycles < segmentsX; ++xCycles)
        {
            PuzzlePiece *piece = PuzzlePiece::create(file, Rect
                (0 + (secX * xCycles), 0 + (secY * yCycles), secX, secY));

            configurePiece(piece, startPos, secX, secY, xCycles, yCycles, pScene);

            pScene->addChild(piece, 1);
            puzzlePieces[calculateOffset(xCycles, yCycles)] = piece;
        }
    }
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

    if (imageSizeX + paddingX != 800)
    {
        scaleFactorX = (800 - paddingX) / spr->getContentSize().width;
    }

    if (imageSizeX + paddingY != 600)
    {
        scaleFactorY = (600 - paddingY) / spr->getContentSize().height;
    }
}

PuzzlePiece &Puzzle::getPiece(const int piece) const
{
    return *puzzlePieces[piece];
}

PuzzlePiece &Puzzle::getPiece(const Coordinate &coords) const
{
    return getPiece(calculateOffset(coords));
}

PuzzlePiece &Puzzle::getPiece(const int x, const int y) const
{
    return getPiece({ x, y });
}

PuzzlePiece **Puzzle::getPieces() const
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

int Puzzle::getTotalPieces() const
{
    return totalPieces;
}

// Swaps the array position and coordinates of two pieces.
void Puzzle::swapPieces(const int fromPiece, const int toPiece) const
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

void Puzzle::swapPieces(const PuzzlePiece &fromRef, const PuzzlePiece &toRef) const
{
    swapPieces(fromRef.getArrayPos(), toRef.getArrayPos());
}

bool Puzzle::isPuzzleComplete() const
{
    int correctPieces = 0;
    for (int i = 0; i < totalPieces; ++i)
    {
        if (puzzlePieces[i]->getArrayPos() == puzzlePieces[i]->getID())
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

