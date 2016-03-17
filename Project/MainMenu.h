#pragma once

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void initSettings();
    void initMenu();

    void menuStartGame(cocos2d::Ref *sender);
    void menuCloseCallback(cocos2d::Ref *pSender);

    CREATE_FUNC(MainMenu);

private:

};