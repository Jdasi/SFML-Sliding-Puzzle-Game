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
    , gameOver(false)
    , startPosX(0)
    , startPosY(0)
    , numMoves(0)
    , movesLabel(nullptr)
    , timeLabel(nullptr)
    , previewLabel(nullptr)
    , previewImage(nullptr)
    , menuPuzzle(nullptr)
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

    timer.startTimer();

    initBackdrop();
    initPuzzle();
    initLabels();
    initMenu();
    initPreviewImage();

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

    int product = GameSettings::getSegments().x * GameSettings::getSegments().y;
    boardManager.generateRandomMoves(product * product);

    boardManager.moveBlankSpaceToStart();
}

void PuzzleGame::initLabels()
{
    movesLabel = Label::createWithTTF("Moves: ", GameSettings::getFontName(), 30);
    movesLabel->setPosition(Vec2(visibleSize.width - 210, (visibleSize.height / 2) - 20));

    updateMovesLabel();

    timeLabel = Label::createWithTTF("Time: ", GameSettings::getFontName(), 30);
    timeLabel->setPosition(Vec2(visibleSize.width - 210, (visibleSize.height / 2) + 20));

    updateTimeLabel();

    this->addChild(movesLabel, 2);
    this->addChild(timeLabel, 2);
}

void PuzzleGame::initMenu()
{
    menuPuzzle = new MenuItemSprite();
    menuPuzzle->initWithNormalSprite(
        Sprite::create("utility/change_up.png"),
        Sprite::create("utility/change_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleGame::gotoPuzzleSelection, this));

    MenuItemSprite *menuMain = new MenuItemSprite();
    menuMain->initWithNormalSprite(
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

    Sprite *pane = Sprite::create();
    pane->setTextureRect(Rect(0, 0, 300, 768));
    pane->setPosition(Vec2(visibleSize.width - 210, visibleSize.height / 2));
    pane->setColor(Color3B::BLACK);
    pane->setOpacity(175);

    this->addChild(menu, 2);
    this->addChild(pane, 1);
}

void PuzzleGame::initPreviewImage()
{
    previewImage = Sprite::create(GameSettings::getImageName());
    previewImage->setPosition(Vec2(visibleSize.width - 210, (visibleSize.height / 2) + 175));
    previewImage->setScaleX(264 / previewImage->getContentSize().width);
    previewImage->setScaleY(198 / previewImage->getContentSize().height);

    previewLabel = Label::createWithTTF("Preview", GameSettings::getFontName(), 26);
    previewLabel->setPosition
        (Vec2(previewImage->getPositionX(), previewImage->getPositionY() + 125));

    this->addChild(previewLabel, 2);
    this->addChild(previewImage, 2);
}

bool PuzzleGame::interactWithPuzzle(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (gameOver)
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
    bool result = boardManager.generateTileMoves(sequence, piece);

    if (!result)
    {
        return false;
    }

    performMoves(sequence);
    return true;
}

void PuzzleGame::performMoves(MoveSequence &seq)
{
    // We need to reverse the vector because the pieces are pushed back in reverse order.
    std::reverse(seq.pieceContainer.begin(), seq.pieceContainer.end());
    for (PuzzlePiece *p : seq.pieceContainer)
    {
        MoveBy *move = MoveBy::create(0.1f, Vec2(seq.xMoveDist, seq.yMoveDist));
        p->runAction(move);

        boardManager.swapPieces(p->getArrayPos(), boardManager.findBlankSpace());
        boardManager.updateBlankspaceInfo();

        GameProfile::modifyProfileStat(ProfileStat::totalMoves, 1);
        updateMovesLabel(1);

        if (boardManager.isPuzzleComplete())
        {
            gameOver = true;
            endGame();
        }
    }
}

void PuzzleGame::updateMovesLabel(int increment)
{
    numMoves += increment;
    movesLabel->setString("Moves: " + std::to_string(numMoves));
}

void PuzzleGame::updateTimeLabel()
{
    timer.makeTimePoint();
    timeLabel->setString("Time: " + calculateTime(std::to_string(timer.getTime())));
}

void PuzzleGame::update(float delta)
{
    if (!gameOver)
    {
        updateTimeLabel();
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
}

void PuzzleGame::changeExistingElements()
{
    boardManager.hideAllPieces();
    previewLabel->setOpacity(0);
    previewImage->setOpacity(0);
    menuPuzzle->initWithNormalSprite(
        Sprite::create("utility/new_up.png"),
        Sprite::create("utility/new_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleGame::gotoPuzzleSelection, this));
    rewardPlayer();
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

void PuzzleGame::gotoMainMenu(cocos2d::Ref *sender)
{
    timer.endTimerAndRecord();
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}

void PuzzleGame::gotoPuzzleSelection(cocos2d::Ref *sender)
{
    timer.endTimerAndRecord();
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleSelection::createScene()));
}