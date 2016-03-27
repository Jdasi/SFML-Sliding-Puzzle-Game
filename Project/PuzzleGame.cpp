#include "PuzzleGame.h"
#include "GameSettings.h"
#include "GameProfile.h"
#include "MainMenu.h"
#include "PuzzleSelection.h"

USING_NS_CC;

cocos2d::Scene *PuzzleGame::createScene()
{
    cocos2d::Scene *scene = Scene::create();
    auto layer = PuzzleGame::create();

    scene->addChild(layer);

    return scene;
}

bool PuzzleGame::init()
{
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

    GameProfile::modifyProfileSetting(ProfileSetting::puzzlesAttempted, 1);

    return true;
}

void PuzzleGame::initBackdrop()
{
    Sprite *backdrop = Sprite::create("backdrops/regal_puzzle.jpg");

    backdrop->setAnchorPoint(Vec2(0, 0));
    backdrop->setPosition(Vec2(0, 0));

    this->addChild(backdrop, 0);
}

void PuzzleGame::initPuzzle()
{
    puzzle.initPuzzle(this, startPosX, startPosY);

    int product = GameSettings::getSegments().x * GameSettings::getSegments().y;
    //generateRandomValidMoves(product * product);
    generateRandomValidMoves(1);

    moveBlankSpaceToStart();
}

void PuzzleGame::initLabels()
{
    movesLabel = Label::createWithTTF("Moves: ", "fonts/Marker Felt.ttf", 30);
    movesLabel->setPosition(Vec2(visibleSize.width - 210, (visibleSize.height / 2) - 100));

    updateMovesLabel();
}

void PuzzleGame::initMenu()
{
    MenuItemFont *menuChoosePuzzle = MenuItemFont::create(
        "Choose Puzzle",
        CC_CALLBACK_1(PuzzleGame::gotoPuzzleSelection, this));

    MenuItemFont *menuMain = MenuItemFont::create(
        "Main Menu",
        CC_CALLBACK_1(PuzzleGame::gotoMainMenu, this));

    Menu *menu = Menu::create(menuChoosePuzzle, menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width - 210, visibleSize.height / 2));

    menuChoosePuzzle->setPositionY(-225);
    menuMain->setPositionY(-275);

    this->addChild(menu);
    this->addChild(movesLabel);
}

void PuzzleGame::initPreviewImage()
{
    Sprite *preview = Sprite::create(GameSettings::getImageName());
    preview->setPosition(Vec2(visibleSize.width - 210, (visibleSize.height / 2) + 175));
    preview->setScaleX(264 / preview->getContentSize().width);
    preview->setScaleY(198 / preview->getContentSize().height);

    Label *label = Label::createWithTTF("Preview", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(preview->getPositionX(), preview->getPositionY() + 125));

    this->addChild(label);
    this->addChild(preview);
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
    
    if (tryUserMove(currentPieceTag, currPiece.x, currPiece.y - 1))
    {
        return;
    }
}

void PuzzleGame::generateRandomValidMoves(int times)
{
    for (int i = 0; i < times; ++i)
    {
        int blankSpace = puzzle.findBlankSpace();
        coordinate blankSpaceCoords = puzzle.calculateCoordinates(blankSpace);

        bool tileSwapped = false;
        while (!tileSwapped)
        {
            int randomDirection = rand() % 4;

            switch (randomDirection)
            {
            case 0:
            {
                if (tryComputerMove(blankSpace, blankSpaceCoords.x + 1, blankSpaceCoords.y))
                {
                    tileSwapped = true;
                }

                break;
            }
            case 1:
            {
                if (tryComputerMove(blankSpace, blankSpaceCoords.x, blankSpaceCoords.y + 1))
                {
                    tileSwapped = true;
                }

                break;
            }
            case 2:
            {
                if (tryComputerMove(blankSpace, blankSpaceCoords.x - 1, blankSpaceCoords.y))
                {
                    tileSwapped = true;
                }

                break;
            }
            case 3:
            {
                if (tryComputerMove(blankSpace, blankSpaceCoords.x, blankSpaceCoords.y - 1))
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

    GameProfile::modifyProfileSetting(ProfileSetting::totalMoves, 1);

    if (puzzle.isPuzzleComplete())
    {
        puzzle.getPiece(puzzle.findBlankSpace()).setBlankSpace(false);
        GameProfile::modifyProfileSetting(ProfileSetting::puzzlesCompleted, 1);
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

void PuzzleGame::moveBlankSpaceToStart()
{
    for (int i = 0; i < GameSettings::getSegments().x; ++i)
    {
        int blankSpace = puzzle.findBlankSpace();
        coordinate blankSpaceCoords = puzzle.calculateCoordinates(blankSpace);

        if (!tryComputerMove(blankSpace, blankSpaceCoords.x + 1, blankSpaceCoords.y))
        {
            break;
        }
    }

    for (int i = 0; i < GameSettings::getSegments().y; ++i)
    {
        int blankSpace = puzzle.findBlankSpace();
        coordinate blankSpaceCoords = puzzle.calculateCoordinates(blankSpace);

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