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

    void generateValidMove();
    bool tryMove(int fromPiece, int toX, int toY);

    cocos2d::Size visibleSize { cocos2d::Director::getInstance()->getVisibleSize() };
    Puzzle puzzle;

    int startPosX;
    int startPosY;

    int currentPieceTag;
    bool gameOver;
    int numMoves;
};