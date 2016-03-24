#include "GameSettings.h"
#include "settingsMenu.h"
#include "ImageMenu.h"

USING_NS_CC;

cocos2d::Scene *ImageMenu::createScene()
{
    cocos2d::Scene *scene = Scene::create();
    auto layer = ImageMenu::create();

    scene->addChild(layer);

    return scene;
}

bool ImageMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }

    initMenu();
    initLabels();
    initPreviewImage();

    return true;
}

void ImageMenu::initMenu()
{
    MenuItemSprite *leftArrow = new MenuItemSprite();
    leftArrow->initWithNormalSprite(
        Sprite::create("utility/arrow_up.png"),
        Sprite::create("utility/arrow_dn.png"),
        nullptr,
        CC_CALLBACK_1(ImageMenu::leftArrowClick, this));

    MenuItemSprite *rightArrow = new MenuItemSprite();
    rightArrow->initWithNormalSprite(
        Sprite::create("utility/arrow_up.png"),
        Sprite::create("utility/arrow_dn.png"),
        nullptr,
        CC_CALLBACK_1(ImageMenu::rightArrowClick, this));

    MenuItemFont *back = MenuItemFont::create(
        "Back",
        CC_CALLBACK_1(ImageMenu::gotoSettingsMenu, this));

    cocos2d::Menu *menu = Menu::create(leftArrow, rightArrow, back, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    leftArrow->setPosition(Vec2(-300, 25));
    leftArrow->setScale(0.3f);
    leftArrow->setRotation3D(Vec3(0, 180, 0));

    rightArrow->setPosition(Vec2(300, 25));
    rightArrow->setScale(0.3f);

    back->setPositionY(-300);

    this->addChild(menu);
}

void ImageMenu::initLabels()
{
    cocos2d::Label *sceneTitle = Label::createWithTTF
        ("Image Menu", "fonts/Marker Felt.ttf", 32);
    sceneTitle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));

    imageName = Label::createWithTTF("Placeholder", "fonts/Marker Felt.ttf", 22);
    imageName->setPosition(Vec2(visibleSize.width / 2, 250));

    imageNumber = Label::createWithTTF("Placeholder", "fonts/Marker Felt.ttf", 20);
    imageNumber->setPosition(Vec2(visibleSize.width / 2, 220));

    updateImageLabels();

    this->addChild(sceneTitle);
    this->addChild(imageName);
    this->addChild(imageNumber);
}

void ImageMenu::initPreviewImage()
{
    displayedImage = Sprite::create();
    displayedImage->setPosition
        (Vec2(visibleSize.width / 2, (visibleSize.height / 2) + 50));

    updateDisplayedImage();

    this->addChild(displayedImage);
}

bool ImageMenu::leftArrowClick(cocos2d::Ref *sender)
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

bool ImageMenu::rightArrowClick(cocos2d::Ref *sender)
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

void ImageMenu::updateImageLabels()
{
    imageName->setString(GameSettings::getImageName());
    imageNumber->setString("( " + std::to_string(GameSettings::getImageID() + 1) + " / " + std::to_string(GameSettings::getPuzzles().size()) + " )");
}

void ImageMenu::updateDisplayedImage()
{
    displayedImage->initWithFile(GameSettings::getImageName());
    displayedImage->setScaleX(400 / displayedImage->getContentSize().width);
    displayedImage->setScaleY(300 / displayedImage->getContentSize().height);
}

void ImageMenu::gotoSettingsMenu(cocos2d::Ref* sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, SettingsMenu::createScene()));
}