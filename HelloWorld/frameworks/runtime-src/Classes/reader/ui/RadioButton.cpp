#include "RadioButton.h"
#include "RadioButtonGroup.h"

NS_CCR_BEGIN



CreatorRadioButton* CreatorRadioButton::create()
{
	CreatorRadioButton* widget = new (std::nothrow) CreatorRadioButton();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

CreatorRadioButton* CreatorRadioButton::create(const std::string& backGround,
	const std::string& backGroundSelected,
	const std::string& cross,
	const std::string& backGroundDisabled,
	const std::string& frontCrossDisabled,
	TextureResType texType)
{
	CreatorRadioButton *pWidget = new (std::nothrow) CreatorRadioButton;
	if (pWidget && pWidget->init(backGround,
		backGroundSelected,
		cross,
		backGroundDisabled,
		frontCrossDisabled,
		texType))
	{
		pWidget->autorelease();
		return pWidget;
	}
	CC_SAFE_DELETE(pWidget);
	return nullptr;
}

CreatorRadioButton* CreatorRadioButton::create(const std::string& backGround,
	const std::string& cross,
	TextureResType texType)
{
	CreatorRadioButton *pWidget = new (std::nothrow) CreatorRadioButton;
	if (pWidget && pWidget->init(backGround,
		"",
		cross,
		"",
		"",
		texType))
	{
		pWidget->autorelease();
		return pWidget;
	}
	CC_SAFE_DELETE(pWidget);
	return nullptr;
}


CreatorRadioButton::CreatorRadioButton()
{
	_zoomScale = 1.0;

	_actionDuration = 0.1;
	_transitionType = 0;
	_enableAutoGrayEffect = false;
	_isToggleGroup = false;

}

CreatorRadioButton::~CreatorRadioButton()
{
}


void CreatorRadioButton::onTouchEnded(Touch *touch, Event *unusedEvent)
{

	if (_isToggleGroup)
	{
		RadioButton::onTouchEnded(touch, unusedEvent);
	}
	else
	{
		bool highlight = _highlight;

		AbstractCheckButton::onTouchEnded(touch, unusedEvent);

		if (highlight)
		{
			if (_isSelected)
			{
				setSelected(false);
				dispatchSelectChangedEvent(false);
			}
			else
			{
				setSelected(true);
				dispatchSelectChangedEvent(true);
			}
		}
	}
	
}


void CreatorRadioButton::releaseUpEvent()
{
	if (_isToggleGroup)
	{
		Widget::releaseUpEvent();

		if (!_isSelected)
		{
			setSelected(true);
			dispatchSelectChangedEvent(true);
		}
		else
		{
			if (_group && _group->isAllowedNoSelection())
			{
				_group->setSelectedButtonWithoutEvent(nullptr);

				auto g = dynamic_cast<CreatorRadioButtonGroup*>( _group);
				if (g)
				{
					 g->callGroupCallback(this, 0);
	
				}
			}
			//isAllowedNoSelection
			
		}
	}
	else
	{
		//checkbox
		Widget::releaseUpEvent();
	}
}






void CreatorRadioButton::dispatchSelectChangedEvent(bool selected)
{
	if (_isToggleGroup)
	{
		RadioButton::dispatchSelectChangedEvent(selected);
	}
	else
	{
		EventType eventType = (selected ? EventType::SELECTED : EventType::UNSELECTED);

		this->retain();
		if (_radioButtonEventCallback)
		{
			_radioButtonEventCallback(this, eventType);
		}
		if (_ccEventCallback)
		{
			_ccEventCallback(this, static_cast<int>(eventType));
		}

		this->release();
	}
	
}

void CreatorRadioButton::addChild(Node *child)
{
	
	std::string sChildName = child->getName();
	if (sChildName == getNodeCheckmarkName())
	{
		auto spirte = dynamic_cast<Sprite*>(child);
		if (spirte)
		{
			setCheckMarkNode(spirte);
			ui::RadioButton::addChild(spirte);
			spirte->setLocalZOrder(1);
			return;
		}
	}

	if (sChildName == getNodeBgName())
	{
		auto spirte = dynamic_cast<Sprite*>(child);
		if (spirte)
		{
			setBgNode(spirte);
			addProtectedChild(spirte, -1, -1);
			return;
		}
	}

	ui::RadioButton::addChild(child);
}

void CreatorRadioButton::setCheckMarkNode(cocos2d::Sprite * pNode)
{

	removeProtectedChild(_frontCrossRenderer);
	removeChild(_frontCrossRenderer);

	_frontCrossRenderer = pNode;

	//_backGroundBoxRenderer->REMO
	//_backGroundSelectedBoxRenderer = Sprite::create();
	//_frontCrossRenderer = Sprite::create();
	//_backGroundBoxDisabledRenderer = Sprite::create();
	//_frontCrossDisabledRenderer = Sprite::create();
}

void CreatorRadioButton::setBgNode(cocos2d::Sprite * pNode)
{
	removeProtectedChild(_backGroundBoxRenderer);
	removeChild(_backGroundBoxRenderer);

	_backGroundBoxRenderer = pNode;
}

void CreatorRadioButton::setNodeBgName(std::string sBgName) {
	_nodeBgName = sBgName;
};

void CreatorRadioButton::setNodeCheckmarkName(std::string sName) {
	_nodeCheckmarkName = sName;
};


void CreatorRadioButton::backGroundTextureScaleChangedWithSize()
{

}

void CreatorRadioButton::backGroundSelectedTextureScaleChangedWithSize()
{

}

void CreatorRadioButton::frontCrossTextureScaleChangedWithSize()
{

}

void CreatorRadioButton::backGroundDisabledTextureScaleChangedWithSize()
{

}

void CreatorRadioButton::frontCrossDisabledTextureScaleChangedWithSize()
{

}


void CreatorRadioButton::onPressStateChangedToNormal()
{
	_backGroundBoxRenderer->setVisible(true);
	_backGroundSelectedBoxRenderer->setVisible(false);
	_backGroundBoxDisabledRenderer->setVisible(false);
	_frontCrossDisabledRenderer->setVisible(false);

	_backGroundBoxRenderer->setGLProgramState(this->getNormalGLProgramState(_backGroundBoxRenderer->getTexture()));
	_frontCrossRenderer->setGLProgramState(this->getNormalGLProgramState(_frontCrossRenderer->getTexture()));

	//action
	if (_transitionType == TRANSITION_SPRITE)
	{

	}
	else if (_transitionType == TRANSITION_COLOR)
	{
		this->stopAllActions();
		auto action = TintTo::create(_actionDuration, Color3B(_normalColor.r, _normalColor.b, _normalColor.g));
		auto fadeAction = FadeTo::create(_actionDuration, _normalColor.a);
		auto spwan = Spawn::create(action, fadeAction, nullptr);

		this->runAction(EaseSineOut::create(spwan));
	}
	else if (_transitionType == TRANSITION_SCALE)
	{
		this->stopAllActions();
		this->runAction(EaseSineOut::create(ScaleTo::create(_actionDuration, _backgroundTextureScaleX, _backgroundTextureScaleY)));
	}

	if (_isSelected)
	{
		_frontCrossRenderer->setVisible(true);
		_frontCrossRendererAdaptDirty = true;
	}
}

void CreatorRadioButton::onPressStateChangedToPressed()
{
	_backGroundBoxRenderer->setGLProgramState(this->getNormalGLProgramState(_backGroundBoxRenderer->getTexture()));
	_frontCrossRenderer->setGLProgramState(this->getNormalGLProgramState(_frontCrossRenderer->getTexture()));

	if (!_isBackgroundSelectedTextureLoaded)
	{
		//action
		this->stopAllActions();
		if (_transitionType == TRANSITION_SPRITE)
		{

		}
		else if (_transitionType == TRANSITION_COLOR)
		{
			auto action = TintTo::create(_actionDuration, Color3B(_pressedColor.r, _pressedColor.b, _pressedColor.g));
			auto fadeAction = FadeTo::create(_actionDuration, _pressedColor.a);
			auto spwan = Spawn::create(action, fadeAction, nullptr);

			this->runAction(EaseSineOut::create(spwan));
		}
		else if (_transitionType == TRANSITION_SCALE)
		{
			this->runAction(EaseSineOut::create(ScaleTo::create(_actionDuration, _zoomScale, _zoomScale)));
		}
	}
	else
	{
		_backGroundBoxRenderer->setVisible(false);
		_backGroundSelectedBoxRenderer->setVisible(true);
		_backGroundBoxDisabledRenderer->setVisible(false);
		_frontCrossDisabledRenderer->setVisible(false);
	}
}

void CreatorRadioButton::onPressStateChangedToDisabled()
{
	if (!_isBackgroundDisabledTextureLoaded
		|| !_isFrontCrossDisabledTextureLoaded)
	{
		_backGroundBoxRenderer->setGLProgramState(this->getGrayGLProgramState(_backGroundBoxRenderer->getTexture()));
		_frontCrossRenderer->setGLProgramState(this->getGrayGLProgramState(_frontCrossRenderer->getTexture()));
	}
	else
	{
		_backGroundBoxRenderer->setVisible(false);
		_backGroundBoxDisabledRenderer->setVisible(true);
	}

	_backGroundSelectedBoxRenderer->setVisible(false);
	_frontCrossRenderer->setVisible(false);
	_backGroundBoxRenderer->setScale(_backgroundTextureScaleX, _backgroundTextureScaleY);
	_frontCrossRenderer->setScale(_backgroundTextureScaleX, _backgroundTextureScaleY);

	if (_isSelected)
	{
		_frontCrossDisabledRenderer->setVisible(true);
		_frontCrossDisabledRendererAdaptDirty = true;
	}
}


void CreatorRadioButton::setNormalColor(Color4B& color)
{
	_normalColor = color;
}

Color4B CreatorRadioButton::getNormalColor()const
{
	return _normalColor;
}

void CreatorRadioButton::setPressedColor(Color4B& color)
{
	_pressedColor = color;
}

Color4B CreatorRadioButton::getPressedColor()const
{
	return _pressedColor;
}

void CreatorRadioButton::setDisableColor(Color4B& color)
{
	_disableColor = color;
}

Color4B CreatorRadioButton::getDisableColor()const
{
	return _disableColor;
}

void CreatorRadioButton::setTouchEnabled(bool enable)
{
	if (enable == _touchEnabled)
	{
		return;
	}
	ui::RadioButton::setTouchEnabled(enable);

	if (isAutoGrayEffect() && !enable)
	{
		if (enable)
		{
			_backGroundBoxRenderer->setGLProgramState(this->getNormalGLProgramState(_backGroundBoxRenderer->getTexture()));
			_frontCrossRenderer->setGLProgramState(this->getNormalGLProgramState(_frontCrossRenderer->getTexture()));
			_backGroundSelectedBoxRenderer->setGLProgramState(this->getNormalGLProgramState(_backGroundSelectedBoxRenderer->getTexture()));
		}
		else
		{
			_backGroundBoxRenderer->setGLProgramState(this->getGrayGLProgramState(_backGroundBoxRenderer->getTexture()));
			_frontCrossRenderer->setGLProgramState(this->getGrayGLProgramState(_frontCrossRenderer->getTexture()));
			_backGroundSelectedBoxRenderer->setGLProgramState(this->getGrayGLProgramState(_backGroundSelectedBoxRenderer->getTexture()));
		}
		
	}
}



NS_CCR_END