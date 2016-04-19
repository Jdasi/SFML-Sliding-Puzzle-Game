#include "BoardManager.h"
#include "Puzzle.h"
#include "PuzzlePiece.h"
#include "MoveSequence.h"
#include "GameExceptions.h"
#include "GameSettings.h"

USING_NS_CC;

BoardManager::BoardManager(Puzzle &ref)
    : blankSpace(0)
    , blankSpaceCoords({ 0, 0 }) 
    , puzzle(ref)
    , currentPieceArrayPos(0)
    , lastDir(SlideDirection::down)
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

/* Compares the coordinates of the passed PuzzlePiece and the BlankSpace.
 * If any similarities are found in either X or Y, a SlideDirection is generated.
 */
bool BoardManager::identifyMoves(MoveSequence &seq, PuzzlePiece* const piece)
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

/* Uses the SlideDirection to determine which direction to search for the BlankSpace.
 * Any pieces identified in the search are pushed back into a vector to be processed.
 */ 
bool BoardManager::generateMove(MoveSequence &seq, const SlideDirection &dir) const
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
            throw Utility::MoveGenerationException();
        }
    }

    if (seq.pieceContainer.size() == 0)
    {
        return false;
    }
    
    return true;
}

/* The computer-generated method for shuffling the board.
 * Because we follow the rules of the game to scramble the board, we ensure the puzzle
 * is always solvable.
 */
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
        if (direction == lastDir)
        {
            continue;
        }

        Coordinate attempt { 0, 0 };
        seq.xMoveDist = 0;
        seq.yMoveDist = 0;

        switch (direction)
        {
            case SlideDirection::up:
            {
                attempt = { blankSpaceCoords.x, blankSpaceCoords.y - 1 };

                seq.yMoveDist = -(blankSpaceRef.getBoundingBox().size.height + 
                    puzzle.getPadding());
                direction = SlideDirection::down;

                break;
            }
            case SlideDirection::down:
            {
                attempt = { blankSpaceCoords.x, blankSpaceCoords.y + 1 };

                seq.yMoveDist = blankSpaceRef.getBoundingBox().size.height + 
                    puzzle.getPadding();
                direction = SlideDirection::up;

                break;
            }
            case SlideDirection::left:
            {
                attempt = { blankSpaceCoords.x - 1, blankSpaceCoords.y };
                seq.xMoveDist = blankSpaceRef.getBoundingBox().size.width + 
                    puzzle.getPadding();

                direction = SlideDirection::right;

                break;
            }
            case SlideDirection::right:
            {
                attempt = { blankSpaceCoords.x + 1, blankSpaceCoords.y };

                seq.xMoveDist = -(blankSpaceRef.getBoundingBox().size.width + 
                    puzzle.getPadding());
                direction = SlideDirection::left;

                break;
            }
            default:
            {
                throw Utility::MoveGenerationException();
            }
        }

        if (!inBounds(attempt.x, attempt.y))
        {
            continue;
        }

        currentPieceArrayPos = puzzle.calculateOffset(attempt);

        if (generateMove(seq, direction))
        {
            tilePushed = true;
        }

        lastDir = direction;
    }

    return true;
}

bool BoardManager::pushBackTilesToBeMoved(MoveSequence &seq, const Coordinate pos) const
{
    PuzzlePiece &pieceRef = puzzle.getPiece(pos);
    if (pieceRef.isBlankSpace())
    {
        return false;
    }

    seq.pieceContainer.push_back(&pieceRef);

    return true;
}

int BoardManager::findBlankSpace() const
{
    PuzzlePiece **piecesRef = puzzle.getPieces();
    int totalPieces = puzzle.getTotalPieces();

    for (int i = 0; i < totalPieces; ++i)
    {
        if (piecesRef[i]->isBlankSpace())
        {
            return piecesRef[i]->getArrayPos();
        }
    }

    throw Utility::FindBlankSpaceException();
}

void BoardManager::updateBlankspaceInfo()
{
    blankSpace = findBlankSpace();
    blankSpaceCoords = puzzle.getPiece(blankSpace).getCoordinates();
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
    PuzzlePiece **piecesRef = puzzle.getPieces();
    int totalPieces = puzzle.getTotalPieces();

    for (int i = 0; i < totalPieces; ++i)
    {
        piecesRef[i]->setBlankSpace(hide);
    }
}

void BoardManager::enableAllLabels(const bool enable) const
{
    PuzzlePiece **piecesRef = puzzle.getPieces();
    int totalPieces = puzzle.getTotalPieces();
    
    for (int i = 0; i < totalPieces; ++i)
    {
        if (piecesRef[i]->isBlankSpace())
        {
            continue;
        }

        piecesRef[i]->enableLabel(enable);
    }
}

bool BoardManager::inBounds(const int x, const int y) const
{
    if ((x >= GameSettings::getSegments().x || x < 0) ||
        (y >= GameSettings::getSegments().y || y < 0))
    {
        return false;
    }

    return true;
}