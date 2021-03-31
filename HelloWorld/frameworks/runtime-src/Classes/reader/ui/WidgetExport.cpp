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
#include "WidgetExport.h"


NS_CCR_BEGIN

WidgetAdapter* WidgetAdapter::create()
{
    auto adapter = new (std::nothrow) WidgetAdapter;
    if (adapter && adapter->init()) {
        adapter->autorelease();
        return adapter;
    }
    return nullptr;
}

bool WidgetAdapter::init()
{
    //_layoutNode = cocos2d::ui::Layout::create();
   // _layoutNode->setLayoutType(cocos2d::ui::Layout::Type::RELATIVE);

    //CC_SAFE_RETAIN(_layoutNode);
    return true;
}

WidgetAdapter::WidgetAdapter()
: _isAlignOnce(true)
, _layoutTarget(nullptr)
, _needAdaptNode(nullptr)
, _sMargin(0,0,0,0,0,0)
{

}

WidgetAdapter::~WidgetAdapter()
{
   // CC_SAFE_RELEASE(_layoutNode);
	//CC_SAFE_RELEASE(_parameter);
	if (_layoutTarget)
	{
		_layoutTarget->release();
	}
}

void WidgetAdapter::setIsAlignOnce(bool isAlignOnce)
{
    _isAlignOnce = isAlignOnce;
}
void WidgetAdapter::setAdaptNode(cocos2d::Node* needAdaptNode)
{
    _needAdaptNode = needAdaptNode;
}

void WidgetAdapter::setLayoutTarget(cocos2d::Node* layoutTarget)
{
	if (_layoutTarget)
	{
		_layoutTarget:release();
	}

    _layoutTarget = layoutTarget;
	if (_layoutTarget)
	{
		_layoutTarget->retain();
	}
	
}

void WidgetAdapter::setMargin(const Margin &sMargin)
{
	_sMargin = sMargin;
}

void WidgetAdapter::setAlignComb(AlignComb align)
{
	 _alignComb = align;
}


float WidgetAdapter::getLeftX(float parentWidth, float nodeWidth, float anchorX, float left, bool isAbs )
{
	float x = left + nodeWidth*anchorX;
	if (!isAbs){
		x = parentWidth*left + nodeWidth*anchorX;
	}
	
	return x;
}

float WidgetAdapter::getRightX(float parentWidth,float nodeWidth, float anchorX, float right, bool isAbs)
{
	float x = parentWidth - (right + nodeWidth*(1.0 - anchorX));
	if (!isAbs) {
		x = parentWidth - (right*parentWidth + nodeWidth*(1.0 - anchorX));
	}
	return x;
}

float  WidgetAdapter::getTopY(float parentHeight, float nodeHeight, float anchorY, float top, bool isAbs)
{
	float y = parentHeight - (top + nodeHeight*(1.0 - anchorY));

	if (!isAbs) {
		y = parentHeight - (top*parentHeight + nodeHeight*(1.0 - anchorY));
	}
	return y;
}

float  WidgetAdapter::getBottomY(float parentHeight, float nodeHeight, float anchorY, float bottom, bool isAbs)
{
	float y = bottom + nodeHeight*anchorY;

	if (!isAbs) {
		y = parentHeight*bottom + nodeHeight*anchorY;  
	}

	return y;
}


float WidgetAdapter::getCenterX(float parentWidth, float nodeWidth, float anchorX, float offset, bool isAbs)
{
	float x = parentWidth*0.5 + ((anchorX - 0.5)*nodeWidth) + offset;

	if (!isAbs) {
		x = parentWidth*0.5 + ((anchorX - 0.5)*nodeWidth) + parentWidth*offset;
	}


	return x;
}

float WidgetAdapter::getCenterY(float parentHeight, float nodeHeight, float anchorY, float offset, bool isAbs)
{
	float y = parentHeight*0.5 + ((anchorY - 0.5)*nodeHeight)+ offset;

	if (!isAbs) {
		y = parentHeight*0.5 + ((anchorY - 0.5)*nodeHeight) + parentHeight*offset;
	}

	return y;
}


void WidgetAdapter::syncLayoutProperty()
{
	auto sDesignSize = cocos2d::Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	auto adaptSize   = _needAdaptNode->getContentSize();
	auto anchorPoint = _needAdaptNode->getAnchorPoint();
	auto adaptScaleX = _needAdaptNode->getScaleX();
	auto adaptScaleY = _needAdaptNode->getScaleY();
	auto targetSize  = _layoutTarget->getContentSize();

	adaptSize.width = adaptScaleX*adaptSize.width;
	adaptSize.height = adaptScaleY*adaptSize.height;


	if (_needAdaptNode->getName().compare("root") == 0){
		auto rect = WidgetManager::getSafeUIRect();
		if (rect.size.width > 1)
		{
			sDesignSize = rect.size;
		}
		float x = rect.origin.x + sDesignSize.width*0.5;
		float y = rect.origin.y + sDesignSize.height*0.5;
		_needAdaptNode->setContentSize(sDesignSize);
		_needAdaptNode->setPosition(x,y);
		return;
	}

	if (_needAdaptNode->getName().find("__bg") != std::string::npos ) {
		_needAdaptNode->setContentSize(sDesignSize);
		_needAdaptNode->setPosition(sDesignSize/2);
		return;
	}

	float x = 0.0f, y = 0.0f;
	
	const auto& margin = _sMargin;
	auto alignComb = _alignComb;
	

	switch (alignComb) {
	case AlignComb::TOP:
		x = _needAdaptNode->getPositionX();
		y = getTopY(targetSize.height, adaptSize.height, anchorPoint.y, margin.top, margin._isAbsTop);

		break;
	case AlignComb::LEFT:
		x = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);
		y = _needAdaptNode->getPositionY();
		break;
	case AlignComb::RIGHT:

		x = getRightX(targetSize.width, adaptSize.width, anchorPoint.x, margin.right, margin._isAbsRight);
		y = _needAdaptNode->getPositionY();
		break;
	case AlignComb::BOTTOM:
		 x = _needAdaptNode->getPositionX();
		 y = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom,  margin._isAbsBottom);

		break;
	case AlignComb::CENTER_VERTICAL:
		x = _needAdaptNode->getPositionX();
		y = getCenterY(targetSize.height, adaptSize.height, anchorPoint.y, margin.vcenter, margin._isAbsVerticalCenter);

		break;
	case AlignComb::CENTER_HORIZONTAL:
		x = getCenterX(targetSize.width, adaptSize.width, anchorPoint.x, margin.hcenter, margin._isAbsHorizontalCenter);
		y = _needAdaptNode->getPositionY();

		break;
	case AlignComb::TOP_LEFT:
		x = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);//margin.left + adaptSize.width*anchorPoint.x;
		y = getTopY(targetSize.height, adaptSize.height, anchorPoint.y, margin.top, margin._isAbsTop);//targetSize.height - (margin.top + adaptSize.height*(1.0 - anchorPoint.y));
		break;
	case AlignComb::TOP_RIGHT:
		x = getRightX(targetSize.width, adaptSize.width, anchorPoint.x, margin.right, margin._isAbsRight);//targetSize.width - (margin.right + adaptSize.width*(1.0 - anchorPoint.x));
		y = getTopY(targetSize.height, adaptSize.height, anchorPoint.y, margin.top, margin._isAbsTop); //targetSize.height - (margin.top + adaptSize.height*(1.0 - anchorPoint.y));
		break;
	case AlignComb::RIGHT_BOTTOM:
		x = getRightX(targetSize.width, adaptSize.width, anchorPoint.x, margin.right, margin._isAbsRight); //targetSize.width - (margin.right + adaptSize.width*(1.0 - anchorPoint.x));
		y = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom);//margin.bottom + adaptSize.height*anchorPoint.y;
		break;
	case AlignComb::LEFT_BOTTOM:
		x = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft); //margin.left + adaptSize.width*anchorPoint.x;
		y = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom);// margin.bottom + adaptSize.height*anchorPoint.y;
		break;
	case AlignComb::LEFT_RIGHT:

	{
		float lx = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);
		float rx = getRightX(targetSize.width, adaptSize.width, anchorPoint.x, margin.right, margin._isAbsRight);
	
		float width = rx - lx + adaptSize.width;
		adaptSize.width = width < 0 ? adaptSize.width : width;

		_needAdaptNode->setContentSize(adaptSize);
		x = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);

		y = _needAdaptNode->getPositionY();

	}

		break;
		
	case AlignComb::LEFT_CENTER_VERTICAL:
		x = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);//margin.left + adaptSize.width*anchorPoint.x;
		y = getCenterY(targetSize.height, adaptSize.height, anchorPoint.y, margin.vcenter, margin._isAbsVerticalCenter);// targetSize.height*0.5 + ((anchorPoint.y - 0.5)*adaptSize.height);
		break;
	case AlignComb::RIGHT_CENTER_VERTICAL:
		x = getRightX(targetSize.width, adaptSize.width, anchorPoint.x, margin.right, margin._isAbsRight); //targetSize.width - (margin.right + adaptSize.width*(1.0 - anchorPoint.x));
		y = getCenterY(targetSize.height, adaptSize.height, anchorPoint.y, margin.vcenter, margin._isAbsVerticalCenter);//targetSize.height*0.5 + ((anchorPoint.y - 0.5)*adaptSize.height);
		break;
	case AlignComb::TOP_CENTER_HORIZONTAL:

		x = getCenterX(targetSize.width, adaptSize.width, anchorPoint.x, margin.hcenter, margin._isAbsHorizontalCenter);// targetSize.width*0.5 + ((anchorPoint.x - 0.5)*adaptSize.width);
		y = getTopY(targetSize.height, adaptSize.height, anchorPoint.y, margin.top, margin._isAbsTop);//targetSize.height - (margin.top + adaptSize.height*(1.0 - anchorPoint.y));
		break;
	case AlignComb::BOTTOM_CENTER_HORIZONTAL:
		x = getCenterX(targetSize.width, adaptSize.width, anchorPoint.x, margin.hcenter,margin._isAbsHorizontalCenter);
		y = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom); //margin.bottom + adaptSize.height*anchorPoint.y;
		break;
	case AlignComb::CENTER_IN_PARENT:
		x = getCenterX(targetSize.width, adaptSize.width, anchorPoint.x, margin.hcenter, margin._isAbsHorizontalCenter);
		y = getCenterY(targetSize.height, adaptSize.height, anchorPoint.y, margin.vcenter, margin._isAbsVerticalCenter);
		break;
	case AlignComb::LEFT_TOP_BOTTOM_RIGHT:


	{
		float lx = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left,  margin._isAbsLeft);
		float rx = getRightX(targetSize.width,adaptSize.width, anchorPoint.x, margin.right,  margin._isAbsRight);
		float ty = getTopY(targetSize.height, adaptSize.height, anchorPoint.y, margin.top,  margin._isAbsTop);
		float by = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom);

		float width = rx - lx + adaptSize.width;
		float height = ty - by+ adaptSize.height;

		adaptSize.width = width < 0 ? adaptSize.width : width;
		adaptSize.height = height < 0 ? adaptSize.height : height;

		_needAdaptNode->setContentSize(adaptSize);
		x = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);;
		y = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom);

	}

		break;
	case AlignComb::LEFT_TOP_RIGHT:
	{
		float lx = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);
		float rx = getRightX(targetSize.width, adaptSize.width, anchorPoint.x, margin.right, margin._isAbsRight);
		float ty = getTopY(targetSize.height, adaptSize.height, anchorPoint.y, margin.top, margin._isAbsTop);


		float width = rx - lx + adaptSize.width;
		adaptSize.width = width < 0 ? adaptSize.width : width;

		_needAdaptNode->setContentSize(adaptSize);
		x = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);
		y = ty;

	}
		
		break;
	case AlignComb::LEFT_BOTTOM_RIGHT:
	{
		float lx = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);
		float rx = getRightX(targetSize.width, adaptSize.width, anchorPoint.x, margin.right, margin._isAbsRight);
		
		float by = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom);

		float width = rx - lx + adaptSize.width;
		

		adaptSize.width = width < 0 ? adaptSize.width : width;
		

		_needAdaptNode->setContentSize(adaptSize);
		x = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);
		y = by;

	}
		break;

	case WidgetAdapter::AlignComb::LEFT_BOTTOM_TOP:

	{
		float lx = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left,  margin._isAbsLeft);
		float ty = getTopY(targetSize.height, adaptSize.height, anchorPoint.y, margin.top, margin._isAbsTop);
		float by = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom);

		float height = ty - by + adaptSize.height;
		adaptSize.height = height < 0 ? adaptSize.height : height;
		_needAdaptNode->setContentSize(adaptSize);

		x = lx;
		y = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom);

	}

		break;
	case WidgetAdapter::AlignComb::RIGHT_BOTTOM_TOP:
	{
		//float lx = getLeftX(adaptSize.width, anchorPoint.x, margin.left);
		float rx = getRightX(targetSize.width, adaptSize.width, anchorPoint.x, margin.right, margin._isAbsRight);
		float ty = getTopY(targetSize.height, adaptSize.height, anchorPoint.y, margin.top, margin._isAbsTop);
		float by = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom,  margin._isAbsBottom);

		float height = ty - by + adaptSize.height;
		adaptSize.height = height < 0 ? adaptSize.height : height;
		_needAdaptNode->setContentSize(adaptSize);

		x = rx;
		y = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom);

	}
		break;

	case WidgetAdapter::AlignComb::LEFT_RIGHT_CENTER_VERTICAL:
	{
		float lx = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);
		float rx = getRightX(targetSize.width, adaptSize.width, anchorPoint.x, margin.right, margin._isAbsRight);
		float cy = getCenterY(targetSize.height, adaptSize.height, anchorPoint.y, margin.vcenter, margin._isAbsVerticalCenter);
		

		float width = rx - lx + adaptSize.width;
		adaptSize.width = width < 0 ? adaptSize.width : width;
		_needAdaptNode->setContentSize(adaptSize);

		x = getLeftX(targetSize.width, adaptSize.width, anchorPoint.x, margin.left, margin._isAbsLeft);
		y = cy;

	}
	break;

	case WidgetAdapter::AlignComb::BOTTOM_TOP_CENTER_HORIZONTAL:
	{
		//float lx = getLeftX(adaptSize.width, anchorPoint.x, margin.left);
		float cx = getCenterX(targetSize.width, adaptSize.width, anchorPoint.x, margin.hcenter, margin._isAbsHorizontalCenter);
		float ty = getTopY(targetSize.height, adaptSize.height, anchorPoint.y, margin.top, margin._isAbsTop);
		float by = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom);

		float height = ty - by + adaptSize.height;
		adaptSize.height = height < 0 ? adaptSize.height : height;
		_needAdaptNode->setContentSize(adaptSize);

		x = cx;
		y = getBottomY(targetSize.height, adaptSize.height, anchorPoint.y, margin.bottom, margin._isAbsBottom);

	}
	break;

	default:

		break;
	}

	_needAdaptNode->setPosition(x, y);
}

void WidgetAdapter::setupLayout()
{
    auto parent = _needAdaptNode->getParent();
    CCASSERT(parent != nullptr, "adaptNode's parent can't be null");

    // set default layout target to parent node
    if (_layoutTarget == nullptr) {
        //_layoutTarget = parent;
		setLayoutTarget(parent);
    }
   // _needAdaptNode->removeFromParentAndCleanup(false);
   // _layoutNode->setName(PLUGIN_EXTRA_LAYOUT_NAME);
   // _layoutNode->addChild(_needAdaptNode);
   // parent->addChild(_layoutNode);
}


cocos2d::Node* WidgetAdapter::getAdaptNode() {
	return _needAdaptNode;
}

cocos2d::Rect WidgetManager::m_sSafeUIRect = cocos2d::Rect();

WidgetManager::WidgetManager()
: _forceAlignDirty(false)
{

}

WidgetManager::~WidgetManager()
{
}


void WidgetManager::cleanup()
{
	cocos2d::Node::cleanup();
	_needAdaptWidgets.clear();
}

void WidgetManager::addWidget(WidgetAdapter* pWidget)
{
	_needAdaptWidgets.pushBack(pWidget);
}

void  WidgetManager::removeWidgetByNode(cocos2d::Node* pNode)
{
	if (!pNode)
	{
		return;
	}

	int size = _needAdaptWidgets.size();
	for (int i = 0; i < size; ++i)
	{
		auto pWidget = _needAdaptWidgets.at(i);
		if (pWidget->getAdaptNode() == pNode)
		{
			pWidget->setAdaptNode(nullptr);
			_needAdaptWidgets.erase(i);
			break;
		}
		
	}
}


void WidgetManager::update(float dt)
{
    doAlign();
}

void WidgetManager::forceDoAlign()
{
    _forceAlignDirty = true;
    doAlign();
}

void WidgetManager::setSafeUIRect(cocos2d::Rect &sRect)
{
	m_sSafeUIRect = sRect;
}


void WidgetManager::forceDoAlignAssignNode(cocos2d::Node* assignNode)
{
	if (assignNode)
	{
		for (auto& adapter : _needAdaptWidgets) {
			if (adapter->getAdaptNode() == assignNode)
			{
				adapter->syncLayoutProperty();
				break;
			}
		}
	}
}

void WidgetManager::doAlign()
{
	if (_forceAlignDirty)
	{
		for (auto& adapter : _needAdaptWidgets) {
			if (_forceAlignDirty || !(adapter->_isAlignOnce))
			{
				adapter->syncLayoutProperty();
			}
		}
	}
    
    _forceAlignDirty = false;
}

void WidgetManager::setupWidgets()
{
    for (auto& adapter:_needAdaptWidgets) {
        adapter->setupLayout();
        adapter->syncLayoutProperty();
    }
    scheduleUpdate();
}
NS_CCR_END
