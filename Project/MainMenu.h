#pragma once

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(MainMenu);

private:
    void initProfile();
    void initSettings();
    void initBackdrop();
    void initStarDisplay();
    void initMenu();

    void gotoPuzzleSelection(cocos2d::Ref *sender);
    void gotoUnlocks(cocos2d::Ref *sender);
    void gotoStatistics(cocos2d::Ref *sender);
    void exitGame(cocos2d::Ref *pSender);

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};