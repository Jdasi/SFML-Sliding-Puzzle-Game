#pragma once

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(MainMenu);

private:
    void initProfile() const;
    void initSettings() const;
    void initBackdrop();
    void initStarDisplay();
    void initMenu();

    void gotoPuzzleSelection(Ref* const sender) const;
    void gotoUnlocks(Ref* const sender) const;
    void gotoStatistics(Ref* const sender) const;
    void exitGame(Ref* const sender) const;

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};