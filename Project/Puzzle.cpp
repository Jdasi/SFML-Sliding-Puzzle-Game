#include "Project/Puzzle.h"
#include "Project/PuzzleScene.h"
#include "Project/GameSettings.h"

USING_NS_CC;

Puzzle::Puzzle()
    : sizeX(0)
    , sizeY(0)
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

    float sizeX = puzzle->getContentSize().width;
    float sizeY = puzzle->getContentSize().height;

    float secX = sizeX / segmentsX;
    float secY = sizeY / segmentsY;

    int pad = 2;

    for (int yCycles = 0; yCycles < segmentsY; ++yCycles)
    {
        for (int xCycles = 0; xCycles < segmentsX; ++xCycles)
        {
            PuzzlePiece *piece = PuzzlePiece::create(file, Rect
                (0 + (secX * xCycles), 0 + (secY * yCycles), secX, secY));

            // set anchor point to top left of image
            piece->setAnchorPoint(Vec2(0, 1));
            piece->setPosition(Vec2(startPosX + ((secX * xCycles) + (xCycles * pad)), 
                visibleSize.height - startPosY - ((secY * yCycles) + (yCycles * pad))));
            piece->setTag(((yCycles * segmentsX) + xCycles) + 1);

            pScene->addChild(piece, (yCycles * segmentsX) + xCycles);
            
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan = CC_CALLBACK_2(PuzzleScene::onTouchBegan, pScene);
            _eventDispatcher->
                addEventListenerWithSceneGraphPriority(listener, piece);

            puzzlePieces.push_back(piece);
        }
    }

    srand(static_cast<int>(time(nullptr)));
    //int affectedPiece = ((rand() % segmentsY) * segmentsX) + (rand() % segmentsX);
    int affectedPiece = (segmentsX * segmentsY) - 1;

    puzzlePieces[affectedPiece]->setOpacity(0);
    puzzlePieces[affectedPiece]->setBlankSpace(true);
    puzzlePieces[affectedPiece]->setTag(0);
}

int Puzzle::calculateOffset(int x, int y)
{
    return (position.y * GameSettings::getSegments().x) + position.x;
}