#include "GameSettings.h"
#include "settingsMenu.h"
#include "MainMenu.h"

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

    cocos2d::Label *label = Label::createWithTTF("Settings", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 150));
    this->addChild(label, 1);

    initMenu();

    return true;
}


void SettingsMenu::initMenu()
{
    MenuItemFont *mainMenu = MenuItemFont::create(
        "Main Menu",
        CC_CALLBACK_1(SettingsMenu::gotoMainMenu, this));

    cocos2d::Menu *menu = Menu::create(mainMenu, nullptr);
    menu->alignItemsVertically();
    this->addChild(menu, 1);

    // --------------------------------------------

    xSegmentsSlider = ui::Slider::create();
    xSegmentsSlider->loadBarTexture("Slider_Back.png"); // what the slider looks like
    xSegmentsSlider->loadSlidBallTextures("SliderNode_Normal.png", "SliderNode_Press.png", "SliderNode_Disable.png");
    xSegmentsSlider->loadProgressBarTexture("Slider_PressBar.png");
    xSegmentsSlider->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 2) - 150));
    xSegmentsSlider->setPercent(1);
    xSegmentsSlider->setMaxPercent(7);

    xSliderLabel = Label::createWithTTF(std::to_string(xSegmentsSlider->getPercent() + 3), "fonts/Marker Felt.ttf", 24);
    xSliderLabel->setPosition(Vec2(xSegmentsSlider->getPosition().x, xSegmentsSlider->getPosition().y + 25));
    this->addChild(xSliderLabel, 1);

    xSegmentsSlider->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
        {
            break;
        }
        case ui::Widget::TouchEventType::ENDED:
        {
            GameSettings::setSegmentsX(xSegmentsSlider->getPercent() + 3);
            xSliderLabel->setString(std::to_string(xSegmentsSlider->getPercent() + 3));
            break;
        }
        default: {}
        }
    });

    ySegmentsSlider = ui::Slider::create();
    ySegmentsSlider->loadBarTexture("Slider_Back.png"); // what the slider looks like
    ySegmentsSlider->loadSlidBallTextures("SliderNode_Normal.png", "SliderNode_Press.png", "SliderNode_Disable.png");
    ySegmentsSlider->loadProgressBarTexture("Slider_PressBar.png");
    ySegmentsSlider->setPosition(Vec2(visibleSize.width / 2, (visibleSize.height / 2) - 250));
    ySegmentsSlider->setPercent(1);
    ySegmentsSlider->setMaxPercent(7);

    ySliderLabel = Label::createWithTTF(std::to_string(ySegmentsSlider->getPercent() + 3), "fonts/Marker Felt.ttf", 24);
    ySliderLabel->setPosition(Vec2(ySegmentsSlider->getPosition().x, ySegmentsSlider->getPosition().y + 25));
    this->addChild(ySliderLabel, 1);

    ySegmentsSlider->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
        {
            break;
        }
        case ui::Widget::TouchEventType::ENDED:
        {
            GameSettings::setSegmentsY(ySegmentsSlider->getPercent() + 3);
            ySliderLabel->setString(std::to_string(ySegmentsSlider->getPercent() + 3));
            break;
        }
        default: {}
        }
    });

    this->addChild(xSegmentsSlider);
    this->addChild(ySegmentsSlider);
}

void SettingsMenu::gotoMainMenu(cocos2d::Ref *sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(1, MainMenu::createScene()));
}