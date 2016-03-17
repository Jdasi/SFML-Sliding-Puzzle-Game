#include "MySpriteAction.h"

MySpriteAction *MySpriteAction::create(
    cocos2d::Sprite *spr,
    cocos2d::SpriteFrame *fr)
{
    MySpriteAction *act = new MySpriteAction();

    if (act)
    {
        // set sprite for auto release and assign the variables passed through to the constructor
        act->autorelease();
        act->sprite = spr;
        act->frame = fr;
    }

    return act;
}

void MySpriteAction::update(float time)
{
    CC_UNUSED_PARAM(time);
    sprite->setSpriteFrame(frame);
}

MySpriteAction *MySpriteAction::clone() const
{
    // only needed if we clone the action i.e. create a copy, don't worry too much about this
    MySpriteAction *act = new MySpriteAction();
    act->autorelease();
    act->frame = this->frame;
    act->sprite = this->sprite;
    return act;
}