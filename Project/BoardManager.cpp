#include "BoardManager.h"
#include "GameSettings.h"
#include "Puzzle.h"
#include "PuzzlePiece.h"
#include "MoveSequence.h"

USING_NS_CC;

BoardManager::BoardManager(Puzzle &ref)
    : blankSpace(0)
    , blankSpaceCoords({ 0, 0 }) 
    , puzzle(ref)
    , currentPieceArrayPos(0)
{
}

bool BoardManager::sanityCheckMove(cocos2d::Rect &rect, cocos2d::Touch &touch, 
                                   PuzzlePiece *piece)
{
    if (piece == nullptr)
    {
        return false;
    }

    if (!rect.containsPoint(touch.getLocation()))
    {
        return false;
    }

    if (piece->isBlankSpace())
    {
        return false;
    }

    if (puzzle.getPiece(blankSpace).getNumberOfRunningActions() != 0)
    {
        return false;
    }

    return true;
}

bool BoardManager::generateTileMoves(MoveSequence &seq, PuzzlePiece *piece)
{
    currentPieceArrayPos = piece->getArrayPos();
    Coordinate currPieceCoords = piece->getCoordinates();
    updateBlankspaceInfo();

    if (currPieceCoords.x == blankSpaceCoords.x)
    {
        if (currPieceCoords.y > blankSpaceCoords.y)
        {
            seq.yMoveDist = piece->getBoundingBox().size.height + puzzle.getPadding();
            generateMove(seq, SlideDirection::up);
        }
        else
        {
            seq.yMoveDist = -(piece->getBoundingBox().size.height + puzzle.getPadding());
            generateMove(seq, SlideDirection::down);
        }

        return true;
    }

    if (currPieceCoords.y == blankSpaceCoords.y)
    {
        if (currPieceCoords.x > blankSpaceCoords.x)
        {
            seq.xMoveDist = -(piece->getBoundingBox().size.width + puzzle.getPadding());
            generateMove(seq, SlideDirection::left);
        }
        else
        {
            seq.xMoveDist = piece->getBoundingBox().size.width + puzzle.getPadding();
            generateMove(seq, SlideDirection::right);
        }

        return true;
    }

    return false;
}

void BoardManager::generateMove(MoveSequence &seq, SlideDirection dir)
{
    switch (dir)
    {
        case SlideDirection::up:
        {
            for (int i = puzzle.getPiece(currentPieceArrayPos).getCoordinates().y;
                 i > blankSpaceCoords.y; --i)
            {
                if (!pushBackTilesToBeMoved(seq.pieceContainer, seq.labelContainer, 
                                           { blankSpaceCoords.x, i }))
                {
                    break;
                }
            }

            break;
        }
        case SlideDirection::down:
        {
            for (int i = puzzle.getPiece(currentPieceArrayPos).getCoordinates().y;
                 i < blankSpaceCoords.y; ++i)
            {
                if (!pushBackTilesToBeMoved(seq.pieceContainer, seq.labelContainer,
                                           { blankSpaceCoords.x, i }))
                {
                    break;
                }
            }

            break;
        }
        case SlideDirection::left:
        {
            for (int i = puzzle.getPiece(currentPieceArrayPos).getCoordinates().x;
                 i > blankSpaceCoords.x; --i)
            {
                if (!pushBackTilesToBeMoved(seq.pieceContainer, seq.labelContainer,
                                           { i, blankSpaceCoords.y }))
                {
                    break;
                }
            }

            break;
        }
        case SlideDirection::right:
        {
            for (int i = puzzle.getPiece(currentPieceArrayPos).getCoordinates().x;
                 i < blankSpaceCoords.x; ++i)
            {
                if (!pushBackTilesToBeMoved(seq.pieceContainer, seq.labelContainer,
                                           { i, blankSpaceCoords.y }))
                {
                    break;
                }
            }

            break;
        }
        default:
        {
            throw std::runtime_error("Error in generateMove");
        }
    }

}

bool BoardManager::pushBackTilesToBeMoved(std::vector<PuzzlePiece*> &pieceContainer, 
                                          std::vector<Label*> &labelContainer,
                                          Coordinate pos)
{
    PuzzlePiece &pieceRef = puzzle.getPiece(pos);
    if (pieceRef.isBlankSpace())
    {
        return false;
    }

    pieceContainer.push_back(&pieceRef);
    labelContainer.push_back(pieceRef.getNumLabel());
    return true;
}

void BoardManager::generateRandomMoves(int times)
{
    for (int i = 0; i < times; ++i)
    {
        updateBlankspaceInfo();

        bool tileSwapped = false;
        while (!tileSwapped)
        {
            SlideDirection direction = static_cast<SlideDirection>(rand() % 4);

            switch (direction)
            {
                case SlideDirection::right:
                {
                    if (tryComputerMove(blankSpace, 
                                       { blankSpaceCoords.x + 1, blankSpaceCoords.y }))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                case SlideDirection::down:
                {
                    if (tryComputerMove(blankSpace,
                                       { blankSpaceCoords.x , blankSpaceCoords.y + 1 }))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                case SlideDirection::left:
                {
                    if (tryComputerMove(blankSpace,
                                       { blankSpaceCoords.x - 1, blankSpaceCoords.y }))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                case SlideDirection::up:
                {
                    if (tryComputerMove(blankSpace,
                                       { blankSpaceCoords.x , blankSpaceCoords.y - 1 }))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                default: {}
            }
        }
    }
}

bool BoardManager::tryComputerMove(int fromPiece, Coordinate coords)
{
    if (!puzzle.inBounds(coords.x, coords.y))
    {
        return false;
    }

    PuzzlePiece &fromPieceRef = puzzle.getPiece(fromPiece);
    PuzzlePiece &toPieceRef = puzzle.getPiece(coords.x, coords.y);

    computerMovePiece(fromPieceRef, toPieceRef);
    computerMovePieceLabel(fromPieceRef, coords);

    puzzle.swapPieces(fromPieceRef, toPieceRef);

    return true;
}

void BoardManager::computerMovePiece(PuzzlePiece &fromPieceRef, PuzzlePiece &toPieceRef)
    const
{
    Vec2 pieceFromPos = fromPieceRef.Node::getPosition();
    Vec2 pieceToPos = toPieceRef.Node::getPosition();

    fromPieceRef.Sprite::setPosition(pieceToPos);
    toPieceRef.Sprite::setPosition(pieceFromPos);
}

void BoardManager::computerMovePieceLabel(PuzzlePiece &fromPieceRef, Coordinate coords)
{
    Label *fromPieceLabelPtr = fromPieceRef.getNumLabel();
    Label *toPieceLabelPtr = puzzle.getPiece(coords.x, coords.y).getNumLabel();

    Vec2 labelFromPos = fromPieceLabelPtr->getPosition();
    Vec2 labelToPos = toPieceLabelPtr->getPosition();

    fromPieceLabelPtr->setPosition(labelToPos);
    toPieceLabelPtr->setPosition(labelFromPos);
}

int BoardManager::findBlankSpace() const
{
    std::vector<PuzzlePiece*> &piecesRef = puzzle.getPieces();
    for (PuzzlePiece *piece : piecesRef)
    {
        if (piece->isBlankSpace())
        {
            return piece->getArrayPos();
        }
    }

    throw std::runtime_error("Error in findBlankSpace");
}

void BoardManager::updateBlankspaceInfo()
{
    blankSpace = findBlankSpace();
    blankSpaceCoords = puzzle.getPiece(blankSpace).getCoordinates();
}

void BoardManager::moveBlankSpaceToStart()
{
    for (int i = 0; i < GameSettings::getSegments().x; ++i)
    {
        updateBlankspaceInfo();

        if (!tryComputerMove(blankSpace, { blankSpaceCoords.x + 1, blankSpaceCoords.y }))
        {
            break;
        }
    }

    for (int i = 0; i < GameSettings::getSegments().y; ++i)
    {
        updateBlankspaceInfo();

        if (!tryComputerMove(blankSpace, { blankSpaceCoords.x , blankSpaceCoords.y + 1 }))
        {
            break;
        }
    }
}

void BoardManager::swapPieces(int fromPiece, int toPiece)
{
    puzzle.swapPieces(fromPiece, toPiece);
}

bool BoardManager::isPuzzleComplete() const
{
    return puzzle.isPuzzleComplete();
}

void BoardManager::hideAllPieces(bool hide) const
{
    std::vector<PuzzlePiece*> &piecesRef = puzzle.getPieces();

    for (PuzzlePiece *piece : piecesRef)
    {
        if (hide)
        {
            piece->setBlankSpace(true);
        }
        else
        {
            piece->setBlankSpace(false);
        }
    }
}

void BoardManager::enableAllLabels(bool enable) const
{
    std::vector<PuzzlePiece*> &piecesRef = puzzle.getPieces();

    for (PuzzlePiece *piece : piecesRef)
    {
        if (piece->isBlankSpace())
        {
            continue;
        }

        if (enable)
        {
            piece->enableLabel(true);
        }
        else
        {
            piece->enableLabel(false);
        }
    }
}