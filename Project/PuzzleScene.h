#pragma once
#include "Project/Puzzle.h"

#include <cocos2d.h>

class PuzzleScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene *createScene();
    virtual bool init();

    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event* event) override;

    // implement the "static create()" method manually
    CREATE_FUNC(PuzzleScene);

private:
    void initBackground();
    void initPuzzle();


    void swapTile(cocos2d::Sprite *spr);

    cocos2d::Size visibleSize { cocos2d::Director::getInstance()->getVisibleSize() };
    Puzzle puzzle;

    int startPosX;
    int startPosY;

    int currentTileTag;
    bool gameOver;
    int numMoves;
};