/****************************************************************************
 Copyright (c) 2017 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/


#include "AnimateClip.h"
#include "AnimationClip.h"
#include "AnimationClipProperties.h"
#include "Easing.h"
#include "Bezier.h"
#include "ui/CocosGUI.h"
#include "base/ccUtils.h"
#include <functional>

namespace  {

    creator::AnimationClip* g_clip = nullptr;

}

USING_NS_CCR;

AnimateClip* AnimateClip::createWithAnimationClip(cocos2d::Node* rootTarget, AnimationClip* clip)
{
    AnimateClip* animate = new (std::nothrow) AnimateClip;
    if (animate && animate->initWithAnimationClip(rootTarget, clip))
        animate->autorelease();
    else {
        delete animate;
        animate = nullptr;
    }

    return animate;
}

AnimateClip::AnimateClip()
: _clip(nullptr)
, _elapsed(0)
, _rootTarget(nullptr)
, _needStop(true)
, _durationToStop(0.f)
{
}

AnimateClip::~AnimateClip()
{
    // a loop animate might keep running until destruction, memory will leak if not stop it
    _endCallback = nullptr;
    if(_running)
        stopAnimate();

    CC_SAFE_RELEASE(_clip);
    CC_SAFE_RELEASE(_rootTarget);
}

void AnimateClip::startAnimate()
{
    _running = true;
    scheduleUpdate();
}

void AnimateClip::stopAnimate()
{
	if (_running)
	{
		unscheduleUpdate();
		// release self
		_running = false;

		if (_endCallback)
			_endCallback();
	}

}

void AnimateClip::pauseAnimate()
{
    unscheduleUpdate();
}

void AnimateClip::resumeAnimate()
{
    scheduleUpdate();
}

void AnimateClip::setCallbackForEndevent(const AnimateEndCallback &callback)
{
    _endCallback = std::move(callback);
}

bool AnimateClip::initWithAnimationClip(cocos2d::Node* rootTarget, AnimationClip* clip)
{
    _clip = clip;
    _rootTarget = rootTarget;
    CC_SAFE_RETAIN(_rootTarget);

    if (_clip)
    {
        _clip->retain();
        _durationToStop = _clip->getDuration();

        auto wrapMode = clip->getWrapMode();
        if (wrapMode == AnimationClip::WrapMode::Loop
            || wrapMode == AnimationClip::WrapMode::LoopReverse
            || wrapMode == AnimationClip::WrapMode::PingPong  // PingPong and PingPongReverse are loop animations
            || wrapMode == AnimationClip::WrapMode::PingPongReverse)
            _needStop = false;

        // assign it to be used in anonymous namespace
        g_clip = _clip;

		const auto& allAnimProperties = _clip->getAnimProperties();
		//for (const auto& animProperties : allAnimProperties)
		for(auto animProperties = allAnimProperties.begin(); animProperties!= allAnimProperties.end(); ++animProperties)
		{
			(*animProperties)->setTarget(getTarget((*animProperties)->path));
		}
    }

    return clip != nullptr;
}

void AnimateClip::update(float dt) {
    _elapsed += dt;

    if (_needStop && _elapsed*_clip->getSpeed() >= _durationToStop)
    {
        stopAnimate();

        return;
    }

	auto p = _rootTarget;

	bool isIn = false;
	while (p)
	{
		cocos2d::Scene* pScene = dynamic_cast<cocos2d::Scene*>(p);
		if (pScene)
		{
			isIn = true;
			break;
		}
		p = p->getParent();
	}
	if (!isIn)
	{
		stopAnimate();
		return;
	}

	//auto pre = cocos2d::utils::getTimeInMilliseconds();
    const auto& allAnimProperties = _clip->getAnimProperties();
	auto elapsed = computeElapse();
	for (auto it = allAnimProperties.begin(); it != allAnimProperties.end(); ++it)//const auto& animProperties : allAnimProperties)
	{
		auto target = (*it)->getTarget();
		if (target)
		{
			
			auto &aniMap = (*it)->m_sAnimMap;

			for (auto it = aniMap.begin(); it != aniMap.end(); ++it)
			{
				(*it)(target, elapsed);
			}
		}
	}


	
        //doUpdate(animProperties);

	//auto cur = cocos2d::utils::getTimeInMilliseconds();
	//CCLOG("useTime %lld %d,name %s", cur - pre, allAnimProperties.size(), _clip->getName().c_str());
}

void AnimateClip::doUpdate( AnimProperties* animProperties) const
{
	auto target = animProperties->getTarget();
	if (target && target->getParent())
	{
		auto elapsed = computeElapse();
		auto &aniMap = animProperties->m_sAnimMap;

		for (auto it = aniMap.begin(); it != aniMap.end(); ++it)
		{
			(*it)(target, elapsed);
		}


		/*  // update position
		  cocos2d::Vec2 nextPos;
		  if (getNextValue(animProperties.animPosition, elapsed, nextPos))
		  {
			  auto s =target->getParent()->getContentSize();
			  auto ap = target->getParent()->getAnchorPoint();
			  nextPos.x += s.width*ap.x;
			  nextPos.y += s.height*ap.y;
			  target->setPosition(nextPos);
		  }


		  // update color
		  cocos2d::Color3B nextColor;
		  if (getNextValue(animProperties.animColor, elapsed, nextColor))
			  target->setColor(nextColor);

		  // update scaleX
		  float nextValue;
		  if (getNextValue(animProperties.animScaleX, elapsed, nextValue))
			  target->setScaleX(nextValue);

		  // update scaleY
		  if (getNextValue(animProperties.animScaleY, elapsed, nextValue))
			  target->setScaleY(nextValue);

		  // rotation
		  if (getNextValue(animProperties.animRotation, elapsed, nextValue))
			  target->setRotation(nextValue);

		  // SkewX
		  if (getNextValue(animProperties.animSkewX, elapsed, nextValue))
			  target->setSkewX(nextValue);

		  // SkewY
		  if (getNextValue(animProperties.animSkewY, elapsed, nextValue))
			  target->setSkewY(nextValue);

		  // Opacity
		  if (getNextValue(animProperties.animOpacity, elapsed, nextValue))
			  target->setOpacity(nextValue);

		  // anchor x
		  if (getNextValue(animProperties.animAnchorX, elapsed, nextValue))
			  target->setAnchorPoint(cocos2d::Vec2(nextValue, target->getAnchorPoint().y));

		  // anchor y
		  if (getNextValue(animProperties.animAnchorY, elapsed, nextValue))
			  target->setAnchorPoint(cocos2d::Vec2(target->getAnchorPoint().x, nextValue));

		  // positoin x
		  if (getNextValue(animProperties.animPositionX, elapsed, nextValue))
		  {
			  auto s = target->getParent()->getContentSize();
			  auto ap = target->getParent()->getAnchorPoint();
			  nextValue += s.width*ap.x;

			  target->setPositionX(nextValue);
		  }

		  // position y
		  if (getNextValue(animProperties.animPositionY, elapsed, nextValue))
		  {
			  auto s = target->getParent()->getContentSize();
			  auto ap = target->getParent()->getAnchorPoint();
			  nextValue += s.height*ap.y;
			  target->setPositionY(nextValue);
		  }


		  // width
		  if (getNextValue(animProperties.animWidth, elapsed, nextValue))
		  {
			  auto size = target->getContentSize();
			  size.width = nextValue;
			  target->setContentSize(size);
		  }

		  // height
		  if (getNextValue(animProperties.animHeight, elapsed, nextValue))
		  {
			  auto size = target->getContentSize();
			  size.height = nextValue;
			  target->setContentSize(size);
		  }


		  std::string nextPath;
		  if (getNextValue(animProperties.animSpriteFrame, elapsed, nextPath))
		  {
			  cocos2d::ui::Button* pButton = dynamic_cast<cocos2d::ui::Button*>(target);

			  if (pButton)
			  {
				  auto frameCache = cocos2d::SpriteFrameCache::getInstance();
				  auto pSpriteFrame = frameCache->getSpriteFrameByName(nextPath);
				  if (pSpriteFrame)
				  {
					  pButton->getRendererNormal()->setSpriteFrame(pSpriteFrame);
				  }
				  else
				  {
					  pButton->getRendererNormal()->setTexture(nextPath);
				  }
			  }
			  else
			  {
				  cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(target);
				  if (pSprite)
				  {
					  auto frameCache = cocos2d::SpriteFrameCache::getInstance();
					  auto pSpriteFrame = frameCache->getSpriteFrameByName(nextPath);
					  if (pSpriteFrame)
					  {
						  pSprite->setSpriteFrame(pSpriteFrame);
					  }
					  else
					  {
						  pSprite->setTexture(nextPath);
					  }
				  }
			  }

		  }*/

    }
}

static cocos2d::Node * getNodeByName(cocos2d::Node * parent,const std::string &path)
{
	 auto index = path.find("/");
	 std::string sName;
	 std::string sSubffixPath;
	 if (index == std::string::npos)
	 {
		 sName = path;
	 }
	 else
	 {
		 sName = path.substr(0, index);
		 sSubffixPath = path.substr(index+1);
	 }

	if ( sSubffixPath.empty())
	{
		if (parent->getName() == sName)
		{
			return parent;
		}
		return nullptr;
	}
	if (parent->getName() == sName)
	{
		auto pChildren = parent->getChildren();
		for (auto it = pChildren.begin(); it != pChildren.end(); ++it)
		{

			auto node = getNodeByName(*it, sSubffixPath);
			if (node)
			{
				return node;
			}


		}
	}

	return nullptr;
}
cocos2d::Node* AnimateClip::getTarget(const std::string &path) const
{
    if (path.empty())
        return _rootTarget;


	auto pChildren = _rootTarget->getChildren();
	for (auto it = pChildren.begin(); it != pChildren.end(); ++it)
	{
		auto node = getNodeByName(*it, path);
		if (node)
		{
			return node;
		}
	}

    //cocos2d::Node *ret = getNodeByName(_rootTarget, path);

   /* _rootTarget->enumerateChildren(path, [&ret](cocos2d::Node* result) -> bool {
        ret = result;
        return true;
    });*/
	
	//_rootTarget

    return nullptr;
}

float AnimateClip::computeElapse() const
{
    auto elapsed = _elapsed* _clip->getSpeed();
    auto duration = _clip->getDuration();

    // as the time goes, _elapsed will be bigger than duration when _needStop = false
    elapsed = fmodf(elapsed, duration);

    const auto wrapMode = _clip->getWrapMode();
    bool oddRound = (static_cast<int>(_elapsed / duration) % 2) == 0;
    if (wrapMode == AnimationClip::WrapMode::Reverse  // reverse mode
        || (wrapMode == AnimationClip::WrapMode::PingPong && !oddRound) // pingpong mode and it is the second round
        || (wrapMode == AnimationClip::WrapMode::PingPongReverse && oddRound) // pingpongreverse mode and it is the first round
        || (wrapMode == AnimationClip::WrapMode::LoopReverse)  // loop reverse mode, reverse again and again
        )
        elapsed = duration - elapsed;

    return elapsed;
}

