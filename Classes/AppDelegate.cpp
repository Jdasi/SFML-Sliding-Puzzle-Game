#include "Project/MainMenu.h"
#include "Project/PuzzleScene.h"

#include "AppDelegate.h"
USING_NS_CC;

bool AppDelegate::applicationDidFinishLaunching() 
{
    cocos2d::Director *director = Director::getInstance();
    cocos2d::GLView *glview = director->getOpenGLView();

    if (!glview)
    {
        glview = GLViewImpl::create("Sliding Puzzle Game");
        glview->setFrameSize(1366, 768);
        director->setOpenGLView(glview);
    }

    auto scene = MainMenu::createScene();
    //auto scene = PuzzleScene::createScene();

    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() 
{

}

void AppDelegate::applicationWillEnterForeground() 
{

}