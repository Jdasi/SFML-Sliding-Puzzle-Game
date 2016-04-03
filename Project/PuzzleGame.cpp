#include "PuzzleGame.h"
#include "GameSettings.h"
#include "GameProfile.h"
#include "MainMenu.h"
#include "PuzzleSelection.h"

USING_NS_CC;

PuzzleGame::PuzzleGame()
    : blankSpace(0)
    , blankSpaceCoords({0, 0})
    , gameOver(false)
    , startPosX(100)
    , startPosY(80)
    , currentPieceArrayPos(0)
    , numMoves(0)
    , movesLabel(nullptr)
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
    puzzle.initPuzzle(this, startPosX, startPosY);

    int product = GameSettings::getSegments().x * GameSettings::getSegments().y;
    generateRandomMoves(product * product);

    moveBlankSpaceToStart();
}

void PuzzleGame::initLabels()
{
    movesLabel = Label::createWithTTF("Moves: ", "fonts/Marker Felt.ttf", 30);
    movesLabel->setPosition(Vec2(visibleSize.width - 210, (visibleSize.height / 2) - 50));

    updateMovesLabel();

    this->addChild(movesLabel, 2);
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

    previewLabel = Label::createWithTTF("Preview", "fonts/Marker Felt.ttf", 24);
    previewLabel->setPosition
        (Vec2(previewImage->getPositionX(), previewImage->getPositionY() + 125));

    this->addChild(previewLabel, 2);
    this->addChild(previewImage, 2);
}

void PuzzleGame::updateMovesLabel(int increment)
{
    numMoves += increment;
    movesLabel->setString("Moves: " + std::to_string(numMoves));
}

bool PuzzleGame::interactWithPuzzle(cocos2d::Touch *touch, cocos2d::Event *event)
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

    PuzzlePiece *piece = dynamic_cast<PuzzlePiece*>(event->getCurrentTarget());
    if (piece->isBlankSpace())
    {
        return false;
    }

    if (puzzle.getPiece(blankSpace).getNumberOfRunningActions() != 0)
    {
        return false;
    }

    currentPieceArrayPos = piece->getArrayPos();
    coordinate currPieceCoords = piece->getCoordinates();
    updateBlankspaceInfo();

    if (currPieceCoords.x == blankSpaceCoords.x)
    {
        float moveDist = piece->getBoundingBox().size.height + puzzle.getPadding();
        if (currPieceCoords.y > blankSpaceCoords.y)
        {
            generateMove(SlideDirection::up, 0, moveDist);
        }
        else
        {
            generateMove(SlideDirection::down, 0, -moveDist);
        }

        return true;
    }

    if (currPieceCoords.y == blankSpaceCoords.y)
    {
        float moveDist = piece->getBoundingBox().size.width + puzzle.getPadding();
        if (currPieceCoords.x > blankSpaceCoords.x)
        {
            generateMove(SlideDirection::left, -moveDist, 0);
        }
        else
        {
            generateMove(SlideDirection::right, moveDist, 0);
        }

        return true;
    }

    return false;
}

void PuzzleGame::generateMove(SlideDirection dir, float xMoveDist, float yMoveDist)
{
    std::vector<PuzzlePiece*> piecesToMove;

    switch (dir)
    {
        case SlideDirection::up:
        {
            for (int i = puzzle.getPiece(currentPieceArrayPos).getCoordinates().y;
                     i > blankSpaceCoords.y; --i)
            {
                int offset = puzzle.calculateOffset(blankSpaceCoords.x, i);
                if (puzzle.getPiece(offset).isBlankSpace())
                {
                    break;
                }

                piecesToMove.push_back(&puzzle.getPiece(offset));
            }

            break;
        }
        case SlideDirection::down:
        {
            for (int i = puzzle.getPiece(currentPieceArrayPos).getCoordinates().y;
                     i < blankSpaceCoords.y; ++i)
            {
                int offset = puzzle.calculateOffset(blankSpaceCoords.x, i);
                if (puzzle.getPiece(offset).isBlankSpace())
                {
                    break;
                }

                piecesToMove.push_back(&puzzle.getPiece(offset));
            }

            break;
        }
        case SlideDirection::left:
        {
            for (int i = puzzle.getPiece(currentPieceArrayPos).getCoordinates().x;
                     i > blankSpaceCoords.x; --i)
            {
                int offset = puzzle.calculateOffset(i, blankSpaceCoords.y);
                if (puzzle.getPiece(offset).isBlankSpace())
                {
                    break;
                }

                piecesToMove.push_back(&puzzle.getPiece(offset));
            }

            break;
        }
        case SlideDirection::right:
        {
            for (int i = puzzle.getPiece(currentPieceArrayPos).getCoordinates().x;
                     i < blankSpaceCoords.x; ++i)
            {
                int offset = puzzle.calculateOffset(i, blankSpaceCoords.y);
                if (puzzle.getPiece(offset).isBlankSpace())
                {
                    break;
                }

                piecesToMove.push_back(&puzzle.getPiece(offset));
            }

            break;
        }
        default:
        {
            throw std::runtime_error("Error in generateVerticalMove");
        }
    }

    // We need to reverse the vector because the pieces are pushed back in reverse order.
    std::reverse(piecesToMove.begin(), piecesToMove.end());
    for (PuzzlePiece *p : piecesToMove)
    {
        MoveBy *move = MoveBy::create(0.1f, Vec2(xMoveDist, yMoveDist));
        p->runAction(move);

        puzzle.swapPieces(p->getArrayPos(), blankSpace);
        updateBlankspaceInfo();

        GameProfile::modifyProfileStat(ProfileStat::totalMoves, 1);
        updateMovesLabel(1);

        if (puzzle.isPuzzleComplete())
        {
            gameOver = true;
            endGame();
        }
    }
}

void PuzzleGame::generateRandomMoves(int times)
{
    for (int i = 0; i < times; ++i)
    {
        updateBlankspaceInfo();

        bool tileSwapped = false;
        while (!tileSwapped)
        {
            SlideDirection direction = static_cast<SlideDirection>(rand() % 4);

            switch (direction)
            {
                case SlideDirection::right:
                {
                    if (tryComputerMove(blankSpace, 
                        blankSpaceCoords.x + 1, blankSpaceCoords.y))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                case SlideDirection::down:
                {
                    if (tryComputerMove(blankSpace, 
                        blankSpaceCoords.x, blankSpaceCoords.y + 1))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                case SlideDirection::left:
                {
                    if (tryComputerMove(blankSpace, 
                        blankSpaceCoords.x - 1, blankSpaceCoords.y))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                case SlideDirection::up:
                {
                    if (tryComputerMove(blankSpace, 
                        blankSpaceCoords.x, blankSpaceCoords.y - 1))
                    {
                        tileSwapped = true;
                    }

                    break;
                }
                default: {}
            }
        }
    }
}

bool PuzzleGame::tryComputerMove(int fromPiece, int toX, int toY)
{
    if (!puzzle.inBounds(toX, toY))
    {
        return false;
    }

    int toPiece = puzzle.calculateOffset(toX, toY);

    Vec2 fromPos = puzzle.getPiece(fromPiece).getPosition();
    Vec2 toPos = puzzle.getPiece(toPiece).getPosition();

    puzzle.getPiece(fromPiece).setPosition(toPos);
    puzzle.getPiece(toPiece).setPosition(fromPos);

    puzzle.swapPieces(fromPiece, toPiece);

    return true;
}

void PuzzleGame::updateBlankspaceInfo()
{
    blankSpace = puzzle.findBlankSpace();
    blankSpaceCoords = puzzle.getPiece(blankSpace).getCoordinates();
}

void PuzzleGame::moveBlankSpaceToStart()
{
    for (int i = 0; i < GameSettings::getSegments().x; ++i)
    {
        updateBlankspaceInfo();

        if (!tryComputerMove(blankSpace, blankSpaceCoords.x + 1, blankSpaceCoords.y))
        {
            break;
        }
    }

    for (int i = 0; i < GameSettings::getSegments().y; ++i)
    {
        updateBlankspaceInfo();

        if (!tryComputerMove(blankSpace, blankSpaceCoords.x, blankSpaceCoords.y + 1))
        {
            break;
        }
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
    
    // Change existing elements.
    puzzle.getPiece(puzzle.findBlankSpace()).setBlankSpace(false);
    puzzle.hideAllPieces();
    previewLabel->setOpacity(0);
    previewImage->setOpacity(0);
    menuPuzzle->initWithNormalSprite(
        Sprite::create("utility/new_up.png"),
        Sprite::create("utility/new_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleGame::gotoPuzzleSelection, this));
    rewardPlayer();

    // Create new elements.
    Sprite *completedPuzzle = Sprite::create(GameSettings::getImageName());
    completedPuzzle->setAnchorPoint(Vec2(0, 1));
    completedPuzzle->setPosition(Vec2(startPosX, visibleSize.height - startPosY));
    completedPuzzle->setScaleX(800 / previewImage->getContentSize().width);
    completedPuzzle->setScaleY(600 / previewImage->getContentSize().height);

    Sprite *youWin = Sprite::create("utility/win.png");
    youWin->setPosition(Vec2(previewLabel->getPositionX(), visibleSize.height - 100));

    Label *tip = Label::createWithTTF("tip", "fonts/Marker Felt.ttf", 24);
    tip->setPosition(Vec2(previewLabel->getPositionX(), visibleSize.height - 160));
    tip->setString("You earned " +
        std::to_string(GameSettings::getCurrentPuzzleValue()) + " stars!");
    tip->enableGlow(Color4B::BLACK);

    Sprite *star = Sprite::create("utility/star.png");
    star->setPosition(Vec2(previewImage->getPositionX(), tip->getPositionY() - 70));
    star->setScale(0.25f);

    Label *numStars =
        Label::createWithTTF("numStars", "fonts/Marker Felt.ttf", 24);
    numStars->setPosition(Vec2(star->getPositionX() + 75, star->getPositionY()));
    numStars->setString("x " + std::to_string(GameSettings::getCurrentPuzzleValue()));
    numStars->enableGlow(Color4B::BLACK);

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