#include "PuzzleGame.h"
#include "GameProfile.h"
#include "MainMenu.h"
#include "PuzzleSelection.h"

USING_NS_CC;

coordinate PuzzleGame::blankSpaceCoords{ 0, 0 };

cocos2d::Scene *PuzzleGame::createScene()
{
    cocos2d::Scene *scene = Scene::create();
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

    startPosX = 100;
    startPosY = 80;

    currentPieceTag = 0;
    gameOver = false;
    numMoves = 0;

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

    //int product = GameSettings::getSegments().x * GameSettings::getSegments().y;
    //generateRandomValidMoves(product * product);

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
    MenuItemSprite *menuChange = new MenuItemSprite();
    menuChange->initWithNormalSprite(
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

    Menu *menu = Menu::create(menuChange, menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width - 210, visibleSize.height / 2));

    menuChange->setScale(0.66f);
    menuMain->setScale(0.66f);

    menuChange->setPositionY(-215);
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
    Sprite *preview = Sprite::create(GameSettings::getImageName());
    preview->setPosition(Vec2(visibleSize.width - 210, (visibleSize.height / 2) + 175));
    preview->setScaleX(264 / preview->getContentSize().width);
    preview->setScaleY(198 / preview->getContentSize().height);

    Label *label = Label::createWithTTF("Preview", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(preview->getPositionX(), preview->getPositionY() + 125));

    this->addChild(label, 2);
    this->addChild(preview, 2);
}

void PuzzleGame::updateMovesLabel(int increment)
{
    numMoves += increment;
    movesLabel->setString("Moves: " + std::to_string(numMoves));
}

bool PuzzleGame::interactWithPuzzle(cocos2d::Touch *touch, cocos2d::Event *event)
{
    PuzzlePiece *piece = static_cast<PuzzlePiece*>(event->getCurrentTarget());

    if (piece->isBlankSpace())
    {
        return false;
    }

    Point pt = touch->getLocation();
    Rect recTemp = piece->getBoundingBox();

    if (!recTemp.containsPoint(pt))
    {
        return false;
    }

    currentPieceTag = piece->getTag();

    generateValidMove();

    return true;
}

void PuzzleGame::generateValidMove()
{
    coordinate currPiece = puzzle.calculateCoordinates(currentPieceTag);

    if (tryUserMove(currentPieceTag, currPiece.x + 1, currPiece.y))
    {
        return;
    }

    if (tryUserMove(currentPieceTag, currPiece.x, currPiece.y + 1))
    {
        return;
    }
    
    if (tryUserMove(currentPieceTag, currPiece.x - 1, currPiece.y))
    {
        return;
    }
    
    tryUserMove(currentPieceTag, currPiece.x, currPiece.y - 1);
}

void PuzzleGame::generateRandomValidMoves(int times)
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
                case SlideDirection::up:
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
                case SlideDirection::down:
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

bool PuzzleGame::tryUserMove(int fromPiece, int toX, int toY)
{
    if (!puzzle.inBounds(toX, toY))
    {
        return false;
    }

    int toPiece = puzzle.calculateOffset(toX, toY);

    if (!puzzle.isPieceBlankSpace(toPiece))
    {
        return false;
    }

    if (puzzle.getPiece(toPiece).getNumberOfRunningActions() != 0)
    {
        return false;
    }

    float time = 0.1f;

    MoveTo *action1 = MoveTo::create(time, puzzle.getPiece(toPiece).getPosition());
    MoveTo *action2 = MoveTo::create(time, puzzle.getPiece(fromPiece).getPosition());

    puzzle.getPiece(fromPiece).runAction(action1);
    puzzle.getPiece(toPiece).runAction(action2);

    puzzle.swapPieces(fromPiece, toPiece);
    updateMovesLabel(1);

    GameProfile::modifyProfileStat(ProfileStat::totalMoves, 1);

    if (puzzle.isPuzzleComplete())
    {
        GameProfile::modifyProfileStat(ProfileStat::puzzlesCompleted, 1);
        GameProfile::modifyProfileStat
            (ProfileStat::stars, GameSettings::getCurrentPuzzleValue());
        GameProfile::modifyProfileStat
            (ProfileStat::totalStarsCollected, GameSettings::getCurrentPuzzleValue());

        puzzle.getPiece(puzzle.findBlankSpace()).setBlankSpace(false);
        gameOver = true;

        // ------- code possibly for somewhere else?
        Sprite *spr = Sprite::create();
        spr->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
        spr->setColor(Color3B::WHITE);
        spr->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        spr->setOpacity(100);
        FadeOut *fade = FadeOut::create(1);
        spr->runAction(fade);
        this->addChild(spr, 100);
    }

    return true;
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
    blankSpaceCoords = puzzle.calculateCoordinates(blankSpace);
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

void PuzzleGame::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}

void PuzzleGame::gotoPuzzleSelection(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleSelection::createScene()));
}