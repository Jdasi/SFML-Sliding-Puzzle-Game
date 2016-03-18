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

    currentTileTag = 0;
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
    if (event->getCurrentTarget()->getTag() > 0)
    {
        PuzzlePiece *piece = static_cast<PuzzlePiece*>(event->getCurrentTarget());

        Point pt = touch->getLocation();
        Rect recTemp = piece->getBoundingBox();

        if (!recTemp.containsPoint(pt))
        {
            return false;
        }

        currentTileTag = piece->getTag();

        cocos2d::JumpBy *jump = JumpBy::create(0.5f, Vec2(0, 0), 100, 1);
        piece->runAction(jump);
        
        //swapTile(sprite);
    }

    return true;
}

void PuzzleScene::swapTile(cocos2d::Sprite *spr)
{
    
}