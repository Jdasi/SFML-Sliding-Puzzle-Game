#include "BoardManager.h"
#include "GameSettings.h"
#include "Puzzle.h"
#include "PuzzlePiece.h"

USING_NS_CC;

BoardManager::BoardManager(Puzzle &ref, int &blankSpaceRef)
    : blankSpace(blankSpaceRef)
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

bool BoardManager::generateTileMoves(std::vector<PuzzlePiece*> &piecesToMove, 
                                     PuzzlePiece *piece, float &xMoveDist, 
                                     float &yMoveDist)
{
    currentPieceArrayPos = piece->getArrayPos();
    coordinate currPieceCoords = piece->getCoordinates();
    updateBlankspaceInfo();

    if (currPieceCoords.x == blankSpaceCoords.x)
    {
        if (currPieceCoords.y > blankSpaceCoords.y)
        {
            yMoveDist = piece->getBoundingBox().size.height + puzzle.getPadding();
            generateMove(piecesToMove, SlideDirection::up);
        }
        else
        {
            yMoveDist = -(piece->getBoundingBox().size.height + puzzle.getPadding());
            generateMove(piecesToMove, SlideDirection::down);
        }

        return true;
    }

    if (currPieceCoords.y == blankSpaceCoords.y)
    {
        if (currPieceCoords.x > blankSpaceCoords.x)
        {
            xMoveDist = -(piece->getBoundingBox().size.width + puzzle.getPadding());
            generateMove(piecesToMove, SlideDirection::left);
        }
        else
        {
            xMoveDist = piece->getBoundingBox().size.width + puzzle.getPadding();
            generateMove(piecesToMove, SlideDirection::right);
        }

        return true;
    }

    return false;
}

void BoardManager::generateMove(std::vector<PuzzlePiece*> &piecesToMove, 
                                SlideDirection dir)
{
    switch (dir)
    {
        case SlideDirection::up:
        {
            for (int i = puzzle.getPiece(currentPieceArrayPos).getCoordinates().y;
            i > blankSpaceCoords.y; --i)
            {
                if (!pushBackTilesToBeMoved(piecesToMove, { blankSpaceCoords.x, i }))
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
                if (!pushBackTilesToBeMoved(piecesToMove, { blankSpaceCoords.x, i }))
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
                if (!pushBackTilesToBeMoved(piecesToMove, { i, blankSpaceCoords.y }))
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
                if (!pushBackTilesToBeMoved(piecesToMove, { i, blankSpaceCoords.y }))
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

bool BoardManager::pushBackTilesToBeMoved(std::vector<PuzzlePiece*> &container, 
                                          coordinate pos)
{
    PuzzlePiece &pieceRef = puzzle.getPiece(pos);
    if (pieceRef.isBlankSpace())
    {
        return false;
    }

    container.push_back(&pieceRef);
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
                        blankSpaceCoords.x + 1, blankSpaceCoords.y))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                case SlideDirection::down:
                {
                    if (tryComputerMove(blankSpace,
                        blankSpaceCoords.x, blankSpaceCoords.y + 1))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                case SlideDirection::left:
                {
                    if (tryComputerMove(blankSpace,
                        blankSpaceCoords.x - 1, blankSpaceCoords.y))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                case SlideDirection::up:
                {
                    if (tryComputerMove(blankSpace,
                        blankSpaceCoords.x, blankSpaceCoords.y - 1))
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

bool BoardManager::tryComputerMove(int fromPiece, int toX, int toY)
{
    if (!puzzle.inBounds(toX, toY))
    {
        return false;
    }

    PuzzlePiece &fromPieceRef = puzzle.getPiece(fromPiece);
    PuzzlePiece &toPieceRef = puzzle.getPiece(toX, toY);

    Vec2 fromPos = fromPieceRef.getPosition();
    Vec2 toPos = toPieceRef.getPosition();

    fromPieceRef.setPosition(toPos);
    toPieceRef.setPosition(fromPos);

    puzzle.swapPieces(fromPieceRef, toPieceRef);

    return true;
}

void BoardManager::updateBlankspaceInfo()
{
    blankSpace = puzzle.findBlankSpace();
    blankSpaceCoords = puzzle.getPiece(blankSpace).getCoordinates();
}

void BoardManager::moveBlankSpaceToStart()
{
    for (int i = 0; i < GameSettings::getSegments().x; ++i)
    {
        updateBlankspaceInfo();

        if (!tryComputerMove(blankSpace, blankSpaceCoords.x + 1, blankSpaceCoords.y))
        {
            break;
        }
    }

    for (int i = 0; i < GameSettings::getSegments().y; ++i)
    {
        updateBlankspaceInfo();

        if (!tryComputerMove(blankSpace, blankSpaceCoords.x, blankSpaceCoords.y + 1))
        {
            break;
        }
    }
}