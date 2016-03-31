#pragma once

#include "Puzzle.h"
#include "GameSettings.h"

#include <cocos2d.h>

class PuzzleGame : public cocos2d::Layer
{
public:
    static cocos2d::Scene *createScene();
    virtual bool init();

    bool interactWithPuzzle(cocos2d::Touch *touch, cocos2d::Event* event);

    CREATE_FUNC(PuzzleGame);

private:
    enum class SlideDirection
    {
        up = 0,
        right,
        down,
        left
    };

    void initBackdrop();
    void initPuzzle();
    void initLabels();
    void initMenu();
    void initPreviewImage();

    void updateMovesLabel(int increment = 0);

    void generateValidMove();
    void generateRandomValidMoves(int times);

    bool tryUserMove(int fromPiece, int toX, int toY);
    bool tryComputerMove(int fromPiece, int toX, int toY);

    void updateBlankspaceInfo();
    void moveBlankSpaceToStart();

    void gotoMainMenu(cocos2d::Ref *sender);
    void gotoPuzzleSelection(cocos2d::Ref *sender);

    cocos2d::Size visibleSize { cocos2d::Director::getInstance()->getVisibleSize() };
    Puzzle puzzle;

    int startPosX;
    int startPosY;

    int currentPieceTag;

    int blankSpace;
    static coordinate blankSpaceCoords;

    bool gameOver;
    int numMoves;
    cocos2d::Label *movesLabel;
};