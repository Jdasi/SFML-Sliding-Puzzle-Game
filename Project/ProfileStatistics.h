#pragma once

#include "cocos2d.h"

class ProfileStatistics : public cocos2d::Layer
{
public:
    ProfileStatistics() = default;
    ~ProfileStatistics() = default;

    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(ProfileStatistics);

private:
    void initBackdrop();
    void initMenu();
    void initMenuPane();
    void initStaticLabels();
    void initDynamicLabels();

    void gotoMainMenu(Ref* const sender) const;

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };
};