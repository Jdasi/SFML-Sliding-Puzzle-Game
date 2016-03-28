#pragma once

#include "cocos2d.h"

class ProfileUnlocks : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(ProfileUnlocks);

private:
    void initBackdrop();
    void initMenu();

    void gotoMainMenu(cocos2d::Ref *sender);

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};