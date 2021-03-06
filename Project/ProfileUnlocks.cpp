#include "ProfileUnlocks.h"
#include "GameProfile.h"
#include "GameSettings.h"
#include "MainMenu.h"

USING_NS_CC;

ProfileUnlocks::ProfileUnlocks()
    : unlocksRef(GameProfile::getUnlocks())
    , previewImages(new Sprite*[GameProfile::getNumUnlocks()])
    , backdrop(nullptr)
    , selectionRect(nullptr)
    , actionButton (nullptr)
    , contextHintLabel (nullptr)
    , numStars(nullptr)
    , fadedOpacity(150)
    , selectedOpacity(255)
    , currentSelection(GameProfile::stringToBackgroundID
        (GameProfile::getCurrentBackground()))
    , action(ContextAction::select)
{
}

ProfileUnlocks::~ProfileUnlocks()
{
    delete[] previewImages;
}

Scene *ProfileUnlocks::createScene()
{
    Scene *scene = Scene::create();
    Layer *layer = ProfileUnlocks::create();

    scene->addChild(layer);

    return scene;
}

bool ProfileUnlocks::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Sprite *sceneTitle = Sprite::create("utility/unlocks.png");
    sceneTitle->setPosition
        (Vec2((visibleSize.width / 2), visibleSize.height - 100));
    this->addChild(sceneTitle, 2);

    initBackdrop();
    initStarDisplay();
    initMenu();
    initMenuPane();
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

    numStars = Label::createWithTTF("numStars", GameSettings::getFontName(), 28);
    numStars->setPosition(Vec2(star->getPositionX() + 100, star->getPositionY() - 50));
    numStars->enableGlow(Color4B::BLACK);

    updateNumStarsLabel();

    this->addChild(star, 1);
    this->addChild(numStars, 1);
}

void ProfileUnlocks::initMenu()
{
    actionButton = MenuItemSprite::create(
        Sprite::create("utility/select_up.png"),
        Sprite::create("utility/select_dn.png"),
        nullptr,
        CC_CALLBACK_1(ProfileUnlocks::performContextAction, this));

    MenuItemSprite *menuMain = MenuItemSprite::create(
        Sprite::create("utility/main_up.png"),
        Sprite::create("utility/main_dn.png"),
        nullptr,
        CC_CALLBACK_1(ProfileUnlocks::gotoMainMenu, this));

    Menu *menu = Menu::create(actionButton, menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    menuMain->setScale(0.66f);
    menuMain->setPosition(Vec2(0, -300));

    actionButton->setScale(0.66f);
    actionButton->setPosition(Vec2(0, -240));

    this->addChild(menu, 2);
}

void ProfileUnlocks::initMenuPane()
{
    Sprite *pane = Sprite::create();
    pane->setTextureRect(Rect(0, 0, 1366, 300));
    pane->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 2) + 50));
    pane->setColor(Color3B::BLACK);
    pane->setOpacity(175);

    this->addChild(pane, 1);
}

void ProfileUnlocks::initPreviewImages()
{
    std::string imgSuffix;

    for (int i = 0; i < 4; ++i)
    {
        Sprite *spr = Sprite::create();

        imgSuffix = unlocksRef[i].isLocked() ? "_locked.jpg" : "_puzzle.jpg";

        spr->initWithFile("backdrops/" + unlocksRef[i].getName() + imgSuffix);
        spr->setPosition(Vec2(250 + (i * 285) + (i * 2),
            (visibleSize.height / 2) + 100));
        spr->setScaleX(273 / spr->getContentSize().width);
        spr->setScaleY(154 / spr->getContentSize().height);
        spr->setOpacity(fadedOpacity);
        spr->setTag(i);

        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(ProfileUnlocks::imageClick, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spr);

        previewImages[i] = spr;
        
        Label *lbl = Label::createWithTTF
            (unlocksRef[i].getName(), GameSettings::getFontName(), 28);
        lbl->setPosition(Vec2(spr->getPositionX(), spr->getPositionY() - 100));

        this->addChild(spr, 3);
        this->addChild(lbl, 3);
    }

    previewImages[currentSelection]->setOpacity(selectedOpacity);

    selectionRect = Sprite::create();
    selectionRect->setTextureRect(Rect(0, 0, 280, 165));
    selectionRect->setColor(Color3B::YELLOW);

    updateSelectionRect();
    
    this->addChild(selectionRect, 2);
}

void ProfileUnlocks::initLabels()
{
    contextHintLabel = Label::createWithTTF
        ("contextHint", GameSettings::getFontName(), 26);
    contextHintLabel->setPosition
        (Vec2(visibleSize.width / 2, (visibleSize.height / 2) - 75));
    contextHintLabel->setColor(Color3B::YELLOW);

    updateContextHintLabel();

    this->addChild(contextHintLabel, 2);
}

bool ProfileUnlocks::imageClick(Touch* const touch, Event* const event)
{
    Sprite *spr = static_cast<Sprite*>(event->getCurrentTarget());
    Point pt = touch->getLocation();
    Rect recTemp = spr->getBoundingBox();

    if (!recTemp.containsPoint(pt))
    {
        return false;
    }

    int numUnlocks = GameProfile::getNumUnlocks();
    for (int i = 0; i < numUnlocks; ++i)
    {
        previewImages[i]->setOpacity(fadedOpacity);
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
    int starsAvailable = stoi(GameProfile::getProfileStat(ProfileStat::stars));

    if (unlocksRef[currentSelection].isLocked())
    {
        if (starsAvailable >= unlocksRef[currentSelection].getStarCost())
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
    }
    else
    {
        actionButton->initWithNormalSprite(
            Sprite::create("utility/select_up.png"),
            Sprite::create("utility/select_dn.png"),
            nullptr,
            CC_CALLBACK_1(ProfileUnlocks::performContextAction, this));
        action = ContextAction::select;
    }
}

void ProfileUnlocks::performContextAction(Ref* const sender)
{
    switch (action)
    {
        case ContextAction::unlock:
        {
            GameProfile::modifyProfileStat(ProfileStat::stars, 
                -unlocksRef[currentSelection].getStarCost());
            GameProfile::modifyProfileStat(ProfileStat::backgroundsUnlocked, 
                unlocksRef[currentSelection].getName());

            previewImages[currentSelection]->initWithFile
                ("backdrops/" + unlocksRef[currentSelection].getName() + "_puzzle.jpg");

            unlocksRef[currentSelection].setLocked(false);

            updateActionButton();
            updateNumStarsLabel();

            action = ContextAction::select;
            performContextAction();

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

void ProfileUnlocks::updateBackdrop() const
{
    backdrop->initWithFile
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");
}

void ProfileUnlocks::updateSelectionRect() const
{
    Vec2 currImagePos = previewImages[currentSelection]->getPosition();
    selectionRect->setPosition(Vec2(currImagePos.x, currImagePos.y));
}

void ProfileUnlocks::updateContextHintLabel() const
{
    GameUnlock &unlock = unlocksRef[currentSelection];
    std::string unlockCost = std::to_string(unlock.getStarCost());

    std::string str;
    switch (action)
    {
        case ContextAction::unlock:
        {
            str += "This backdrop costs " + unlockCost + " stars to unlock. ";
            str += "Press 'Unlock' to access '" + unlock.getName() + "' now!";

            break;
        }
        case ContextAction::select:
        {
            if (previewImages[currentSelection]->getTag() == 
                GameProfile::stringToBackgroundID(GameProfile::getCurrentBackground()))
            {
                str += "This backdrop is currently enabled.";
            }
            else
            {
                str += "You own this backdrop. Press 'Select' to set '";
                str += unlock.getName() + "' as your backdrop now!";
            }

            break;
        }
        case ContextAction::null:
        {
            str += "This backdrop costs " + unlockCost + " stars to unlock. ";
            str += "Earn more stars by completing puzzles!";

            break;
        }
    }

    contextHintLabel->setString(str);
}

void ProfileUnlocks::updateNumStarsLabel() const
{
    numStars->setString("x " + GameProfile::getProfileStat(ProfileStat::stars));
}


void ProfileUnlocks::gotoMainMenu(Ref* const sender) const
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}