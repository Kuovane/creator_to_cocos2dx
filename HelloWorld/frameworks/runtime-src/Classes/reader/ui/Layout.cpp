#include "Layout.h"

NS_CCR_BEGIN


CreatorLayout* CreatorLayout::create()
{
	CreatorLayout* widget = new (std::nothrow) CreatorLayout();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

CreatorLayout::CreatorLayout()
{

}

CreatorLayout::~CreatorLayout()
{
}



void CreatorLayout::addChild(Node *child, int zOrder, int tag)
{
	child->setGlobalZOrder(_globalZOrder);
	ui::Widget::addChild(child, zOrder, tag);
	_doLayoutDirty = true;
}

void CreatorLayout::addChild(Node* child, int zOrder, const std::string &name)
{
	child->setGlobalZOrder(_globalZOrder);
	ui::Widget::addChild(child, zOrder, name);
	_doLayoutDirty = true;
}


void CreatorLayout::doResizeModeLayout()
{
	
}

void CreatorLayout::doHorizonalLayout()
{
	auto pChildren = getChildren();
	auto parentSize = this->getContentSize();

	if (_horizontalDirection == HDirectionType::LEFT_TO_RIGHT)
	{
		float startX = _paddingLeft;
		float dx = _spacingX;
		int count = getChildrenCount();

		for (int i = 0; i < count; ++i)
		{
			auto pChild = pChildren.at(i);

			if (!pChild->isVisible())
			{
				continue;
			}

			auto size = pChild->getContentSize();
			auto anr = pChild->getAnchorPoint();

			float leftWidth = size.width * anr.x;
			
			float x = startX + leftWidth;

			pChild->setPositionX(x);

			startX = startX + size.width + dx;

		}

		switch (_resizeModeType)
		{
		case creator::CreatorLayout::ResizeModeType::CONTAINER:
		{
			float targetWidth = startX - _spacingX + _paddingRight;
			this->setContentSize(Size(targetWidth, parentSize.height));

		}
		break;
		case creator::CreatorLayout::ResizeModeType::CHILDREN:
		{

			float targetWidth = parentSize.width - _paddingRight - _paddingLeft;
			targetWidth = targetWidth - (count - 1)*_spacingX;


			float itemWidth = targetWidth / count;


			for (int i = 0; i < count; ++i)
			{
				auto pChild = pChildren.at(i);

				pChild->setContentSize(Size(itemWidth, pChild->getContentSize().height));

			}

		}

		break;
		default:
			break;
		}
	}
	else if (_horizontalDirection == HDirectionType::RIGHT_TO_LEFT)
	{
		
		float dx = _spacingX;
		int count = getChildrenCount();
		

		float startX = - _paddingRight;

		for (int i = 0; i < count; ++i)
		{
			auto pChild = pChildren.at(i);
			if (!pChild->isVisible())
			{
				continue;
			}
			auto size = pChild->getContentSize();
			auto anr = pChild->getAnchorPoint();

			float rightWidth = size.width * (1.0f - anr.x);

			float x = startX - rightWidth;

			pChild->setPositionX(x);

			startX = startX - size.width- dx;

		}



		switch (_resizeModeType)
		{
		case creator::CreatorLayout::ResizeModeType::CONTAINER:
		case creator::CreatorLayout::ResizeModeType::NONE:
		{
			float targetWidth = - (startX + _spacingX - _paddingLeft);
			this->setContentSize(Size(targetWidth, parentSize.height));


		

			for (int i = 0; i < count; ++i)
			{
				auto pChild = pChildren.at(i);

				float x = pChild->getPositionX();
				x += targetWidth;
				pChild->setPositionX(x);
			}


		}
		break;
		case creator::CreatorLayout::ResizeModeType::CHILDREN:
		{

			float targetWidth = parentSize.width - _paddingRight - _paddingLeft;
			targetWidth = targetWidth - (count - 1)*_spacingX;


			float itemWidth = targetWidth / count;


			for (int i = 0; i < count; ++i)
			{
				auto pChild = pChildren.at(i);

				pChild->setContentSize(Size(itemWidth, pChild->getContentSize().height));
			}
		}

		break;
		default:
			break;
		}

	}

	
}

void CreatorLayout::doVerticalLayout()
{
	auto pChildren = getChildren();
	auto parentSize = this->getContentSize();

	if (_verticalDirection == VDirectionType::BOTTOM_TO_TOP)
	{
		float startY = _paddingBottom;
		//float d = _spacingY;
		int count = getChildrenCount();

		for (int i = 0; i < count; ++i)
		{
			auto pChild = pChildren.at(i);
			if (!pChild->isVisible())
			{
				continue;
			}
			auto size = pChild->getContentSize();
			auto anr = pChild->getAnchorPoint();

			float bottomHeight = size.height * anr.y;

			float y = startY + bottomHeight;

			pChild->setPositionY(y);

			startY = startY + size.height + _spacingY;

		}

		switch (_resizeModeType)
		{
		case creator::CreatorLayout::ResizeModeType::CONTAINER:
		{
			float targetHeight= startY - _spacingY + _paddingTop;
			this->setContentSize(Size(parentSize.width, targetHeight));

		}
		break;
		case creator::CreatorLayout::ResizeModeType::CHILDREN:
		{

			float targetHeight= parentSize.height - _paddingTop - _paddingBottom;
			targetHeight = targetHeight - (count - 1)*_spacingY;


			float itemHeight = targetHeight / count;


			for (int i = 0; i < count; ++i)
			{
				auto pChild = pChildren.at(i);

				pChild->setContentSize(Size(pChild->getContentSize().width, itemHeight));

			}

		}

		break;
		default:
			break;
		}
	}
	else if (_verticalDirection == VDirectionType::TOP_TO_BOTTOM)
	{

		int count = getChildrenCount();


		float startY = 0 - _paddingTop; //parentSize.height - _paddingTop;

		for (int i = 0; i < count; ++i)
		{
			auto pChild = pChildren.at(i);
			if (!pChild->isVisible())
			{
				continue;
			}
			auto size = pChild->getContentSize();
			auto anr = pChild->getAnchorPoint();

			float rightHeight = size.height * (1.0f - anr.y);

			float y = startY - rightHeight;

			pChild->setPositionY(y);

			startY = startY - size.height - _spacingY;

		}




		switch (_resizeModeType)
		{
		
		case creator::CreatorLayout::ResizeModeType::CHILDREN:
		{

			float targetHeight = parentSize.height - _paddingTop - _paddingBottom;
			targetHeight = targetHeight - (count - 1)*_spacingY;


			float itemHeight = targetHeight / count;


			for (int i = 0; i < count; ++i)
			{
				auto pChild = pChildren.at(i);

				pChild->setContentSize(Size(pChild->getContentSize().width, itemHeight));

			}

		}

		break;
		case creator::CreatorLayout::ResizeModeType::CONTAINER:
		case creator::CreatorLayout::ResizeModeType::NONE:
		{
			startY = startY + _spacingY - _paddingBottom;

			float targetHeight = -startY;//parentSize.height- (startY + _spacingY - _paddingBottom);
			this->setContentSize(Size(parentSize.width, targetHeight));


			for (int i = 0; i < count; ++i)
			{
				auto pChild = pChildren.at(i);

				float y = pChild->getPositionY();
				y += targetHeight;
				pChild->setPositionY(y);
			}

		}
		default:


			break;
		}

	}
}


void CreatorLayout::doGridLayout()
{
	auto pChildren = getChildren();
	int count = getChildrenCount();

	auto parentSize = this->getContentSize();


	if (count > 0)
	{
		
		Vec2 sCurPos = getGridStartPos();
		Vec2 sNextPos = sCurPos;
		auto node = getFirstNode();//pChildren.at(0);
		auto sCurlineSize = node->getContentSize();

		for (int i = 0; i < count; ++i)
		{
			auto pChild = pChildren.at(i);
			if (!pChild->isVisible())
			{
				continue;
			}
			sCurPos = sNextPos;
			sNextPos = setGridCellPos(pChild, sCurPos, sCurlineSize);
			
		}

		if (_resizeModeType == ResizeModeType::CONTAINER || _resizeModeType == ResizeModeType::CHILDREN)
		{
			if (_startAxis == StartAxis::HORIZONAL)
			{

				if (_verticalDirection == VDirectionType::BOTTOM_TO_TOP)
				{
					float h = sCurPos.y + sCurlineSize.height + _paddingTop;
					this->setContentSize(Size(parentSize.width, h));
				}
				else
				{

					float targetHeight = - (sCurPos.y - sCurlineSize.height) + _paddingBottom;
					this->setContentSize(Size(parentSize.width, targetHeight));


					for (int i = 0; i < count; ++i)
					{
						auto pChild = pChildren.at(i);

						float y = pChild->getPositionY();
						y += targetHeight;
						pChild->setPositionY(y);
					}

					
				}				
			}
			else
			{
				if (_horizontalDirection == HDirectionType::LEFT_TO_RIGHT)
				{
					float w = sCurPos.x + sCurlineSize.width + _paddingRight;
					this->setContentSize(Size(w, parentSize.height));
				}
				else
				{
					float w =  - (sCurPos.x - sCurlineSize.width) + _paddingLeft;
					this->setContentSize(Size(w, parentSize.height));

					for (int i = 0; i < count; ++i)
					{
						auto pChild = pChildren.at(i);

						float x = pChild->getPositionX();
						x += w;
						pChild->setPositionX(x);
					}
				}
			}
		}
	}

	//float startY = parentSize.height - _paddingTop;

	

	
}

cocos2d::Node* CreatorLayout::getFirstNode()
{
	auto pChildren = getChildren();
	for (auto it = pChildren.begin(); it != pChildren.end(); it++)
	{
		if ((*it)->isVisible())
		{
			return (*it);
		}
	}
	return pChildren.at(0);
}

void CreatorLayout::doLayout()
{

	if (!_doLayoutDirty)
	{
		return;
	}

	sortAllChildren();

	switch (_CreatorType)
	{
	case creator::CreatorLayout::CreatorType::NONE:
		break;
	case creator::CreatorLayout::CreatorType::HORIZONAL:
		doHorizonalLayout();
		break;
	case creator::CreatorLayout::CreatorType::VERTICAL:
		doVerticalLayout();
		break;
	case creator::CreatorLayout::CreatorType::GRID:
		doGridLayout();
		break;
	default:
		break;
	}

	_doLayoutDirty = false;
}


cocos2d::Vec2 CreatorLayout::getGridStartPos()
{
	cocos2d::Vec2 sRet;
	


	if (_resizeModeType == ResizeModeType::CHILDREN)
	{
		auto parentSize = this->getContentSize();
		sRet.x = _horizontalDirection == HDirectionType::LEFT_TO_RIGHT ? _paddingLeft : parentSize.width -_paddingRight;

		sRet.y = _verticalDirection == VDirectionType::BOTTOM_TO_TOP ? _paddingBottom : parentSize.height -_paddingTop;
	}
	else

	{
		auto parentSize = this->getContentSize();
		if (_startAxis == StartAxis::HORIZONAL)
		{
			sRet.x = _horizontalDirection == HDirectionType::LEFT_TO_RIGHT ? _paddingLeft : parentSize.width - _paddingRight;

			sRet.y = _verticalDirection == VDirectionType::BOTTOM_TO_TOP ? _paddingBottom : -_paddingTop;
		}
		else
		{
			sRet.x = _horizontalDirection == HDirectionType::LEFT_TO_RIGHT ? _paddingLeft : -_paddingRight;

			sRet.y = _verticalDirection == VDirectionType::BOTTOM_TO_TOP ? _paddingBottom : parentSize.height - _paddingTop;
		}

	}

	

	return sRet;
}

Vec2 CreatorLayout::setGridCellPos(Node *pNode, cocos2d::Vec2& desPos, cocos2d::Size &sCurLineSize)
{
	Vec2 sNextPos;
	auto parentSize = this->getContentSize();

	if (_resizeModeType == ResizeModeType::NONE)
	{
		if (_startAxis == StartAxis::HORIZONAL)
		{
			sNextPos = setGridCellPosInHAxizAndNoneMode(pNode, desPos, sCurLineSize);
		}
		else
		{
			sNextPos = setGridCellPosInVAxizAndNoneMode(pNode, desPos, sCurLineSize);
		}
	}

	else if (_resizeModeType == ResizeModeType::CONTAINER)
	{
		if (_startAxis == StartAxis::HORIZONAL)
		{
			sNextPos = setGridCellPosInHAxizAndNoneMode(pNode, desPos, sCurLineSize);
		}
		else
		{
			sNextPos = setGridCellPosInVAxizAndNoneMode(pNode, desPos, sCurLineSize);
		}
	}

	else if (_resizeModeType == ResizeModeType::CHILDREN)
	{
		pNode->setContentSize(_cellSize);
		if (_startAxis == StartAxis::HORIZONAL)
		{
			sNextPos = setGridCellPosInHAxizAndNoneMode(pNode, desPos, sCurLineSize);
		}
		else
		{
			sNextPos = setGridCellPosInVAxizAndNoneMode(pNode, desPos, sCurLineSize);
		}
	}

	return sNextPos;
}

cocos2d::Vec2 CreatorLayout::setGridCellPosInHAxizAndNoneMode(Node* pNode, cocos2d::Vec2& desPos, cocos2d::Size &sCurLineSize)
{

	Vec2 sNextPos;
	auto parentSize = this->getContentSize();

	bool bFlagNextLine = false;
	bool bFlag2NextLine = false;

	auto size = pNode->getContentSize();
	auto anr = pNode->getAnchorPoint();
	float x = 0, y = 0;

	//LEFT_TO_RIGHT
	if (_horizontalDirection == HDirectionType::LEFT_TO_RIGHT)
	{
		x = desPos.x + size.width*anr.x;
		//超出边界
		if (desPos.x + size.width + _paddingRight > parentSize.width)
		{
			bFlagNextLine = true;
			x = _paddingLeft + size.width*anr.x;
			desPos.x = _paddingLeft;
		}
		pNode->setPositionX(x);

		//下一个元素 x 位置
		sNextPos.x = desPos.x + _spacingX + size.width;
		if (sNextPos.x + _paddingRight > parentSize.width)
		{
			bFlag2NextLine = true;
			sNextPos.x = _paddingLeft;
		}

	}
	//RIGHT_TO_LEFT
	else if (_horizontalDirection == HDirectionType::RIGHT_TO_LEFT)
	{
		x = desPos.x - size.width*(1 - anr.x);
		//超出边界
		if (desPos.x - size.width - _paddingLeft < 0)
		{
			bFlagNextLine = true;
			x = (parentSize.width - _paddingRight) - size.width*(1 - anr.x);
			desPos.x = (parentSize.width - _paddingRight);
		}
		pNode->setPositionX(x);


		//下一个元素 x 位置
		sNextPos.x = desPos.x - _spacingX - size.width;
		if (sNextPos.x - _paddingLeft < 0)
		{
			bFlag2NextLine = true;
			sNextPos.x = (parentSize.width - _paddingRight);
		}

	}

	//BOTTOM_TO_TOP
	if (_verticalDirection == VDirectionType::BOTTOM_TO_TOP)
	{
		//换行
		if (bFlagNextLine)
		{
			desPos.y = desPos.y + _spacingY + sCurLineSize.height;
			sCurLineSize.height = size.height;
		}


		y = desPos.y + size.height *  anr.y;
		pNode->setPositionY(y);


		////下一个元素 y 位置
		if (bFlag2NextLine)
		{
			sNextPos.y = desPos.y + _spacingY + sCurLineSize.height;
		}
		else
		{
			sNextPos.y = desPos.y;
		}
	}
	else if (_verticalDirection == VDirectionType::TOP_TO_BOTTOM)
	{
		//换行
		if (bFlagNextLine)
		{
			desPos.y = desPos.y - _spacingY - sCurLineSize.height;
			sCurLineSize.height = size.height;
		}


		y = desPos.y - size.height * (1 - anr.y);
		pNode->setPositionY(y);


		////下一个元素 y 位置
		if (bFlag2NextLine)
		{
			sNextPos.y = desPos.y - _spacingY - sCurLineSize.height;
		}
		else
		{
			sNextPos.y = desPos.y;
		}
	}

	return sNextPos;
}


cocos2d::Vec2 CreatorLayout::setGridCellPosInVAxizAndNoneMode(Node* pNode, cocos2d::Vec2& desPos, cocos2d::Size &sCurLineSize)
{

	Vec2 sNextPos;
	auto parentSize = this->getContentSize();

	bool bFlagNextLine = false;
	bool bFlag2NextLine = false;

	auto size = pNode->getContentSize();
	auto anr = pNode->getAnchorPoint();
	float x = 0, y = 0;

	//BOTTOM_TO_TOP
	if (_verticalDirection == VDirectionType::BOTTOM_TO_TOP)
	{
		y = desPos.y + size.height*anr.y;
		//超出边界
		if (desPos.y + size.height + _paddingTop > parentSize.height)
		{
			bFlagNextLine = true;
			y = _paddingBottom + size.height*anr.y;
			desPos.y = _paddingBottom;
		}
		pNode->setPositionY(y);

		//下一个元素 y 位置
		sNextPos.y = desPos.y + _spacingY + size.height;
		if (sNextPos.y + _paddingTop > parentSize.height)
		{
			bFlag2NextLine = true;
			sNextPos.y = _paddingBottom;
		}

	}
	//RIGHT_TO_LEFT
	else if (_verticalDirection == VDirectionType::TOP_TO_BOTTOM)
	{
		y = desPos.y - size.height*(1 - anr.y);
		//超出边界
		if (desPos.y - size.height - _paddingBottom < 0)
		{
			bFlagNextLine = true;
			y = (parentSize.height - _paddingTop) - size.height*(1 - anr.y);
			desPos.y = (parentSize.height - _paddingTop);
		}
		pNode->setPositionY(y);


		//下一个元素 y 位置
		sNextPos.y = desPos.y - _spacingY - size.height;
		if (sNextPos.y - _paddingBottom < 0)
		{
			bFlag2NextLine = true;
			sNextPos.y = (parentSize.height - _paddingTop);
		}

	}

	//LEFT_TO_RIGHT
	if (_horizontalDirection == HDirectionType::LEFT_TO_RIGHT)
	{
		//换行
		if (bFlagNextLine)
		{
			desPos.x = desPos.x + _spacingX + sCurLineSize.width;
			sCurLineSize.width = size.width;
		}


		x = desPos.x + size.width *  anr.x;
		pNode->setPositionX(x);


		////下一个元素 x 位置
		if (bFlag2NextLine)
		{
			sNextPos.x = desPos.x + _spacingX + sCurLineSize.width;
		}
		else
		{
			sNextPos.x = desPos.x;
		}
	}
	else if (_horizontalDirection == HDirectionType::RIGHT_TO_LEFT)
	{
		//换行
		if (bFlagNextLine)
		{
			desPos.x = desPos.x - _spacingX - sCurLineSize.width;
			sCurLineSize.width = size.width;
		}


		x = desPos.x - size.width * (1 - anr.x);
		pNode->setPositionX(x);


		////下一个元素 x 位置
		if (bFlag2NextLine)
		{
			sNextPos.x = desPos.x - _spacingX - sCurLineSize.width;
		}
		else
		{
			sNextPos.x = desPos.x;
		}
	}
	return sNextPos;
}

void CreatorLayout::setCellSize(float w, float h) {
	_cellSize.width = w; _cellSize.height = h;
}



NS_CCR_END