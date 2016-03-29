#include "ProfileUnlocks.h"
#include "GameSettings.h"
#include "GameProfile.h"
#include "MainMenu.h"

USING_NS_CC;

std::vector<GameUnlock> &ProfileUnlocks::unlocksRef = GameProfile::getUnlocks();

cocos2d::Scene *ProfileUnlocks::createScene()
{
    cocos2d::Scene *scene = Scene::create();
    auto layer = ProfileUnlocks::create();

    scene->addChild(layer);

    return scene;
}

bool ProfileUnlocks::init()
{
    if (!Layer::init())
    {
        return false;
    }

    currentSelection = 0;

    //unlocksRef[GameProfile::backgroundNameToInt(BackgroundName::regal)].setLocked(false);

    Sprite *sceneTitle = Sprite::create("utility/unlocks.png");
    sceneTitle->setPosition
        (Vec2((visibleSize.width / 2), visibleSize.height - 100));

    this->addChild(sceneTitle, 2);

    initBackdrop();
    initMenu();
    initPreviewImages();

    return true;
}

void ProfileUnlocks::initBackdrop()
{
    Sprite *backdrop = Sprite::create
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");

    backdrop->setAnchorPoint(Vec2(0, 0));
    backdrop->setPosition(Vec2(0, 0));

    this->addChild(backdrop, 0);
}

void ProfileUnlocks::initMenu()
{
    actionButton = new MenuItemSprite();
    actionButton->initWithNormalSprite(
        Sprite::create("utility/select_up.png"),
        Sprite::create("utility/select_dn.png"),
        nullptr,
        CC_CALLBACK_1(ProfileUnlocks::contextAction, this));

    MenuItemSprite *menuMain = new MenuItemSprite();
    menuMain->initWithNormalSprite(
        Sprite::create("utility/main_up.png"),
        Sprite::create("utility/main_dn.png"),
        nullptr,
        CC_CALLBACK_1(ProfileUnlocks::gotoMainMenu, this));

    cocos2d::Menu *menu = Menu::create(actionButton, menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    menuMain->setScale(0.66f);
    menuMain->setPosition(Vec2(0, -300));

    this->addChild(menu, 2);
}

void ProfileUnlocks::initPreviewImages()
{
    std::string imgSuffix;
    for (int i = 0; i < 4; ++i)
    {
        Sprite *spr = nullptr;

        if (unlocksRef[i].isLocked())
        {
            imgSuffix = "_locked.jpg";
        }
        else
        {
            imgSuffix = "_puzzle.jpg";
        }

        spr = Sprite::create("backdrops/" + unlocksRef[i].getName() + imgSuffix);

        spr->setPosition(Vec2(250 + (i * 285) + (i * 2), (visibleSize.height / 2) + 100));
        spr->setScaleX(273 / spr->getContentSize().width);
        spr->setScaleY(154 / spr->getContentSize().height);
        spr->setOpacity(fadedOpacity);
        spr->setTag(i);

        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(ProfileUnlocks::imageClick, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spr);

        previewImages.push_back(spr);

        this->addChild(spr, 1);
    }

    //previewImages[GameSettings::getImageID()]->setOpacity(selectedOpacity);
}

bool ProfileUnlocks::imageClick(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Sprite *spr = static_cast<Sprite*>(event->getCurrentTarget());

    Point pt = touch->getLocation();
    Rect recTemp = spr->getBoundingBox();

    if (!recTemp.containsPoint(pt))
    {
        return false;
    }

    for (auto i : previewImages)
    {
        i->setOpacity(fadedOpacity);
    }

    spr->setOpacity(selectedOpacity);
    currentSelection = spr->getTag();

    updateActionButton();

    return true;
}

void ProfileUnlocks::updateActionButton()
{

}

void ProfileUnlocks::contextAction(cocos2d::Ref *sender)
{
    
}

void ProfileUnlocks::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}