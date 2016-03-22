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
    initPresetImages();

    return true;
}


void ImageMenu::initMenu()
{
    MenuItemFont *back = MenuItemFont::create(
        "Back",
        CC_CALLBACK_1(ImageMenu::gotoSettingsMenu, this));

    cocos2d::Menu *menu = Menu::create(back, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    back->setPositionY(-250);

    this->addChild(menu, 1);
}

void ImageMenu::initLabels()
{
    cocos2d::Label *imageMenu = Label::createWithTTF("Image Menu", "fonts/Marker Felt.ttf", 32);
    imageMenu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
    this->addChild(imageMenu, 1);

    cocos2d::Label *clickTip = Label::createWithTTF("Click on an image to select it", "fonts/Marker Felt.ttf", 20);
    clickTip->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 180));
    this->addChild(clickTip, 1);
}

void ImageMenu::initPresetImages()
{
    for (int i = 0; i < 5; ++i)
    {
        Sprite *spr = nullptr;

        switch (i)
        {
            case 0:
            {
                spr = Sprite::create("puzzles/bridge.jpg");
                break;
            }
            case 1:
            {
                spr = Sprite::create("puzzles/divers.jpg");
                break;
            }
            case 2:
            {
                spr = Sprite::create("puzzles/dog.jpg");
                break;
            }
            case 3:
            {
                spr = Sprite::create("puzzles/game.jpg");
                break;
            }
            case 4:
            {
                spr = Sprite::create("puzzles/wheel.jpg");
                break;
            }
        }

        spr->setPosition(Vec2(275 + (i * 200) + (i * 2), (visibleSize.height / 2) + 100));
        spr->setScaleX(200 / spr->getContentSize().width);
        spr->setScaleY(150 / spr->getContentSize().height);
        spr->setOpacity(fadedOpacity);
        spr->setTag(i);

        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(ImageMenu::imageClick, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, spr);

        presetImages.push_back(spr);

        this->addChild(spr, 1);
    }

    presetImages[GameSettings::getImageID()]->setOpacity(selectedOpacity);
}

bool ImageMenu::imageClick(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Sprite *spr = static_cast<Sprite*>(event->getCurrentTarget());

    Point pt = touch->getLocation();
    Rect recTemp = spr->getBoundingBox();

    if (!recTemp.containsPoint(pt))
    {
        return false;
    }

    for (auto i : presetImages)
    {
        i->setOpacity(fadedOpacity);
    }

    spr->setOpacity(selectedOpacity);
    int imgID = spr->getTag();

    switch (imgID)
    {
        case 0:
        {
            GameSettings::setImageName("puzzles/bridge.jpg", imgID);
            break;
        }
        case 1:
        {
            GameSettings::setImageName("puzzles/divers.jpg", imgID);
            break;
        }
        case 2:
        {
            GameSettings::setImageName("puzzles/dog.jpg", imgID);
            break;
        }
        case 3:
        {
            GameSettings::setImageName("puzzles/game.jpg", imgID);
            break;
        }
        case 4:
        {
            GameSettings::setImageName("puzzles/wheel.jpg", imgID);
            break;
        }
    }

    return true;
}

void ImageMenu::gotoSettingsMenu(cocos2d::Ref* sender)
{
    Director::getInstance()->replaceScene(
        TransitionFade::create(0.5, SettingsMenu::createScene()));
}