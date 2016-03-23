#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>

class ImageMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void initMenu();
    void initLabels();
    void initPresetImages();
    void initEditBox();

    void textFieldInteraction(cocos2d::Ref *sender, cocos2d::ui::TextField::EventType type);

    bool imageClick(cocos2d::Touch *touch, cocos2d::Event *event);
    void fadePresetImages();

    void gotoSettingsMenu(cocos2d::Ref *sender);

    CREATE_FUNC(ImageMenu);

private:
    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };

    int fadedOpacity = 100;
    int selectedOpacity = 255;

    std::vector<cocos2d::Sprite*> presetImages;
    cocos2d::ui::TextField *textField;
};