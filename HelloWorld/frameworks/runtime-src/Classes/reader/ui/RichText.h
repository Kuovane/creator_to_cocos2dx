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
#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Macros.h"


using namespace cocos2d;
NS_CCR_BEGIN

class CreatorRichText : public ui::RichText
{
public :


	CreatorRichText();
	~CreatorRichText();
	/**
	* Create and return a empty CreatorLayout instance pointer.
	*/
	static CreatorRichText* create();

	ProtectedNode* createArrangeNode();

	void removeAllElements();

	int getElementCount();
	
	void flipElements();

	void setElementsAnchorPoint(Vec2 sVec2);

	void formatRenderers();

	ui::RichElement * getElement(int index);

	void setElementText(int index, std::string sContent);

	void setXMLData(std::string);

private:
	ProtectedNode* _arrangeNode;
	Vec2 m_sDefaultAnchorPoint;

};

NS_CCR_END
