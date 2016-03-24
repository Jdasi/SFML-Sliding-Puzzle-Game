#include "GameSettings.h"
#include "settingsMenu.h"
#include "MainMenu.h"
#include "ImageMenu.h"

USING_NS_CC;

cocos2d::Scene *SettingsMenu::createScene()
{
    cocos2d::Scene *scene = Scene::create();
    auto layer = SettingsMenu::create();

    scene->addChild(layer);

    return scene;
}

bool SettingsMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }

    cocos2d::Label *label = Label::createWithTTF("Settings", "fonts/Marker Felt.ttf", 32);
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
    this->addChild(label, 1);

    initMenu();

    return true;
}


void SettingsMenu::initMenu()
{
    MenuItemFont *changeImage = MenuItemFont::create(
        "Change Image",
        CC_CALLBACK_1(SettingsMenu::gotoImageMenu, this));

    MenuItemFont *mainMenu = MenuItemFont::create(
        "Main Menu",
        CC_CALLBACK_1(SettingsMenu::gotoMainMenu, this));

    cocos2d::Menu *menu = Menu::create(changeImage, mainMenu, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    changeImage->setPositionY(-50);
    mainMenu->setPositionY(-250);

    this->addChild(menu, 1);

    // --------------------------------------------

    xSegmentsSlider = ui::Slider::create();
    xSegmentsSlider->loadBarTexture("utility/Slider_Back.png");
    xSegmentsSlider->loadSlidBallTextures("utility/SliderNode_Normal.png",
                                          "utility/SliderNode_Press.png",
                                          "utility/SliderNode_Disable.png");
    xSegmentsSlider->loadProgressBarTexture("utility/Slider_PressBar.png");
    xSegmentsSlider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 250));
    xSegmentsSlider->setPercent(GameSettings::getSegments().x - 3);
    xSegmentsSlider->setMaxPercent(7);

    xSliderLabel = Label::createWithTTF("X Segments: " + std::to_string(xSegmentsSlider->getPercent() + 3), "fonts/Marker Felt.ttf", 24);
    xSliderLabel->setPosition(Vec2(xSegmentsSlider->getPosition().x, xSegmentsSlider->getPosition().y + 30));
    this->addChild(xSliderLabel, 1);

    xSegmentsSlider->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
        switch (type)
        {
        case ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
        {
            GameSettings::setSegmentsX(xSegmentsSlider->getPercent() + 3);
            xSliderLabel->setString("X Segments: " + std::to_string(xSegmentsSlider->getPercent() + 3));

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
    ySegmentsSlider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 350));
    ySegmentsSlider->setPercent(GameSettings::getSegments().y - 3);
    ySegmentsSlider->setMaxPercent(7);

    ySliderLabel = Label::createWithTTF("Y Segments: " + std::to_string(ySegmentsSlider->getPercent() + 3), "fonts/Marker Felt.ttf", 24);
    ySliderLabel->setPosition(Vec2(ySegmentsSlider->getPosition().x, ySegmentsSlider->getPosition().y + 30));
    this->addChild(ySliderLabel, 1);

    ySegmentsSlider->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
        switch (type)
        {
        case ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
        {
            GameSettings::setSegmentsY(ySegmentsSlider->getPercent() + 3);
            ySliderLabel->setString("Y Segments: " + std::to_string(ySegmentsSlider->getPercent() + 3));

            break;
        }
        default: {}
        }
    });

    this->addChild(xSegmentsSlider);
    this->addChild(ySegmentsSlider);
}

void SettingsMenu::gotoImageMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, ImageMenu::createScene()));
}

void SettingsMenu::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, MainMenu::createScene()));
}