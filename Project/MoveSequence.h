#pragma once

#include "Coordinate.h"

class PuzzlePiece;

class MoveSequence
{
public:
    MoveSequence();
    ~MoveSequence();

    int size() const;
    PuzzlePiece **getPieceContainer() const;

    float getMoveDistX() const;
    void setMoveDistX(const float x);

    float getMoveDistY() const;
    void setMoveDistY(const float y);

    void setMoveDist(const float x, const float y);

    void pushBack(PuzzlePiece *piece);

private:
    void deleteContainer() const;

    int sz;
    PuzzlePiece **pieceContainer;
    float moveDistX;
    float moveDistY;
};