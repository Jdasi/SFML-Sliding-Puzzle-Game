#include "GameSettings.h"
#include "GameProfile.h"
#include "PuzzleSelection.h"
#include "MainMenu.h"
#include "PuzzleGame.h"

USING_NS_CC;

PuzzleSelection::PuzzleSelection()
    : puzzlesRef(GameSettings::getPuzzles())
    , xSliderLabel(nullptr)
    , ySliderLabel(nullptr)
    , xSegmentsSlider(nullptr)
    , ySegmentsSlider(nullptr)
    , displayedImage(nullptr)
    , imageName(nullptr)
    , imageNumber(nullptr)
    , puzzleValue(nullptr)
    , animSwitch(nullptr)
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

    GameSettings::enumeratePuzzles();
    sanityCheckPuzzles();

    Sprite *sceneTitle = Sprite::create("utility/chooseLabel.png");
    sceneTitle->setPosition
        (Vec2((visibleSize.width / 2) - 150, visibleSize.height - 100));

    this->addChild(sceneTitle, 1);

    initBackdrop();
    initControlMenu();
    initHelpMenu();
    initMenuPane();
    initSliders();
    initPreviewImage();
    initArrows();
    initRewardsPane();
    initAnimSwitch();

    return true;
}

void PuzzleSelection::initBackdrop()
{
    Sprite *backdrop = Sprite::create
        ("backdrops/" + GameProfile::getCurrentBackground() + "_menu.jpg");

    backdrop->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    this->addChild(backdrop, 0);
}

void PuzzleSelection::initControlMenu()
{
    MenuItemSprite *menuPlay = MenuItemSprite::create(
        Sprite::create("utility/play_up.png"),
        Sprite::create("utility/play_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleSelection::gotoPuzzleGame, this));
    menuPlay->setScale(0.66f);

    MenuItemSprite *menuMain = MenuItemSprite::create(
        Sprite::create("utility/main_up.png"),
        Sprite::create("utility/main_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleSelection::gotoMainMenu, this));
    menuMain->setScale(0.66f);

    Menu *controlMenu = Menu::create(menuPlay, menuMain, nullptr);
    controlMenu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    menuPlay->setPosition(Vec2(400, -240));
    menuMain->setPosition(Vec2(400, -300));

    this->addChild(controlMenu, 2);
}

void PuzzleSelection::initHelpMenu()
{
    MenuItemSprite *menuHelp = MenuItemSprite::create(
        Sprite::create("utility/help_up.png"),
        Sprite::create("utility/help_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleSelection::puzzleTip, this));
    menuHelp->setScale(0.5f);

    Menu *helpMenu = Menu::create(menuHelp, nullptr);
    helpMenu->setPosition(Vec2
        ((visibleSize.width / 2) + 100, (visibleSize.height / 2) + 200));

    this->addChild(helpMenu, 2);
}

void PuzzleSelection::initMenuPane()
{
    Sprite *pane = Sprite::create();
    pane->setTextureRect(Rect(0, 0, 300, 768));
    pane->setPosition(Vec2((visibleSize.width / 2) + 400, visibleSize.height / 2));
    pane->setColor(Color3B::BLACK);
    pane->setOpacity(175);

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
    xSegmentsSlider->setPosition(Vec2
        ((visibleSize.width / 2) + 400, (visibleSize.height / 2) + 130));
    xSegmentsSlider->setPercent(GameSettings::getSegments().x - 3);
    xSegmentsSlider->setMaxPercent(4);

    xSliderLabel = Label::createWithTTF("X Segments: " + std::to_string
        (xSegmentsSlider->getPercent() + 3), GameSettings::getFontName(), 26);
    xSliderLabel->setPosition(Vec2
        (xSegmentsSlider->getPosition().x, xSegmentsSlider->getPosition().y + 30));
    xSliderLabel->setColor(Color3B::YELLOW);

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
    ySegmentsSlider->setPosition(Vec2
        ((visibleSize.width / 2) + 400, (visibleSize.height / 2) + 35));
    ySegmentsSlider->setPercent(GameSettings::getSegments().y - 3);
    ySegmentsSlider->setMaxPercent(4);

    ySliderLabel = Label::createWithTTF("Y Segments: " + std::to_string
        (ySegmentsSlider->getPercent() + 3), GameSettings::getFontName(), 26);
    ySliderLabel->setPosition(Vec2
        (ySegmentsSlider->getPosition().x, ySegmentsSlider->getPosition().y + 30));
    ySliderLabel->setColor(Color3B::YELLOW);

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
    MenuItemSprite *leftArrow = MenuItemSprite::create(
        Sprite::create("utility/arrow_up.png"),
        Sprite::create("utility/arrow_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleSelection::leftArrowClick, this));

    MenuItemSprite *rightArrow = MenuItemSprite::create(
        Sprite::create("utility/arrow_up.png"),
        Sprite::create("utility/arrow_dn.png"),
        nullptr,
        CC_CALLBACK_1(PuzzleSelection::rightArrowClick, this));

    Menu *menu = Menu::create(leftArrow, rightArrow, nullptr);
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
    rewardDesc->setColor(Color3B::YELLOW);

    Sprite *star = Sprite::create("utility/star.png");
    star->setPosition(Vec2((visibleSize.width / 2) + 400, 280));
    star->setScale(0.25f);
    
    puzzleValue = Label::createWithTTF("puzzleValue", GameSettings::getFontName(), 26);
    puzzleValue->setPosition(Vec2(star->getPositionX() + 80, star->getPositionY()));

    updateRewardsLabel();

    this->addChild(rewardDesc, 1);
    this->addChild(star, 1);
    this->addChild(puzzleValue, 1);
}

void PuzzleSelection::initAnimSwitch()
{
    animSwitch = Sprite::create("utility/switch_on.png");
    animSwitch->setPosition(Vec2
        ((visibleSize.width / 2) + 400, (visibleSize.height / 2) + 250));
    animSwitch->setScale(0.66f);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan =
        CC_CALLBACK_2(PuzzleSelection::flipAnimSwitch, this);
    _eventDispatcher->
        addEventListenerWithSceneGraphPriority(listener, animSwitch);

    Label *animLabel = Label::createWithTTF
        ("Animated Shuffling", GameSettings::getFontName(), 26);
    animLabel->setPosition(Vec2
        (animSwitch->getPositionX(), animSwitch->getPositionY() + 50));
    animLabel->setColor(Color3B::YELLOW);

    updateAnimSwitch();

    this->addChild(animSwitch, 2);
    this->addChild(animLabel, 2);
}

bool PuzzleSelection::leftArrowClick(Ref* const sender) const
{
    int currentImageID = GameSettings::getImageID();
    if (--currentImageID < 0)
    {
        currentImageID = puzzlesRef.size() - 1;
    }

    updateImageAndLabels(currentImageID);

    return true;
}

bool PuzzleSelection::rightArrowClick(Ref* const sender) const
{
    unsigned int currentImageID = GameSettings::getImageID();
    if (++currentImageID > (puzzlesRef.size() - 1))
    {
        currentImageID = 0;
    }

    updateImageAndLabels(currentImageID);

    return true;
}

void PuzzleSelection::updateImageAndLabels(int currentImageID) const
{
    GameSettings::setImageName
        ("puzzles/" + puzzlesRef[currentImageID], currentImageID);

    updateDisplayedImage();
    updateImageLabels();
}

void PuzzleSelection::updateDisplayedImage() const
{
    displayedImage->initWithFile(GameSettings::getImageName());
    displayedImage->setScaleX(528 / displayedImage->getContentSize().width);
    displayedImage->setScaleY(396 / displayedImage->getContentSize().height);
}

void PuzzleSelection::updateImageLabels() const
{
    std::string currentImageID = std::to_string(GameSettings::getImageID() + 1);
    std::string totalPuzzles = std::to_string(puzzlesRef.size());

    imageName->setString(GameSettings::getImageName());
    imageNumber->setString("( " + currentImageID + " / " + totalPuzzles + " )");
}

void PuzzleSelection::updateRewardsLabel() const
{
    puzzleValue->setString("x " + std::to_string(calculatePuzzleValue()));
}

void PuzzleSelection::puzzleTip(Ref* const sender) const
{
    const char tip[]
    {
        "You can expand your puzzle library by adding images to the puzzles folder. \
        (Accepted image types are .jpg and .png)"
    };

    MessageBox(tip, "Square Slide: Puzzles Tip");
}

// Circumvent most problems with image discrepencies since last enumeration.
void PuzzleSelection::sanityCheckPuzzles()
{
    unsigned int tempID = GameSettings::getImageID();
    if (tempID < puzzlesRef.size())
    {
        if (GameSettings::getImageName() !=
            ("puzzles/" + puzzlesRef[GameSettings::getImageID()]))
        {
            setDefaultPuzzle();
        }
    }
    else
    {
        setDefaultPuzzle();
    }
}

void PuzzleSelection::setDefaultPuzzle() const
{
    GameSettings::setImageName("puzzles/" + puzzlesRef[0], 0);
}

int PuzzleSelection::calculatePuzzleValue() const
{
    int xSegments = GameSettings::getSegments().x;
    int ySegments = GameSettings::getSegments().y;

    return ((xSegments * ySegments) * 2) / 2;
}

bool PuzzleSelection::flipAnimSwitch(Touch* const touch, Event* const event) const
{
    Rect recTemp = event->getCurrentTarget()->getBoundingBox();
    if (!recTemp.containsPoint(touch->getLocation()))
    {
        return false;
    }

    updateAnimStatus();
    updateAnimSwitch();

    return true;
}

void PuzzleSelection::updateAnimStatus() const
{
    GameProfile::setProfileStat(ProfileStat::animatedShuffling,
                                !GameProfile::animatedShufflingEnabled());
}

void PuzzleSelection::updateAnimSwitch() const
{
    animSwitch->initWithFile(GameProfile::animatedShufflingEnabled() ?
                             "utility/switch_on.png" : "utility/switch_off.png");
}

void PuzzleSelection::gotoMainMenu(Ref* const sender) const
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}

void PuzzleSelection::gotoPuzzleGame(Ref* const sender) const
{
    GameSettings::setCurrentPuzzleValue(calculatePuzzleValue());

    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleGame::createScene()));
}