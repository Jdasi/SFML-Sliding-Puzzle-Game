#include "GameSettings.h"

#include "PuzzleScene.h"

#include "MainMenu.h"
USING_NS_CC;

cocos2d::Scene *MainMenu::createScene()
{
    cocos2d::Scene *scene = Scene::create();
    auto layer = MainMenu::create();

    scene->addChild(layer);

    return scene;
}

bool MainMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    cocos2d::Label *label = Label::createWithTTF("Sliding Puzzle", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 2) + 150));
    this->addChild(label, 1);

    initSettings();
    initMenu();

    return true;
}

void MainMenu::initSettings()
{
    GameSettings::setImageName("puzzles/dog.jpg");  // Options: dog, cats, bear, bunny, moon
    GameSettings::setSegments(4, 4);
}

void MainMenu::initMenu()
{
    // Create the first menu item, this will exit the app
    MenuItemSprite *mainMenuExit = new MenuItemSprite();
    mainMenuExit->initWithNormalSprite(
        Sprite::create("CloseNormal.png"),
        Sprite::create("CloseSelected.png"),
        nullptr,
        CC_CALLBACK_1(MainMenu::menuCloseCallback, this));

    MenuItemFont *mainMenuStartGame = MenuItemFont::create(
        "Play Game",
        CC_CALLBACK_1(MainMenu::menuStartGame, this));

    // Create the actual menu and assign the menu to the HelloWorld scene
    cocos2d::Menu *menu = Menu::create(mainMenuStartGame, mainMenuExit, nullptr);
    menu->alignItemsVertically();
    this->addChild(menu, 1);
}

void MainMenu::menuStartGame(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(1, PuzzleScene::createScene()));
}

void MainMenu::menuCloseCallback(cocos2d::Ref *pSender)
{
    Director::getInstance()->end();
}
