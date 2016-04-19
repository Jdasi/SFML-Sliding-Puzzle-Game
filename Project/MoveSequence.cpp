#include "MoveSequence.h"

MoveSequence::MoveSequence()
    : sz(0)
    , moveDistX(0.0f)
    , moveDistY(0.0f)
    , pieceContainer(nullptr)
{
}

MoveSequence::~MoveSequence()
{
    deleteContainer();
}

void MoveSequence::deleteContainer() const
{
    if (pieceContainer != nullptr)
    {
        delete[] pieceContainer;
    }
}

int MoveSequence::size() const
{
    return sz;
}

PuzzlePiece **MoveSequence::getPieceContainer() const
{
    return pieceContainer;
}

float MoveSequence::getMoveDistX() const
{
    return moveDistX;
}

void MoveSequence::setMoveDistX(const float x)
{
    moveDistX = x;
}

float MoveSequence::getMoveDistY() const
{
    return moveDistY;
}

void MoveSequence::setMoveDistY(const float y)
{
    moveDistY = y;
}

void MoveSequence::setMoveDist(const float x, const float y)
{
    moveDistX = x;
    moveDistY = y;
}

void MoveSequence::pushBack(PuzzlePiece* piece)
{
    PuzzlePiece **temp = new PuzzlePiece*[sz];

    for (int i = 0; i < sz; ++i)
    {
        temp[i] = pieceContainer[i];
    }

    deleteContainer();

    pieceContainer = new PuzzlePiece*[sz + 1];

    for (int i = 0; i < sz; ++i)
    {
        pieceContainer[i] = temp[i];
    }

    delete[] temp;
    pieceContainer[sz] = piece;

    ++sz;
}