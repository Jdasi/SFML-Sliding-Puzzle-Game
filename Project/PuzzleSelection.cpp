#include "GameSettings.h"
#include "GameProfile.h"
#include "PuzzleSelection.h"
#include "MainMenu.h"
#include "PuzzleGame.h"

USING_NS_CC;

PuzzleSelection::PuzzleSelection()
    : xSliderLabel(nullptr)
    , ySliderLabel(nullptr)
    , xSegmentsSlider(nullptr)
    , ySegmentsSlider(nullptr)
    , displayedImage(nullptr)
    , imageName(nullptr)
    , imageNumber(nullptr)
    , puzzleValue(nullptr)
{
}

Scene *PuzzleSelection::createScene()
{
    Scene *scene = Scene::create();
    auto layer = PuzzleSelection::create();

    scene->addChild(layer);

    return scene;
}

bool PuzzleSelection::init()
{
    if (!Layer::init())
    {
        return false;
    }

    GameSettings::clearPuzzles();
    GameSettings::enumeratePuzzles();

    // Circumvent most problems with image discrepencies since last enumeration.
    unsigned int tempID = GameSettings::getImageID();
    if (tempID < GameSettings::getPuzzles().size())
    {
        if (GameSettings::getImageName() !=
            ("puzzles/" + GameSettings::getPuzzles()[GameSettings::getImageID()]))
        {
            setDefaultPuzzle();
        }
    }
    else
    {
        setDefaultPuzzle();
    }

    Sprite *sceneTitle = Sprite::create("utility/chooseLabel.png");
    sceneTitle->setPosition
        (Vec2((visibleSize.width / 2) - 150, visibleSize.height - 100));

    this->addChild(sceneTitle, 1);

    initBackdrop();
    initMenu();
    initSliders();
    initPreviewImage();
    initArrows();
    initRewardsPane();

    return true;
}

void PuzzleSelection::initBackdrop()
{
    Sprite *backdrop = Sprite::create
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");

    backdrop->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    this->addChild(backdrop, 0);
}

void PuzzleSelection::initMenu()
{
    MenuItemSprite *menuPlay = new MenuItemSprite();
    menuPlay->initWithNormalSprite(
        Sprite::create("utility/play_up.png"),
        Sprite::create("utility/play_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleSelection::gotoPuzzleGame, this));

    MenuItemSprite *menuMain = new MenuItemSprite();
    menuMain->initWithNormalSprite(
        Sprite::create("utility/main_up.png"),
        Sprite::create("utility/main_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleSelection::gotoMainMenu, this));

    cocos2d::Menu *menu = Menu::create(menuPlay, menuMain, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    menuPlay->setScale(0.66f);
    menuMain->setScale(0.66f);

    menuPlay->setPosition(Vec2(400, -240));
    menuMain->setPosition(Vec2(400, -300));

    Sprite *pane = Sprite::create();
    pane->setTextureRect(Rect(0, 0, 300, 768));
    pane->setPosition(Vec2((visibleSize.width / 2) + 400, visibleSize.height / 2));
    pane->setColor(Color3B::BLACK);
    pane->setOpacity(175);

    this->addChild(menu, 2);
    this->addChild(pane, 1);
}

void PuzzleSelection::initSliders()
{
    xSegmentsSlider = ui::Slider::create();
    xSegmentsSlider->loadBarTexture("utility/Slider_Back.png");
    xSegmentsSlider->loadSlidBallTextures("utility/SliderNode_Normal.png",
        "utility/SliderNode_Press.png",
        "utility/SliderNode_Disable.png");
    xSegmentsSlider->loadProgressBarTexture("utility/Slider_PressBar.png");
    xSegmentsSlider->setPosition(Vec2(
        (visibleSize.width / 2) + 400, (visibleSize.height / 2) + 150));
    xSegmentsSlider->setPercent(GameSettings::getSegments().x - 3);
    xSegmentsSlider->setMaxPercent(7);

    xSliderLabel = Label::createWithTTF("X Segments: " + std::to_string
        (xSegmentsSlider->getPercent() + 3), GameSettings::getFontName(), 26);
    xSliderLabel->setPosition(Vec2
        (xSegmentsSlider->getPosition().x, xSegmentsSlider->getPosition().y + 30));

    xSegmentsSlider->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
        switch (type)
        {
        case ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
        {
            updateRewardsLabel();
            GameSettings::setSegmentsX(xSegmentsSlider->getPercent() + 3);
            xSliderLabel->setString
                ("X Segments: " + std::to_string(xSegmentsSlider->getPercent() + 3));

            break;
        }
        default: {}
        }
    });

    ySegmentsSlider = ui::Slider::create();
    ySegmentsSlider->loadBarTexture("utility/Slider_Back.png");
    ySegmentsSlider->loadSlidBallTextures("utility/SliderNode_Normal.png",
        "utility/SliderNode_Press.png",
        "utility/SliderNode_Disable.png");
    ySegmentsSlider->loadProgressBarTexture("utility/Slider_PressBar.png");
    ySegmentsSlider->setPosition(Vec2(
        (visibleSize.width / 2) + 400, (visibleSize.height / 2) + 50));
    ySegmentsSlider->setPercent(GameSettings::getSegments().y - 3);
    ySegmentsSlider->setMaxPercent(7);

    ySliderLabel = Label::createWithTTF("Y Segments: " + std::to_string
        (ySegmentsSlider->getPercent() + 3), GameSettings::getFontName(), 26);
    ySliderLabel->setPosition(Vec2
        (ySegmentsSlider->getPosition().x, ySegmentsSlider->getPosition().y + 30));

    ySegmentsSlider->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
        switch (type)
        {
        case ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
        {
            updateRewardsLabel();
            GameSettings::setSegmentsY(ySegmentsSlider->getPercent() + 3);
            ySliderLabel->setString
                ("Y Segments: " + std::to_string(ySegmentsSlider->getPercent() + 3));

            break;
        }
        default: {}
        }
    });

    this->addChild(xSegmentsSlider, 2);
    this->addChild(ySegmentsSlider, 2);

    this->addChild(xSliderLabel, 2);
    this->addChild(ySliderLabel, 2);
}

void PuzzleSelection::initPreviewImage()
{
    displayedImage = Sprite::create();
    displayedImage->setPosition
        (Vec2((visibleSize.width / 2) - 150, (visibleSize.height / 2)));

    updateDisplayedImage();

    imageName = Label::createWithTTF("imageName", GameSettings::getFontName(), 28);
    imageName->setPosition(Vec2((visibleSize.width / 2) - 150, 150));
    imageName->enableGlow(Color4B::BLACK);

    imageNumber = Label::createWithTTF("imageNumber", GameSettings::getFontName(), 26);
    imageNumber->setPosition(Vec2((visibleSize.width / 2) - 150, 120));
    imageNumber->enableGlow(Color4B::BLACK);

    updateImageLabels();

    this->addChild(displayedImage, 1);
    this->addChild(imageName, 1);
    this->addChild(imageNumber, 1);
}

void PuzzleSelection::initArrows()
{
    MenuItemSprite *leftArrow = new MenuItemSprite();
    leftArrow->initWithNormalSprite(
        Sprite::create("utility/arrow_up.png"),
        Sprite::create("utility/arrow_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleSelection::leftArrowClick, this));

    MenuItemSprite *rightArrow = new MenuItemSprite();
    rightArrow->initWithNormalSprite(
        Sprite::create("utility/arrow_up.png"),
        Sprite::create("utility/arrow_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleSelection::rightArrowClick, this));

    cocos2d::Menu *menu = Menu::create(leftArrow, rightArrow, nullptr);
    menu->setPosition(Vec2((visibleSize.width / 2) - 150, (visibleSize.height / 2)));

    leftArrow->setPosition(Vec2(-325, 0));
    leftArrow->setScale(0.3f);
    leftArrow->setRotation(180.0f);

    rightArrow->setPosition(Vec2(325, 0));
    rightArrow->setScale(0.3f);

    this->addChild(menu, 1);
}

void PuzzleSelection::initRewardsPane()
{
    Label *rewardDesc = Label::createWithTTF
        ("This Puzzle is worth:", GameSettings::getFontName(), 26);
    rewardDesc->setPosition(Vec2((visibleSize.width / 2) + 400, 350));

    Sprite *star = Sprite::create("utility/star.png");
    star->setPosition(Vec2((visibleSize.width / 2) + 400, 280));
    star->setScale(0.25f);
    
    puzzleValue = Label::createWithTTF("puzzleValue", GameSettings::getFontName(), 24);
    puzzleValue->setPosition(Vec2(star->getPositionX() + 75, star->getPositionY()));

    updateRewardsLabel();

    this->addChild(rewardDesc, 1);
    this->addChild(star, 1);
    this->addChild(puzzleValue, 1);
}

void PuzzleSelection::updateDisplayedImage() const
{
    displayedImage->initWithFile(GameSettings::getImageName());
    displayedImage->setScaleX(528 / displayedImage->getContentSize().width);
    displayedImage->setScaleY(396 / displayedImage->getContentSize().height);
}

void PuzzleSelection::updateImageLabels() const
{
    imageName->setString(GameSettings::getImageName());
    imageNumber->setString
        ("( " + std::to_string(GameSettings::getImageID() + 1) + 
         " / " + std::to_string(GameSettings::getPuzzles().size()) + " )");
}

void PuzzleSelection::updateRewardsLabel() const
{
    puzzleValue->setString("x " + std::to_string(calculatePuzzleValue()));
}

bool PuzzleSelection::leftArrowClick(cocos2d::Ref *sender) const
{
    int currentImageID = GameSettings::getImageID();

    if (--currentImageID < 0)
    {
        currentImageID = GameSettings::getPuzzles().size() - 1;
    }

    GameSettings::setImageName
        ("puzzles/" + GameSettings::getPuzzles()[currentImageID], currentImageID);

    updateDisplayedImage();
    updateImageLabels();

    return true;
}

bool PuzzleSelection::rightArrowClick(cocos2d::Ref *sender) const
{
    unsigned int currentImageID = GameSettings::getImageID();

    if (++currentImageID >(GameSettings::getPuzzles().size() - 1))
    {
        currentImageID = 0;
    }

    GameSettings::setImageName
        ("puzzles/" + GameSettings::getPuzzles()[currentImageID], currentImageID);

    updateDisplayedImage();
    updateImageLabels();

    return true;
}

void PuzzleSelection::gotoMainMenu(cocos2d::Ref *sender) const
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}

void PuzzleSelection::gotoPuzzleGame(cocos2d::Ref *sender) const
{
    GameSettings::setCurrentPuzzleValue(calculatePuzzleValue());

    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleGame::createScene()));
}

void PuzzleSelection::setDefaultPuzzle() const
{
    GameSettings::setImageName("puzzles/" + GameSettings::getPuzzles()[0], 0);
}

int PuzzleSelection::calculatePuzzleValue() const
{
    int xSegments = GameSettings::getSegments().x;
    int ySegments = GameSettings::getSegments().y;

    return ((xSegments * ySegments) * 2) / 2;
}