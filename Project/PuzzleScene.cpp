#include "Project/PuzzleScene.h"
#include "Project/GameSettings.h"
#include "Project/MainMenu.h"

USING_NS_CC;

cocos2d::Scene *PuzzleScene::createScene()
{
    cocos2d::Scene *scene = Scene::create();
    auto layer = PuzzleScene::create();

    scene->addChild(layer);

    return scene;
}

bool PuzzleScene::init()
{
    startPosX = 100;
    startPosY = 80;

    currentPieceTag = 0;
    gameOver = false;
    numMoves = 0;

    initBackground();
    initPuzzle();
    initLabels();

    return true;
}

void PuzzleScene::initBackground()
{
    auto backdrop = Sprite::create("backdrops/regal.jpg");

    backdrop->setAnchorPoint(Vec2(0, 0));
    backdrop->setPosition(Vec2(0, 0));

    this->addChild(backdrop, 0);
}

void PuzzleScene::initPuzzle()
{
    puzzle.initPuzzle(this, startPosX, startPosY);

    int product = GameSettings::getSegments().x * GameSettings::getSegments().y;
    //generateRandomValidMoves(product * product);
    generateRandomValidMoves(1);

    moveBlankSpaceToStart();
}

void PuzzleScene::initLabels()
{
    movesLabel = Label::createWithTTF("Moves: ", "fonts/Marker Felt.ttf", 24);
    movesLabel->setPosition(Vec2(visibleSize.width - 250, visibleSize.height - 100));

    updateMovesLabel(0);

    MenuItemFont *menuLabel = MenuItemFont::create(
        "Main Menu",
        CC_CALLBACK_1(PuzzleScene::gotoMainMenu, this));

    Menu *menu = Menu::create(menuLabel, nullptr);
    menu->setPosition(Vec2(visibleSize.width - 250, visibleSize.height - 200));

    this->addChild(menu, 1);
    this->addChild(movesLabel, 1);
}

void PuzzleScene::updateMovesLabel(int increment)
{
    numMoves += increment;

    std::string stringMoves = "Moves: " + std::to_string(numMoves);
    movesLabel->setString(stringMoves);
}

bool PuzzleScene::interactWithPuzzle(cocos2d::Touch *touch, cocos2d::Event *event)
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

void PuzzleScene::generateValidMove()
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

void PuzzleScene::generateRandomValidMoves(int times)
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

bool PuzzleScene::tryUserMove(int fromPiece, int toX, int toY)
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

    if (puzzle.isPuzzleComplete())
    {
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

bool PuzzleScene::tryComputerMove(int fromPiece, int toX, int toY)
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

void PuzzleScene::moveBlankSpaceToStart()
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

void PuzzleScene::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}