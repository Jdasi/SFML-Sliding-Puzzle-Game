#include "Project/PuzzleScene.h"

#include "Project/GameSettings.h"

USING_NS_CC;

cocos2d::Scene *PuzzleScene::createScene()
{
    // 'scene' & 'layer' are in autoreleased objects
    cocos2d::Scene *scene = Scene::create();
    auto layer = PuzzleScene::create();

     // add layer as a child to scene
     scene->addChild(layer);

     // return the scene
     return scene;
}

bool PuzzleScene::init()
{
    startPosX = 100;
    startPosY = 100;

    currentPieceTag = 0;
    gameOver = false;
    numMoves = 0;

    initBackground();
    initPuzzle();

    return true;
}

void PuzzleScene::initBackground()
{
    auto backdrop = Sprite::create("backdrop.jpg");

    backdrop->setScale(1);
    backdrop->setAnchorPoint(Vec2(0, 0));
    backdrop->setPosition(Vec2(0, 0));

    this->addChild(backdrop, 0);
}

void PuzzleScene::initPuzzle()
{
    puzzle.initPuzzle(this, startPosX, startPosY);
}

bool PuzzleScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    PuzzlePiece *piece = static_cast<PuzzlePiece*>(event->getCurrentTarget());

    if (piece->isBlankSpace())
    {
        return false;
    }

    Point pt = touch->getLocation();
    Rect recTemp = piece->getBoundingBox();

    if (!recTemp.containsPoint(pt))
    {
        return false;
    }

    currentPieceTag = piece->getTag();

    generateValidMove();

    /*
    cocos2d::JumpBy *jump = JumpBy::create(0.5f, Vec2(0, 0), 100, 1);
    piece->runAction(jump);
    */

    return true;
}

void PuzzleScene::generateValidMove()
{
    coordinate currPiece = puzzle.calculateCoordinates(currentPieceTag);

    if (tryMove(currentPieceTag, currPiece.x + 1, currPiece.y))
    {
        return;
    }

    if (tryMove(currentPieceTag, currPiece.x, currPiece.y + 1))
    {
        return;
    }
    
    if (tryMove(currentPieceTag, currPiece.x - 1, currPiece.y))
    {
        return;
    }
    
    if (tryMove(currentPieceTag, currPiece.x, currPiece.y - 1))
    {
        return;
    }
}

bool PuzzleScene::tryMove(int fromPiece, int toX, int toY)
{
    int toPiece = puzzle.calculateOffset(toX, toY);

    if (!puzzle.isPieceBlankSpace(toPiece))
    {
        return false;
    }

    float speed = 0.1f;

    MoveTo *action1 = MoveTo::create(speed, puzzle.getPiece(toPiece).getPosition());
    MoveTo *action2 = MoveTo::create(speed, puzzle.getPiece(fromPiece).getPosition());

    puzzle.getPiece(fromPiece).runAction(action1);
    puzzle.getPiece(toPiece).runAction(action2);

    puzzle.swapPieces(fromPiece, toPiece);

    return true;
}