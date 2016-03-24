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
    void initImageSelector();

    bool leftArrowClick(cocos2d::Ref *sender);
    bool rightArrowClick(cocos2d::Ref *sender);

    CREATE_FUNC(ImageMenu);

private:
    void updateDisplayedImage();
    void updateImageLabels();

    void gotoSettingsMenu(cocos2d::Ref *sender);

    cocos2d::Size visibleSize{ cocos2d::Director::getInstance()->getVisibleSize() };

    cocos2d::Sprite *displayedImage;
    cocos2d::Label *imageName;
    cocos2d::Label *imageNumber;
};