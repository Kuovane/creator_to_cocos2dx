#include "AnimateClip.h"
#include "AnimationClip.h"
#include "AnimationClipProperties.h"
#include "Easing.h"
#include "Bezier.h"
#include "ui/CocosGUI.h"

#include <functional>

namespace CreatorAniUtils{

	// -1: invalid index
	// -2: haven't reached first frame, so it should be the same as first frame
	template<typename P>
	int getValidIndex(const P &properties, float elapsed)
	{
		//if (properties.empty())
		//	return -1;

		if (properties.front().frame > elapsed)
			return -2;

		if (properties.back().frame <= elapsed)
			return properties.size() - 1;

		for (int i = 0, len = properties.size(); i < len; ++i)
		{
			const auto& prop = properties[i];
			if (prop.frame > elapsed)
				return i - 1;
		}

		return -1;
	}

	template<typename P>
	float getPercent(const P& p1, const P& p2, float elapsed)
	{
		const auto& curveType = p1.curveType;
		const auto& curveData = p1.curveData;
		auto ratio = (elapsed - p1.frame) / (p2.frame - p1.frame);

		if (!curveType.empty())
		{
			const auto& easingFunc = creator::Easing::getFunction(curveType);
			ratio = easingFunc(ratio);
		}
		if (curveData.size() > 0)
			ratio = creator::Bazier::computeBezier(curveData, ratio);

		return ratio;
	}

	void assignValue(float src, float& dst)
	{
		dst = src;
	}

	void assignValue(std::string src, std::string & dst)
	{
		dst = src;
	}

	void assignValue(const cocos2d::Color3B& src, cocos2d::Color3B& dst)
	{
		dst.r = src.r;
		dst.g = src.g;
		dst.b = src.b;
	}

	void assignValue(const cocos2d::Vec2& src, cocos2d::Vec2& dst)
	{
		dst.x = src.x;
		dst.y = src.y;
	}

	template<typename T>
	void computeNextValue(T start, T end, float percent, T &out)
	{
		out = start + percent * (end - start);
	}

	void computeNextValue(std::string start, std::string end, float percent, std::string &out)
	{
		out = start;
	}

	void computeNextValue(const cocos2d::Color3B& start, const cocos2d::Color3B& end, float percent, cocos2d::Color3B& out)
	{
		computeNextValue(start.r, end.r, percent, out.r);
		computeNextValue(start.g, end.g, percent, out.g);
		computeNextValue(start.b, end.b, percent, out.b);
	}

	void computeNextValue(const cocos2d::Vec2& start, const cocos2d::Vec2& end, float percent, cocos2d::Vec2& out)
	{
		computeNextValue(start.x, end.x, percent, out.x);
		computeNextValue(start.y, end.y, percent, out.y);
	}

	template<typename P, typename T>
	bool getNextValue(const P & properties, float elapsed, T &out)
	{
		int index = getValidIndex(properties, elapsed);
		if (index == -1)
			return false;

		if (index == -2)
		{
			assignValue(properties.front().value, out);
			return true;
		}

		if (index == properties.size() - 1)
		{
			assignValue(properties.back().value, out);
			return true;
		}

		const auto& prop = properties[index];
		const auto& nextProp = properties[index + 1];
		float percent = getPercent(prop, nextProp, elapsed);
		computeNextValue(prop.value, nextProp.value, percent, out);

		return true;
	}
}

USING_NS_CCR;


void AnimProperties::clearAnimMap()
{
	m_sAnimMap.clear();
}

void AnimProperties::updateAnimMap()
{
	clearAnimMap();
	if (animRotation.size() > 0)
	{
		m_sAnimMap.push_back( std::bind(&AnimProperties::updateAnimRotation, this, std::placeholders::_1, std::placeholders::_2));
	}
	if (animPosition.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimPosition, this, std::placeholders::_1, std::placeholders::_2));
	}
	if (animPositionX.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimPositionX, this, std::placeholders::_1, std::placeholders::_2));
	}
	if (animPositionY.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimPositionY, this, std::placeholders::_1, std::placeholders::_2));
	}
	if (animScaleX.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimScaleX, this, std::placeholders::_1, std::placeholders::_2));
	}
	if (animScaleY.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimScaleY, this, std::placeholders::_1, std::placeholders::_2));
	}
	if (animSkewX.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimSkewX, this, std::placeholders::_1, std::placeholders::_2));
	}

	if (animSkewY.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimSkewY, this, std::placeholders::_1, std::placeholders::_2));
	}
	if (animAnchorX.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimAnchorX, this, std::placeholders::_1, std::placeholders::_2));
	}
	if (animAnchorY.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimAnchorY, this, std::placeholders::_1, std::placeholders::_2));
	}
	if (animColor.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimColor, this, std::placeholders::_1, std::placeholders::_2));
	}

	if (animOpacity.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimOpacity, this, std::placeholders::_1, std::placeholders::_2));
	}

	if (animWidth.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimWidth, this, std::placeholders::_1, std::placeholders::_2));
	}

	if (animHeight.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimHeight, this, std::placeholders::_1, std::placeholders::_2));
	}

	if (animSpriteFrame.size() > 0)
	{
		m_sAnimMap.push_back(std::bind(&AnimProperties::updateAnimSpriteFrame, this, std::placeholders::_1, std::placeholders::_2));
	}
}

void AnimProperties::updateAnimRotation(cocos2d::Node* target, float elapsed)
{
	// rotation
	if (CreatorAniUtils::getNextValue(animRotation, elapsed, nextValue))
		target->setRotation(nextValue);
}

void AnimProperties::updateAnimPosition(cocos2d::Node* target, float elapsed)
{
	
	if (CreatorAniUtils::getNextValue(animPosition, elapsed, nextPos))
	{
		auto s = target->getParent()->getContentSize();
		auto ap = target->getParent()->getAnchorPoint();
		nextPos.x += s.width*ap.x;
		nextPos.y += s.height*ap.y;
		target->setPosition(nextPos);
	}
}


void AnimProperties::updateAnimPositionX(cocos2d::Node* target, float elapsed)
{
	// positoin x
	if (CreatorAniUtils::getNextValue(animPositionX, elapsed, nextValue))
	{
		auto s = target->getParent()->getContentSize();
		auto ap = target->getParent()->getAnchorPoint();
		nextValue += s.width*ap.x;

		target->setPositionX(nextValue);
	}
}

void AnimProperties::updateAnimPositionY(cocos2d::Node* target, float elapsed)
{
	// position y
	if (CreatorAniUtils::getNextValue(animPositionY, elapsed, nextValue))
	{
		auto s = target->getParent()->getContentSize();
		auto ap = target->getParent()->getAnchorPoint();
		nextValue += s.height*ap.y;
		target->setPositionY(nextValue);
	}
}

void AnimProperties::updateAnimScaleX(cocos2d::Node* target, float elapsed)
{
	// update scaleX
	
	if (CreatorAniUtils::getNextValue(animScaleX, elapsed, nextValue))
		target->setScaleX(nextValue);
}

void AnimProperties::updateAnimScaleY(cocos2d::Node* target, float elapsed)
{
	// update scaleY
	if (CreatorAniUtils::getNextValue(animScaleY, elapsed, nextValue))
		target->setScaleY(nextValue);
}

void AnimProperties::updateAnimSkewX(cocos2d::Node* target, float elapsed)
{
	// SkewX
	if (CreatorAniUtils::getNextValue(animSkewX, elapsed, nextValue))
		target->setSkewX(nextValue);
}

void AnimProperties::updateAnimSkewY(cocos2d::Node* target, float elapsed)
{
	// SkewY
	if (CreatorAniUtils::getNextValue(animSkewY, elapsed, nextValue))
		target->setSkewY(nextValue);
}

void AnimProperties::updateAnimAnchorX(cocos2d::Node* target, float elapsed)
{
	// anchor x
	if (CreatorAniUtils::getNextValue(animAnchorX, elapsed, nextValue))
		target->setAnchorPoint(cocos2d::Vec2(nextValue, target->getAnchorPoint().y));
}

void AnimProperties::updateAnimAnchorY(cocos2d::Node* target, float elapsed)
{
	// anchor y
	if (CreatorAniUtils::getNextValue(animAnchorY, elapsed, nextValue))
		target->setAnchorPoint(cocos2d::Vec2(target->getAnchorPoint().x, nextValue));
}

void AnimProperties::updateAnimColor(cocos2d::Node* target, float elapsed)
{
	// update color
	
	if (CreatorAniUtils::getNextValue(animColor, elapsed, nextColor))
		target->setColor(nextColor);
}

void AnimProperties::updateAnimOpacity(cocos2d::Node* target, float elapsed)
{
	// Opacity
	if (CreatorAniUtils::getNextValue(animOpacity, elapsed, nextValue))
		target->setOpacity(nextValue);
}

void AnimProperties::updateAnimWidth(cocos2d::Node* target, float elapsed)
{
	// width 
	if (CreatorAniUtils::getNextValue(animWidth, elapsed, nextValue))
	{
		auto size = target->getContentSize();
		size.width = nextValue;
		target->setContentSize(size);
	}
}

void AnimProperties::updateAnimHeight(cocos2d::Node* target, float elapsed)
{
	// height 
	if (CreatorAniUtils::getNextValue(animHeight, elapsed, nextValue))
	{
		auto size = target->getContentSize();
		size.height = nextValue;
		target->setContentSize(size);
	}

}

void AnimProperties::updateAnimSpriteFrame(cocos2d::Node* target, float elapsed)
{

	if (CreatorAniUtils::getNextValue(animSpriteFrame, elapsed, nextPath))
	{
		cocos2d::ui::Button* pButton = dynamic_cast<cocos2d::ui::Button*>(target);

		if (pButton)
		{
			auto frameCache = cocos2d::SpriteFrameCache::getInstance();
			auto pSpriteFrame = frameCache->getSpriteFrameByName(nextPath);
			auto pRenderNormal = pButton->getRendererNormal();
			auto blendFunc = pRenderNormal->getBlendFunc();
			if (pSpriteFrame)
			{
				pRenderNormal->setSpriteFrame(pSpriteFrame);
			}
			else
			{
				pRenderNormal->setTexture(nextPath);
			}
			pRenderNormal->setBlendFunc(blendFunc);
		}
		else
		{
			cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(target);
			if (pSprite)
			{
				auto frameCache = cocos2d::SpriteFrameCache::getInstance();
				auto pSpriteFrame = frameCache->getSpriteFrameByName(nextPath);
				auto blend = pSprite->getBlendFunc();
				if (pSpriteFrame)
				{
					if(pSprite->getSpriteFrame() != pSpriteFrame)
						pSprite->setSpriteFrame(pSpriteFrame);
				}
				else
				{
					pSprite->setTexture(nextPath);
				}
				pSprite->setBlendFunc(blend);
			}
		}

	}
}
