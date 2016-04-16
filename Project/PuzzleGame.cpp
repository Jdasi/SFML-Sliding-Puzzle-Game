#include "PuzzleGame.h"
#include "GameSettings.h"
#include "GameProfile.h"
#include "MainMenu.h"
#include "PuzzleSelection.h"
#include "MoveSequence.h"
#include "TimeUtils.h"

USING_NS_CC;

PuzzleGame::PuzzleGame()
    : boardManager(puzzle)
    , startPosX(0)
    , startPosY(0)
    , gameOver(false)
    , started(false)
    , shuffleTimes(0)
    , computerMoves(0)
    , userMoves(0)
    , showHints(false)
    , usedHints(false)
    , movesLabel(nullptr)
    , timeLabel(nullptr)
    , previewLabel(nullptr)
    , previewImage(nullptr)
    , menuPuzzle(nullptr)
    , hintSwitch(nullptr)
    , switchLabel(nullptr)
{
}

Scene *PuzzleGame::createScene()
{
    Scene *scene = Scene::create();
    auto layer = PuzzleGame::create();

    scene->addChild(layer);

    return scene;
}

bool PuzzleGame::init()
{
    if (!Layer::init())
    {
        return false;
    }

    initBackdrop();
    initPuzzle();
    initLabels();
    initMenu();
    initMenuPane();
    initPreviewImage();
    initHintSwitch();

    GameProfile::modifyProfileStat(ProfileStat::puzzlesAttempted, 1);

    this->scheduleUpdate();

    return true;
}

void PuzzleGame::initBackdrop()
{
    Sprite *backdrop = Sprite::create
        ("backdrops/" + GameProfile::getCurrentBackground() + "_puzzle.jpg");

    backdrop->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    this->addChild(backdrop, 0);
}

void PuzzleGame::initPuzzle()
{
    startPosX = 100;
    startPosY = visibleSize.height - 80;
    puzzle.initPuzzle(this, { startPosX, startPosY });
    boardManager.updateBlankspaceInfo();

    int product = GameSettings::getSegments().x * GameSettings::getSegments().y;
    shuffleTimes = (product * product) * 0.5;

    // Constrain shuffleTimes to a value between 50-300.
    shuffleTimes = shuffleTimes < 50 ? 50 : shuffleTimes;
    shuffleTimes = shuffleTimes > 300 ? 300 : shuffleTimes;

    // Shuffling without animation.
    if (!GameProfile::animatedShufflingEnabled())
    {
        for (int i = 0; i < shuffleTimes; ++i)
        {
            MoveSequence seq;
            if (boardManager.generateRandomMove(seq))
            {
                processMoves(seq, Animation::none);
            }
        }

        timer.startTimer();
        started = true;
    }
}

void PuzzleGame::initLabels()
{
    movesLabel = Label::createWithTTF("Moves: ", GameSettings::getFontName(), 30);
    movesLabel->setPosition(Vec2(visibleSize.width - 210, (visibleSize.height / 2) - 20));

    updateMovesLabel();

    timeLabel = Label::createWithTTF("Time: 00:00:00", GameSettings::getFontName(), 30);
    timeLabel->setPosition(Vec2(visibleSize.width - 210, (visibleSize.height / 2) + 20));

    this->addChild(movesLabel, 2);
    this->addChild(timeLabel, 2);
}

void PuzzleGame::initMenu()
{
    menuPuzzle = MenuItemSprite::create(
        Sprite::create("utility/change_up.png"),
        Sprite::create("utility/change_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleGame::gotoPuzzleSelection, this));

    MenuItemSprite *menuMain = MenuItemSprite::create(
        Sprite::create("utility/main_up.png"),
        Sprite::create("utility/main_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleGame::gotoMainMenu, this));

    Menu *menu = Menu::create(menuPuzzle, menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width - 210, visibleSize.height / 2));

    menuPuzzle->setScale(0.66f);
    menuMain->setScale(0.66f);

    menuPuzzle->setPositionY(-215);
    menuMain->setPositionY(-275);

    this->addChild(menu, 2);
}

void PuzzleGame::initMenuPane()
{
    Sprite *pane = Sprite::create();
    pane->setTextureRect(Rect(0, 0, 300, 768));
    pane->setPosition(Vec2(visibleSize.width - 210, visibleSize.height / 2));
    pane->setColor(Color3B::BLACK);
    pane->setOpacity(175);

    this->addChild(pane, 1);
}

void PuzzleGame::initPreviewImage()
{
    previewImage = Sprite::create(GameSettings::getImageName());
    previewImage->setPosition
        (Vec2(visibleSize.width - 210, (visibleSize.height / 2) + 175));
    previewImage->setScaleX(264 / previewImage->getContentSize().width);
    previewImage->setScaleY(198 / previewImage->getContentSize().height);

    previewLabel = Label::createWithTTF("Preview", GameSettings::getFontName(), 28);
    previewLabel->setPosition
        (Vec2(previewImage->getPositionX(), previewImage->getPositionY() + 125));
    previewLabel->setColor(Color3B::YELLOW);

    this->addChild(previewLabel, 2);
    this->addChild(previewImage, 2);
}

void PuzzleGame::initHintSwitch()
{
    hintSwitch = Sprite::create("utility/switch_off.png");
    hintSwitch->setPosition(Vec2
        (visibleSize.width - 210, (visibleSize.height / 2) - 125));
    hintSwitch->setScale(0.66f);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan =
        CC_CALLBACK_2(PuzzleGame::flipHintSwitch, this);
    _eventDispatcher->
        addEventListenerWithSceneGraphPriority(listener, hintSwitch);

    switchLabel = Label::createWithTTF
        ("Show Hints", GameSettings::getFontName(), 26);
    switchLabel->setPosition(Vec2
        (hintSwitch->getPositionX(), hintSwitch->getPositionY() + 50));
    switchLabel->setColor(Color3B::YELLOW);

    this->addChild(hintSwitch, 2);
    this->addChild (switchLabel, 2);
}

void PuzzleGame::update(float delta)
{
    if (!gameOver && started)
    {
        updateTimeLabel();
    }

    // Shuffling with animation.
    if (GameProfile::animatedShufflingEnabled())
    {
        if (computerMoves < shuffleTimes)
        {
            MoveSequence seq;
            if (boardManager.generateRandomMove(seq))
            {
                processMoves(seq, Animation::slide, 0.01f);
                ++computerMoves;
            }
        }
        else
        {
            if (!started)
            {
                timer.startTimer();
                started = true;
            }
        }
    }
}

// Entry point for puzzle interaction; uses BoardManager to identify a valid move attempt.
bool PuzzleGame::interactWithPuzzle(Touch* const touch, Event* const event)
{
    if (gameOver || !started)
    {
        return false;
    }

    Rect recTemp = event->getCurrentTarget()->getBoundingBox();
    PuzzlePiece *piece = dynamic_cast<PuzzlePiece*>(event->getCurrentTarget());

    if (!boardManager.sanityCheckMove(recTemp, *touch, piece))
    {
        return false;
    }

    MoveSequence sequence;
    if (!boardManager.generateTileMoves(sequence, piece))
    {
        return false;
    }

    processMoves(sequence, Animation::slide, 0.1f);

    if (boardManager.isPuzzleComplete())
    {
        gameOver = true;
        endGame();
    }

    return true;
}

void PuzzleGame::processMoves(MoveSequence &seq, Animation anim, const float speed)
{
    PuzzlePiece &blankSpaceRef = puzzle.getPiece(boardManager.findBlankSpace());

    // We need to reverse the vector because the pieces are pushed back in reverse order.
    std::reverse(seq.pieceContainer.begin(), seq.pieceContainer.end());
    for (PuzzlePiece *p : seq.pieceContainer)
    {
        performMoves(p, &blankSpaceRef, seq, anim, speed);
        performMoves(p->getNumLabel(), blankSpaceRef.getNumLabel(), seq, anim, speed);

        boardManager.swapPieces(p->getArrayPos(), boardManager.findBlankSpace());
        boardManager.updateBlankspaceInfo();

        if (started)
        {
            GameProfile::modifyProfileStat(ProfileStat::totalMoves, 1);
            updateMovesLabel(1);
        }
    }
}

void PuzzleGame::performMoves(Node* const from, Node* const to, const MoveSequence &seq, 
                              Animation anim, const float speed) const
{
    if (anim == Animation::slide)
    {
        MoveBy *moveOne = MoveBy::create(speed, Vec2(seq.xMoveDist, seq.yMoveDist));
        from->runAction(moveOne);

        MoveBy *moveTwo = MoveBy::create(speed, Vec2(-seq.xMoveDist, -seq.yMoveDist));
        to->runAction(moveTwo);
    }
    else
    {
        Vec2 fromPos = from->getPosition();
        Vec2 toPos = to->getPosition();

        from->setPosition(toPos);
        to->setPosition(fromPos);
    }
}

void PuzzleGame::updateMovesLabel(const int increment)
{
    userMoves += increment;
    movesLabel->setString("Moves: " + std::to_string(userMoves));
}

void PuzzleGame::updateTimeLabel()
{
    timer.makeTimePoint();
    timeLabel->setString("Time: " + Utility::timeToString(timer.getTime()));
}

bool PuzzleGame::flipHintSwitch(Touch* const touch, Event* const event)
{
    if (gameOver)
    {
        return false;
    }

    Rect recTemp = event->getCurrentTarget()->getBoundingBox();
    if (!recTemp.containsPoint(touch->getLocation()))
    {
        return false;
    }

    updateHintStatus();

    return true;
}

void PuzzleGame::updateHintStatus()
{
    if (!showHints)
    {
        if (!usedHints)
        {
            usedHints = true;
        }

        hintSwitch->initWithFile("utility/switch_on.png");
        boardManager.enableAllLabels(true);
        showHints = true;
    }
    else
    {
        hintSwitch->initWithFile("utility/switch_off.png");
        boardManager.enableAllLabels(false);
        showHints = false;
    }
}

void PuzzleGame::rewardPlayer() const
{
    GameProfile::modifyProfileStat(ProfileStat::puzzlesCompleted, 1);
    GameProfile::modifyProfileStat
        (ProfileStat::stars, GameSettings::getCurrentPuzzleValue());
    GameProfile::modifyProfileStat
        (ProfileStat::totalStarsCollected, GameSettings::getCurrentPuzzleValue());
}

void PuzzleGame::flashScreen()
{
    Sprite *spr = Sprite::create();
    spr->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    spr->setColor(Color3B::WHITE);
    spr->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    spr->setOpacity(100);
    FadeOut *fade = FadeOut::create(1);
    spr->runAction(fade);

    this->addChild(spr, 100);
}

void PuzzleGame::endGame()
{
    timer.endTimerAndRecord();
    flashScreen();

    changeExistingElements();
    createEndGameElements();
    rewardPlayer();
}

void PuzzleGame::changeExistingElements()
{
    boardManager.hideAllPieces(true);
    previewLabel->setOpacity(0);
    previewImage->setOpacity(0);
    hintSwitch->setOpacity(0);

    usedHints ? switchLabel->setString("Used Hints") : switchLabel->setOpacity(0);

    menuPuzzle->initWithNormalSprite(
        Sprite::create("utility/new_up.png"),
        Sprite::create("utility/new_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleGame::gotoPuzzleSelection, this));
}

void PuzzleGame::createEndGameElements()
{
    Sprite *completedPuzzle = Sprite::create(GameSettings::getImageName());
    completedPuzzle->setAnchorPoint(Vec2(0, 1));
    completedPuzzle->setPosition(Vec2(startPosX, startPosY));
    completedPuzzle->setScaleX(800 / previewImage->getContentSize().width);
    completedPuzzle->setScaleY(600 / previewImage->getContentSize().height);

    Sprite *youWin = Sprite::create("utility/win.png");
    youWin->setPosition(Vec2(previewLabel->getPositionX(), visibleSize.height - 100));

    Label *tip = Label::createWithTTF("tip", GameSettings::getFontName(), 24);
    tip->setPosition(Vec2(previewLabel->getPositionX(), visibleSize.height - 160));
    tip->setString("You earned " +
        std::to_string(GameSettings::getCurrentPuzzleValue()) + " stars!");

    Sprite *star = Sprite::create("utility/star.png");
    star->setPosition(Vec2(previewImage->getPositionX(), tip->getPositionY() - 70));
    star->setScale(0.25f);

    Label *numStars =
        Label::createWithTTF("numStars", GameSettings::getFontName(), 24);
    numStars->setPosition(Vec2(star->getPositionX() + 75, star->getPositionY()));
    numStars->setString("x " + std::to_string(GameSettings::getCurrentPuzzleValue()));

    this->addChild(completedPuzzle, 1);
    this->addChild(youWin, 2);
    this->addChild(tip, 2);
    this->addChild(star, 2);
    this->addChild(numStars, 2);
}

void PuzzleGame::gotoMainMenu(Ref* const sender)
{
    recordTimerIfStarted();

    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}


void PuzzleGame::gotoPuzzleSelection(Ref* const sender)
{
    recordTimerIfStarted();

    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleSelection::createScene()));
}

void PuzzleGame::recordTimerIfStarted()
{
    if (started)
    {
        timer.endTimerAndRecord();
    }
}