#pragma once
#include "Project/Puzzle.h"

#include <cocos2d.h>

class PuzzleScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene *createScene();
    virtual bool init();

    bool interactWithPuzzle(cocos2d::Touch *touch, cocos2d::Event* event);

    // implement the "static create()" method manually
    CREATE_FUNC(PuzzleScene);

private:
    void initBackground();
    void initPuzzle();
    void initLabels();

    void updateMovesLabel(int increment);

    void generateValidMove();
    void generateRandomValidMoves(int times);

    bool tryUserMove(int fromPiece, int toX, int toY);
    bool tryComputerMove(int fromPiece, int toX, int toY);

    void moveBlankSpaceToStart();

    void gotoMainMenu(cocos2d::Ref *sender);

    cocos2d::Size visibleSize { cocos2d::Director::getInstance()->getVisibleSize() };
    Puzzle puzzle;

    int startPosX;
    int startPosY;

    int currentPieceTag;

    bool gameOver;
    int numMoves;
    cocos2d::Label *movesLabel;
};