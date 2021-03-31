#include "RichText.h"
#include "ui/RichtextStringVisitor.h"

NS_CCR_BEGIN

using namespace ui;

CreatorRichText* CreatorRichText::create()
{
	CreatorRichText* widget = new (std::nothrow) CreatorRichText();
	if (widget && widget->init())
	{
		widget->autorelease();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

CreatorRichText::CreatorRichText()
	:_arrangeNode(nullptr)
{
	m_sDefaultAnchorPoint = Vec2::UNIT_Y;
}

CreatorRichText::~CreatorRichText()
{

	if (_arrangeNode)
	{
		_arrangeNode->release();
	}
}


ui::RichElement *CreatorRichText::getElement(int index)
{
	return _richElements.at(index);
}


void CreatorRichText::setElementText(int index, std::string sContent)
{
	auto pElement = getElement(index);
	if (pElement)
	{
		auto pElementText = dynamic_cast<ui::RichElementText*>(pElement);
		if (pElementText)
		{
			pElementText->setText(sContent);
			_formatTextDirty = true;
		}

	}
}



ProtectedNode* CreatorRichText::createArrangeNode()
{
	if (_arrangeNode)
	{
		if (_arrangeNode->getParent())
		{
			_arrangeNode->removeFromParent();
		}

		_arrangeNode->release();
	}


	_arrangeNode = ProtectedNode::create();
	_arrangeNode->retain();
	this->addProtectedChild(_arrangeNode);
	return _arrangeNode;
}

void CreatorRichText::removeAllElements()
{
	_richElements.clear();
	_formatTextDirty = true;

//	formatText();
}

int CreatorRichText::getElementCount()
{
	return _richElements.size();
}

void CreatorRichText::flipElements()
{
	{
		int count = getElementCount();
		Vector<RichElement*> sTemp;
		for (int i = 0; i < count; i++)
		{
			sTemp.pushBack(_richElements.at(count - i - 1));
		}
		_richElements.clear();
		_richElements = sTemp;
	}

	_formatTextDirty = true;
}

void CreatorRichText::setElementsAnchorPoint(Vec2 sVec2)
{
	m_sDefaultAnchorPoint = sVec2;
}

void CreatorRichText::formatRenderers()
{
    if (_formatTextDirty) {
        createArrangeNode();

        float verticalSpace = _defaults[KEY_VERTICAL_SPACE].asFloat();
        float fontSize = _defaults[KEY_FONT_SIZE].asFloat();

        if (_ignoreSize)
        {
            float newContentSizeWidth = 0.0f;
            float nextPosY = 0.0f;
            float firstRowHeight = 0.0f;
            std::vector<std::pair<Vector<Node*>*, float> > rowWidthPairs;
            rowWidthPairs.reserve(_elementRenders.size());
            //for (auto& element : _elementRenders)
            for (size_t i = 0, size = _elementRenders.size(); i < size; i++)
            {
                auto& element = _elementRenders[i];
                float nextPosX = 0.0f;
                float maxY = 0.0f;
                float lineWidth = 0.0f;
                for (auto& iter : element)
                {
                    iter->setAnchorPoint(m_sDefaultAnchorPoint);
                    iter->setPosition(nextPosX, nextPosY);
                    _arrangeNode->addProtectedChild(iter, 1);
                    Size iSize = iter->getContentSize();
                    lineWidth += iSize.width;
                    nextPosX += iSize.width;
                    maxY = MAX(maxY, iSize.height);
                }
                if (i == 0)
                {
                    firstRowHeight = maxY;
                }
                newContentSizeWidth = MAX(newContentSizeWidth, lineWidth);
                nextPosY -= (maxY + verticalSpace);
                rowWidthPairs.emplace_back(&element, nextPosX);
            }
            if (nextPosY < 0.0f)
            {
                nextPosY += verticalSpace;
            }
            this->setContentSize(Size(newContentSizeWidth, -nextPosY));
            _arrangeNode->setPositionY(-nextPosY - firstRowHeight*(1- m_sDefaultAnchorPoint.y));

            for (auto& row : rowWidthPairs)
                doHorizontalAlignment(*row.first, row.second);
        }
        else
        {
            // calculate real height
            float newContentSizeHeight = 0.0f;
            std::vector<float> maxHeights(_elementRenders.size());

            for (size_t i = 0, size = _elementRenders.size(); i<size; i++)
            {
                Vector<Node*>& row = _elementRenders[i];
                float maxHeight = 0.0f;
                for (auto& iter : row)
                {
                    maxHeight = std::max(iter->getContentSize().height, maxHeight);
                }

                // gap for empty line, if _lineHeights[i] == 0, use current RichText's fontSize
                if (row.empty())
                {
                    maxHeight = (_lineHeights[i] != 0.0f ? _lineHeights[i] : fontSize);
                }
                maxHeights[i] = maxHeight;

                // vertical space except for first line
                newContentSizeHeight += (i != 0 ? maxHeight + verticalSpace : maxHeight);
            }
            _customSize.height = newContentSizeHeight;

            // align renders
            float nextPosY = _customSize.height;
            for (size_t i = 0, size = _elementRenders.size(); i<size; i++)
            {
                Vector<Node*>& row = _elementRenders[i];
                float nextPosX = 0.0f;
                nextPosY -= (i != 0 ? maxHeights[i] + verticalSpace : maxHeights[i]);
                for (auto& iter : row)
                {
                    iter->setAnchorPoint(m_sDefaultAnchorPoint);
                    iter->setPosition(nextPosX, nextPosY + maxHeights[i]* m_sDefaultAnchorPoint.y);
                    this->addProtectedChild(iter, 1);
                    nextPosX += iter->getContentSize().width;
                }

                doHorizontalAlignment(row, nextPosX);
            }
        }

        _elementRenders.clear();
        _lineHeights.clear();

        if (_ignoreSize)
        {
            Size s = getVirtualRendererSize();
            this->setContentSize(s);
        }
        else
        {
            this->setContentSize(_customSize);
        }
    }

	updateContentSizeWithTextureSize(_contentSize);
}

static std::string strReplace(const char *pszSrc, const char *pszOld, const char *pszNew)
{
	std::string strContent, strTemp;
	strContent.assign(pszSrc);
	std::string::size_type nPos = 0;
	while (true)
	{
		nPos = strContent.find(pszOld, nPos);
		strTemp = strContent.substr(nPos + strlen(pszOld), strContent.length());
		if (nPos == std::string::npos)
		{
			break;
		}
		strContent.replace(nPos, strContent.length(), pszNew);
		strContent.append(strTemp);
		nPos += strlen(pszNew) - strlen(pszOld) + 1; //·ÀÖ¹ÖØ¸´Ìæ»» ±ÜÃâËÀÑ­»·
	}
	return strContent;
}



void CreatorRichText::setXMLData(std::string xmlData)
{
	removeAllElements();

	if (!xmlData.empty())
	{
		auto ap = this->getAnchorPoint();
		auto igonre = this->isIgnoreContentAdaptWithSize();
		std::string sContent = strReplace(xmlData.c_str(), "\n", "<br/>");

		RichtextStringVisitor visitor;;
		SAXParser parser;
		parser.setDelegator(&visitor);
		parser.parseIntrusive(const_cast<char*>(sContent.c_str()), sContent.length());

		initWithXML(visitor.getOutput());
		this->setAnchorPoint(ap);
		this->ignoreContentAdaptWithSize(igonre);
	}

}


NS_CCR_END