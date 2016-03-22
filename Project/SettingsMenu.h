#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"


class SettingsMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void initMenu();

    void gotoImageMenu(cocos2d::Ref *sender);
    void gotoMainMenu(cocos2d::Ref *sender);

    CREATE_FUNC(SettingsMenu);

private:
    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };

    cocos2d::Label *xSliderLabel;
    cocos2d::Label *ySliderLabel;

    cocos2d::ui::Slider *xSegmentsSlider;
    cocos2d::ui::Slider *ySegmentsSlider;
};