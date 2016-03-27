#include "Project/MainMenu.h"

#include "AppDelegate.h"
USING_NS_CC;

bool AppDelegate::applicationDidFinishLaunching() 
{
    srand(static_cast<int>(time(nullptr)));

    cocos2d::Director *director = Director::getInstance();
    cocos2d::GLView *glview = director->getOpenGLView();

    if (!glview)
    {
        glview = GLViewImpl::create("Square Slide");
        glview->setFrameSize(1366, 768);
        director->setOpenGLView(glview);
    }

    auto scene = MainMenu::createScene();

    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() 
{

}

void AppDelegate::applicationWillEnterForeground() 
{

}