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

bool BoardManager::sanityCheckMove(Rect &rect, Touch &touch, PuzzlePiece* const piece)
                                   const
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

// Identifies a valid move by comparing the coordinates of the piece and blankspace.
bool BoardManager::generateTileMoves(MoveSequence &seq, PuzzlePiece* const piece)
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

// Identifies the pieces that are to be affected when a piece is clicked.
bool BoardManager::generateMove(MoveSequence &seq, const SlideDirection dir)
{
    switch (dir)
    {
        case SlideDirection::up:
        {
            for (int i = puzzle.getPiece(currentPieceArrayPos).getCoordinates().y;
                 i > blankSpaceCoords.y; --i)
            {
                if (!pushBackTilesToBeMoved(seq, { blankSpaceCoords.x, i }))
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
                if (!pushBackTilesToBeMoved(seq, { blankSpaceCoords.x, i }))
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
                if (!pushBackTilesToBeMoved(seq, { i, blankSpaceCoords.y }))
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
                if (!pushBackTilesToBeMoved(seq, { i, blankSpaceCoords.y }))
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

    if (seq.pieceContainer.size() == 0)
    {
        return false;
    }
    
    return true;
}

// Main method for shuffling tiles on puzzle initialisation.
bool BoardManager::generateRandomMove(MoveSequence &seq)
{
    PuzzlePiece &blankSpaceRef = puzzle.getPiece(blankSpace);
    if (blankSpaceRef.getNumberOfRunningActions() != 0)
    {
        return false;
    }

    bool tilePushed = false;
    while (!tilePushed)
    {
        SlideDirection direction = static_cast<SlideDirection>(rand() % 4);
        Coordinate attempt { 0, 0 };

        seq.xMoveDist = 0;
        seq.yMoveDist = 0;

        switch (direction)
        {
            case SlideDirection::up:
            {
                attempt = { blankSpaceCoords.x, blankSpaceCoords.y - 1 };

                seq.yMoveDist = -(blankSpaceRef.getBoundingBox().size.height + puzzle.getPadding());
                direction = SlideDirection::down;

                break;
            }
            case SlideDirection::down:
            {
                attempt = { blankSpaceCoords.x, blankSpaceCoords.y + 1 };

                seq.yMoveDist = blankSpaceRef.getBoundingBox().size.height + puzzle.getPadding();
                direction = SlideDirection::up;

                break;
            }
            case SlideDirection::left:
            {
                attempt = { blankSpaceCoords.x - 1, blankSpaceCoords.y };
                seq.xMoveDist = blankSpaceRef.getBoundingBox().size.width + puzzle.getPadding();

                direction = SlideDirection::right;

                break;
            }
            case SlideDirection::right:
            {
                attempt = { blankSpaceCoords.x + 1, blankSpaceCoords.y };

                seq.xMoveDist = -(blankSpaceRef.getBoundingBox().size.width + puzzle.getPadding());
                direction = SlideDirection::left;

                break;
            }
            default: {}
        }

        if (!puzzle.inBounds(attempt.x, attempt.y))
        {
            continue;
        }

        currentPieceArrayPos = puzzle.calculateOffset(attempt);

        if (generateMove(seq, direction))
        {
            tilePushed = true;
        }
    }

    return true;
}

// Adds all non-blankspace pieces to the vector so they are ready to be moved.
bool BoardManager::pushBackTilesToBeMoved(MoveSequence &seq, const Coordinate pos) const
{
    PuzzlePiece &pieceRef = puzzle.getPiece(pos);
    if (pieceRef.isBlankSpace())
    {
        return false;
    }

    seq.pieceContainer.push_back(&pieceRef);
    seq.labelContainer.push_back(pieceRef.getNumLabel());

    return true;
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
        MoveSequence seq;

        if (!generateMove(seq, SlideDirection::right))
        {
            break;
        }
    }

    for (int i = 0; i < GameSettings::getSegments().y; ++i)
    {
        updateBlankspaceInfo();
        MoveSequence seq;

        if (!generateMove(seq, SlideDirection::down))
        {
            break;
        }
    }
}

void BoardManager::swapPieces(const int fromPiece, const int toPiece) const
{
    puzzle.swapPieces(fromPiece, toPiece);
}

bool BoardManager::isPuzzleComplete() const
{
    return puzzle.isPuzzleComplete();
}

void BoardManager::hideAllPieces(const bool hide) const
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

void BoardManager::enableAllLabels(const bool enable) const
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