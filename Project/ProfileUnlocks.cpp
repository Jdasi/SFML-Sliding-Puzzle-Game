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

    fadedOpacity = 150;
    selectedOpacity = 255;
    currentSelection = GameProfile::stringToBackgroundID
        (GameProfile::getCurrentBackground());
    action = ContextAction::select;
    selectionRect = Sprite::create();

    initBackdrop();
    initStarDisplay();
    initMenu();
    initPreviewImages();
    initLabels();

    return true;
}

void ProfileUnlocks::initBackdrop()
{
    backdrop = Sprite::create();
    backdrop->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    updateBackdrop();

    this->addChild(backdrop, 0);
}

void ProfileUnlocks::initStarDisplay()
{
    Sprite *star = Sprite::create("utility/star.png");
    star->setPosition(Vec2(visibleSize.width - 200, visibleSize.height - 100));
    star->setScale(0.4f);

    numStars = Label::createWithTTF("numStars", "fonts/Marker Felt.ttf", 24);
    numStars->setPosition(Vec2(star->getPositionX() + 100, star->getPositionY() - 50));
    numStars->enableGlow(Color4B::BLACK);

    updateNumStarsLabel();

    this->addChild(star, 1);
    this->addChild(numStars, 1);
}

void ProfileUnlocks::initMenu()
{
    actionButton = new MenuItemSprite();
    updateActionButton();

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

    actionButton->setScale(0.66f);
    actionButton->setPosition(Vec2(0, -240));

    this->addChild(menu, 2);
}

void ProfileUnlocks::initPreviewImages()
{
    std::string imgSuffix;
    for (int i = 0; i < 4; ++i)
    {
        Sprite *spr = Sprite::create();

        if (unlocksRef[i].isLocked())
        {
            imgSuffix = "_locked.jpg";
        }
        else
        {
            imgSuffix = "_puzzle.jpg";
        }

        spr->initWithFile("backdrops/" + unlocksRef[i].getName() + imgSuffix);
        spr->setPosition(Vec2(250 + (i * 285) + (i * 2), (visibleSize.height / 2) + 100));
        spr->setScaleX(273 / spr->getContentSize().width);
        spr->setScaleY(154 / spr->getContentSize().height);
        spr->setOpacity(fadedOpacity);
        spr->setTag(i);

        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(ProfileUnlocks::imageClick, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spr);

        previewImages.push_back(spr);

        Label *lbl = Label::createWithTTF
            (unlocksRef[i].getName(), "fonts/Marker Felt.ttf", 26);
        lbl->setPosition(Vec2(spr->getPositionX(), spr->getPositionY() - 100));

        this->addChild(spr, 3);
        this->addChild(lbl, 3);
    }

    previewImages[currentSelection]->setOpacity(selectedOpacity);

    Sprite *pane = Sprite::create();
    pane->setTextureRect(Rect(0, 0, 1366, 300));
    pane->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 2) + 50));
    pane->setColor(Color3B::BLACK);
    pane->setOpacity(175);

    selectionRect->setTextureRect(Rect(0, 0, 280, 165));
    selectionRect->setColor(Color3B::YELLOW);

    updateSelectionRect();
    
    this->addChild(pane, 1);
    this->addChild(selectionRect, 2);
}

void ProfileUnlocks::initLabels()
{
    Sprite *sceneTitle = Sprite::create("utility/unlocks.png");
    sceneTitle->setPosition
        (Vec2((visibleSize.width / 2), visibleSize.height - 100));

    contextHintLabel = Label::createWithTTF("contextHint", "fonts/Marker Felt.ttf", 24);
    contextHintLabel->setPosition
        (Vec2(visibleSize.width / 2, (visibleSize.height / 2) - 75));
    contextHintLabel->enableGlow(Color4B::BLACK);

    updateContextHintLabel();

    this->addChild(sceneTitle, 2);
    this->addChild(contextHintLabel, 2);
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
    updateSelectionRect();
    updateContextHintLabel();

    return true;
}

void ProfileUnlocks::updateActionButton()
{
    if (unlocksRef[currentSelection].isLocked())
    {
        if (stoi(GameProfile::getProfileStat
            (ProfileStat::stars)) >= unlocksRef[currentSelection].getStarCost())
        {
            actionButton->initWithNormalSprite(
                Sprite::create("utility/unlock_up.png"),
                Sprite::create("utility/unlock_dn.png"),
                nullptr,
                CC_CALLBACK_1(ProfileUnlocks::performContextAction, this));
            action = ContextAction::unlock;
        }
        else
        {
            actionButton->initWithNormalSprite(
                Sprite::create("utility/locked_up.png"),
                Sprite::create("utility/locked_dn.png"),
                nullptr,
                CC_CALLBACK_1(ProfileUnlocks::performContextAction, this));
            action = ContextAction::null;
        }

        return;
    }

    actionButton->initWithNormalSprite(
        Sprite::create("utility/select_up.png"),
        Sprite::create("utility/select_dn.png"),
        nullptr,
        CC_CALLBACK_1(ProfileUnlocks::performContextAction, this));
    action = ContextAction::select;
}

void ProfileUnlocks::performContextAction(cocos2d::Ref *sender)
{
    switch (action)
    {
        case ContextAction::unlock:
        {
            GameProfile::modifyProfileStat
                (ProfileStat::stars, -unlocksRef[currentSelection].getStarCost());
            GameProfile::modifyProfileStat
                (ProfileStat::backgroundsUnlocked, unlocksRef[currentSelection].getName());

            previewImages[currentSelection]->initWithFile
                ("backdrops/" + unlocksRef[currentSelection].getName() + "_puzzle.jpg");

            unlocksRef[currentSelection].setLocked(false);

            updateActionButton();
            updateNumStarsLabel();

            action = ContextAction::select;
            performContextAction(this);

            break;
        }
        case ContextAction::select:
        {
            GameProfile::setProfileStat
                (ProfileStat::currentBackground, unlocksRef[currentSelection].getName());

            updateBackdrop();

            break;
        }
        case ContextAction::null: {}
    }

    updateContextHintLabel();
}

void ProfileUnlocks::updateBackdrop()
{
    backdrop->initWithFile
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");
}

void ProfileUnlocks::updateSelectionRect()
{
    Vec2 currImagePos = previewImages[currentSelection]->getPosition();
    selectionRect->setPosition(Vec2(currImagePos.x, currImagePos.y));
}

void ProfileUnlocks::updateContextHintLabel()
{
    GameUnlock uRef = unlocksRef[currentSelection];
    std::string uRefCost = std::to_string(uRef.getStarCost());

    std::string toString;
    switch (action)
    {
        case ContextAction::unlock:
        {
            toString += "This backdrop costs " + uRefCost + " stars to unlock. ";
            toString += "Press 'Unlock' to access '" + uRef.getName() + "' now!";
            break;
        }
        case ContextAction::select:
        {
            if (previewImages[currentSelection]->getTag() == 
                GameProfile::stringToBackgroundID(GameProfile::getCurrentBackground()))
            {
                toString += "This backdrop is currently enabled.";
            }
            else
            {
                toString += "You own this backdrop. Press 'Select' to set '";
                toString += uRef.getName() + "' as your backdrop now!";
            }

            break;
        }
        case ContextAction::null:
        {
            toString += "This backdrop costs " + uRefCost + " stars to unlock. ";
            toString += "Earn more stars by completing puzzles!";
            break;
        }
    }

    contextHintLabel->setString(toString);
}

void ProfileUnlocks::updateNumStarsLabel()
{
    numStars->setString("x " + GameProfile::getProfileStat(ProfileStat::stars));
}


void ProfileUnlocks::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}