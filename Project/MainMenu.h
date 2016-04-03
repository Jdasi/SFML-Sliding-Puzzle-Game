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

    void gotoPuzzleSelection(cocos2d::Ref *sender) const;
    void gotoUnlocks(cocos2d::Ref *sender) const;
    void gotoStatistics(cocos2d::Ref *sender) const;
    void exitGame(cocos2d::Ref *pSender) const;

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};