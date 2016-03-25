#include "GameSettings.h"
#include "PuzzleSelection.h"
#include "MainMenu.h"
#include "PuzzleGame.h"

USING_NS_CC;

cocos2d::Scene *PuzzleSelection::createScene()
{
    cocos2d::Scene *scene = Scene::create();
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

    initBackdrop();
    initMenu();
    initSliders();
    initLabels();
    initPreviewImage();
    initArrows();

    return true;
}

void PuzzleSelection::initBackdrop()
{
    Sprite *backdrop = Sprite::create("backdrops/regal_menu.jpg");

    backdrop->setAnchorPoint(Vec2(0, 0));
    backdrop->setPosition(Vec2(0, 0));

    this->addChild(backdrop, 0);
}

void PuzzleSelection::initMenu()
{
    MenuItemFont *menuStart = MenuItemFont::create(
        "Start",
        CC_CALLBACK_1(PuzzleSelection::gotoPuzzleGame, this));

    MenuItemFont *menuBack = MenuItemFont::create(
        "Back",
        CC_CALLBACK_1(PuzzleSelection::gotoMainMenu, this));

    cocos2d::Menu *menu = Menu::create(menuStart, menuBack, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    menuStart->setPosition(Vec2(300, -250));
    menuBack->setPosition(Vec2(300, -300));

    this->addChild(menu);
}

void PuzzleSelection::initLabels()
{
    cocos2d::Label *sceneTitle = Label::createWithTTF("Choose a Puzzle!", "fonts/Marker Felt.ttf", 32);
    sceneTitle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));

    imageName = Label::createWithTTF("imageName", "fonts/Marker Felt.ttf", 22);
    imageName->setPosition(Vec2((visibleSize.width / 2) - 250, 250));

    imageNumber = Label::createWithTTF("imageNumber", "fonts/Marker Felt.ttf", 20);
    imageNumber->setPosition(Vec2((visibleSize.width / 2) - 250, 220));

    updateImageLabels();

    this->addChild(sceneTitle);
    this->addChild(imageName);
    this->addChild(imageNumber);
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
        (visibleSize.width / 2) + 300, (visibleSize.height / 2) + 100));
    xSegmentsSlider->setPercent(GameSettings::getSegments().x - 3);
    xSegmentsSlider->setMaxPercent(7);

    xSliderLabel = Label::createWithTTF("X Segments: " + 
        std::to_string(xSegmentsSlider->getPercent() + 3), "fonts/Marker Felt.ttf", 24);
    xSliderLabel->setPosition(Vec2
        (xSegmentsSlider->getPosition().x, xSegmentsSlider->getPosition().y + 30));

    xSegmentsSlider->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
        switch (type)
        {
        case ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
        {
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
        (visibleSize.width / 2) + 300, (visibleSize.height / 2)));
    ySegmentsSlider->setPercent(GameSettings::getSegments().y - 3);
    ySegmentsSlider->setMaxPercent(7);

    ySliderLabel = Label::createWithTTF("Y Segments: " + 
        std::to_string(ySegmentsSlider->getPercent() + 3), "fonts/Marker Felt.ttf", 24);
    ySliderLabel->setPosition(Vec2
        (ySegmentsSlider->getPosition().x, ySegmentsSlider->getPosition().y + 30));

    ySegmentsSlider->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
        switch (type)
        {
        case ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
        {
            GameSettings::setSegmentsY(ySegmentsSlider->getPercent() + 3);
            ySliderLabel->setString
                ("Y Segments: " + std::to_string(ySegmentsSlider->getPercent() + 3));

            break;
        }
        default: {}
        }
    });

    this->addChild(xSegmentsSlider);
    this->addChild(ySegmentsSlider);

    this->addChild(xSliderLabel);
    this->addChild(ySliderLabel);

}

void PuzzleSelection::initPreviewImage()
{
    displayedImage = Sprite::create();
    displayedImage->setPosition
        (Vec2((visibleSize.width / 2) - 250, (visibleSize.height / 2) + 50));

    updateDisplayedImage();

    this->addChild(displayedImage);
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
    menu->setPosition(Vec2((visibleSize.width / 2) - 250, (visibleSize.height / 2) + 50));

    leftArrow->setPosition(Vec2(-300, 0));
    leftArrow->setScale(0.3f);
    leftArrow->setRotation3D(Vec3(0, 180, 0));

    rightArrow->setPosition(Vec2(300, 0));
    rightArrow->setScale(0.3f);

    this->addChild(menu);
}

bool PuzzleSelection::leftArrowClick(cocos2d::Ref *sender)
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

bool PuzzleSelection::rightArrowClick(cocos2d::Ref *sender)
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

void PuzzleSelection::updateDisplayedImage()
{
    displayedImage->initWithFile(GameSettings::getImageName());
    displayedImage->setScaleX(400 / displayedImage->getContentSize().width);
    displayedImage->setScaleY(300 / displayedImage->getContentSize().height);
}

void PuzzleSelection::updateImageLabels()
{
    imageName->setString(GameSettings::getImageName());
    imageNumber->setString("( " + std::to_string(GameSettings::getImageID() + 1) + " / " + std::to_string(GameSettings::getPuzzles().size()) + " )");
}

void PuzzleSelection::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}

void PuzzleSelection::gotoPuzzleGame(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, PuzzleGame::createScene()));
}