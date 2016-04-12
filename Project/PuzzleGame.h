#pragma once

#include "Puzzle.h"
#include "SimpleTimer.h"
#include "BoardManager.h"

#include "cocos2d.h"

class PuzzleGame : public cocos2d::Layer
{
public:
    PuzzleGame();
    static cocos2d::Scene *createScene();
    virtual bool init();

    bool interactWithPuzzle(cocos2d::Touch* const touch, cocos2d::Event* const event);

    CREATE_FUNC(PuzzleGame);

private:
    void initBackdrop();
    void initPuzzle();
    void initLabels();
    void initMenu();
    void initMenuPane();
    void initPreviewImage();
    void initHintSwitch();

    void performMoves(MoveSequence &seq, const float speed);
    void updateMovesLabel(const int increment = 0);
    void updateTimeLabel();

    bool flipHintSwitch(cocos2d::Touch* const touch, cocos2d::Event* const event);
    void updateHintStatus();

    void update(float delta) override;

    void rewardPlayer() const;
    void flashScreen();
    void endGame();
    void changeExistingElements();
    void createEndGameElements();

    void gotoMainMenu(Ref* const sender);
    void gotoPuzzleSelection(Ref* const sender);

    Puzzle puzzle;
    BoardManager boardManager;

    int startPosX;
    int startPosY;

    bool gameOver;
    int shuffleTimes;
    int computerMoves;
    int userMoves;
    SimpleTimer timer;
    bool showHints;
    bool usedHints;

    cocos2d::Label *movesLabel;
    cocos2d::Label *timeLabel;
    cocos2d::Label *previewLabel;
    cocos2d::Sprite *previewImage;
    cocos2d::MenuItemSprite *menuPuzzle;
    cocos2d::Sprite *hintSwitch;
    cocos2d::Label *switchLabel;

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};