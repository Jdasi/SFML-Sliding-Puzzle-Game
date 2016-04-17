#pragma once

#include "Puzzle.h"
#include "SimpleTimer.h"
#include "BoardManager.h"

#include "cocos2d.h"

class PuzzleGame : public cocos2d::Layer
{
    enum class Animation
    {
        slide,
        none
    };

public:
    PuzzleGame();
    ~PuzzleGame() = default;

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

    void update(float delta) override;

    void processMoves(MoveSequence &seq, Animation anim, const float speed = 0.0f);
    void performMoves(Node* const from, Node* const to, const MoveSequence &seq, 
                      Animation anim, const float speed) const;

    void updateMovesLabel(const int increment = 0);
    void updateTimeLabel();

    bool flipHintSwitch(cocos2d::Touch* const touch, cocos2d::Event* const event);
    void updateHintStatus();

    void rewardPlayer() const;
    void flashScreen();
    void endGame();
    void changeExistingElements();
    void createEndGameElements();

    void gotoMainMenu(Ref* const sender);
    void gotoPuzzleSelection(Ref* const sender);
    void recordTimerIfStarted();

    Puzzle puzzle;
    BoardManager boardManager;
    SimpleTimer timer;

    int startPosX;
    int startPosY;
    bool gameOver;
    bool started;
    int shuffleTimes;
    int computerMoves;
    int userMoves;
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