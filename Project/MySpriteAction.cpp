#include "MySpriteAction.h"

MySpriteAction *MySpriteAction::create(
    cocos2d::Sprite *spr,
    cocos2d::SpriteFrame *fr)
{
    MySpriteAction *act = new MySpriteAction();

    if (act)
    {
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
    MySpriteAction *act = new MySpriteAction();
    act->autorelease();
    act->frame = this->frame;
    act->sprite = this->sprite;
    return act;
}