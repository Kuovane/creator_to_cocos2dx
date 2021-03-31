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
#include "CreatorReader_generated.h"

#include "Macros.h"

NS_CCR_BEGIN

// name of extra layout Node
#define PLUGIN_EXTRA_LAYOUT_NAME "Creator Widget to Cocos2d-x Layout"

// creator align flag, see: https://github.com/cocos-creator/engine/blob/master/cocos2d/core/base-ui/CCWidgetManager.js
#define CREATOR_ALIGN_TOP    (1 << 0)
#define CREATOR_ALIGN_MID    (1 << 1)
#define CREATOR_ALIGN_BOT    (1 << 2)
#define CREATOR_ALIGN_LEFT   (1 << 3)
#define CREATOR_ALIGN_CENTER (1 << 4)
#define CREATOR_ALIGN_RIGHT  (1 << 5)

// support export the creator widget component to cocos2d-x layout
class WidgetAdapter : public cocos2d::Ref
{

public:
    // only 9 creator align combinations are supported by cocos2d-x
    enum class AlignComb
    {
		NONE = 0,
        TOP_LEFT = CREATOR_ALIGN_LEFT | CREATOR_ALIGN_TOP,
        TOP_CENTER_HORIZONTAL = CREATOR_ALIGN_CENTER | CREATOR_ALIGN_TOP,
        TOP_RIGHT = CREATOR_ALIGN_TOP | CREATOR_ALIGN_RIGHT,
        LEFT_CENTER_VERTICAL = CREATOR_ALIGN_MID | CREATOR_ALIGN_LEFT,
		TOP_BOTTOM = CREATOR_ALIGN_TOP | CREATOR_ALIGN_BOT,

        CENTER_IN_PARENT = CREATOR_ALIGN_CENTER | CREATOR_ALIGN_MID,

        RIGHT_CENTER_VERTICAL = CREATOR_ALIGN_MID | CREATOR_ALIGN_RIGHT,
        LEFT_BOTTOM = CREATOR_ALIGN_BOT | CREATOR_ALIGN_LEFT,
        BOTTOM_CENTER_HORIZONTAL = CREATOR_ALIGN_CENTER | CREATOR_ALIGN_BOT,
        RIGHT_BOTTOM = CREATOR_ALIGN_RIGHT | CREATOR_ALIGN_BOT,
		LEFT_RIGHT = CREATOR_ALIGN_LEFT| CREATOR_ALIGN_RIGHT,

		LEFT_TOP_BOTTOM_RIGHT = CREATOR_ALIGN_LEFT | CREATOR_ALIGN_RIGHT| CREATOR_ALIGN_TOP| CREATOR_ALIGN_BOT,

		LEFT_TOP_RIGHT = CREATOR_ALIGN_LEFT | CREATOR_ALIGN_RIGHT | CREATOR_ALIGN_TOP,
		LEFT_BOTTOM_RIGHT = CREATOR_ALIGN_LEFT | CREATOR_ALIGN_RIGHT | CREATOR_ALIGN_BOT,
		LEFT_BOTTOM_TOP = CREATOR_ALIGN_LEFT  | CREATOR_ALIGN_TOP | CREATOR_ALIGN_BOT,
		RIGHT_BOTTOM_TOP =   CREATOR_ALIGN_RIGHT | CREATOR_ALIGN_TOP | CREATOR_ALIGN_BOT,
		LEFT_RIGHT_CENTER_VERTICAL = CREATOR_ALIGN_RIGHT | CREATOR_ALIGN_LEFT | CREATOR_ALIGN_MID,
		BOTTOM_TOP_CENTER_HORIZONTAL = CREATOR_ALIGN_TOP | CREATOR_ALIGN_BOT | CREATOR_ALIGN_CENTER,

		TOP= CREATOR_ALIGN_TOP,
		LEFT= CREATOR_ALIGN_LEFT,
		RIGHT= CREATOR_ALIGN_RIGHT,
		BOTTOM = CREATOR_ALIGN_BOT,
		CENTER_VERTICAL = CREATOR_ALIGN_MID,
		CENTER_HORIZONTAL = CREATOR_ALIGN_CENTER,

		
    };


	class  Margin
	{
	public:
		/**
		* Left margin.
		*/
		float left;
		/**
		* Top margin.
		*/
		float top;
		/**
		* Right margin.
		*/
		float right;
		/**
		* Bottom margin.
		*/
		float bottom;

		float hcenter;

		float vcenter;


		bool _isAbsLeft;
		bool _isAbsRight;
		bool _isAbsTop;
		bool _isAbsBottom;
		bool _isAbsHorizontalCenter;
		bool _isAbsVerticalCenter;

	public:
		/**
		* Default constructor.
		*/
		//Margin() {};

		/**
		* Construct a Margin instance with left, top, right and bottom margins.
		*@param l Left margin in float.
		*@param t Top margin in float.
		*@param r Right margin in float.
		*@param b Bottom margin in float.
		*/
		Margin(float l, float t, float r, float b,float hc,float vc) {
			left = l;
			top = t;
			right = r;
			bottom = b;
			hcenter = hc;
			vcenter = vc;

			_isAbsLeft = true;
			_isAbsRight = true;
			_isAbsTop = true;
			_isAbsBottom = true;

			_isAbsHorizontalCenter = true;
			_isAbsVerticalCenter = true;
		};


		void setMargin(float l, float t, float r, float b, float hc, float vc) {
			left = l;
			top = t;
			right = r;
			bottom = b;
			hcenter = hc;
			vcenter = vc;

		};

		void setMarginAbs(bool l, bool t, bool r, bool b, bool hc, bool vc) {

			 _isAbsLeft = l;
			 _isAbsRight = r;
			 _isAbsTop = t;
			 _isAbsBottom = b;

			 _isAbsHorizontalCenter = hc;
			 _isAbsVerticalCenter = vc;
		}
	};



    static WidgetAdapter* create();
    WidgetAdapter();
    virtual ~WidgetAdapter();

    bool init();
    void setIsAlignOnce(bool isAlignOnce);
    void setAdaptNode(cocos2d::Node* needAdaptNode);
	cocos2d::Node* getAdaptNode();

	//void setLayoutParameter(cocos2d::ui::RelativeLayoutParameter *parameter);
	void setMargin(const Margin &sMargin);
	void setAlignComb(AlignComb align);

    // TODO: support the align target of a widget component, default target is parent Node
    void setLayoutTarget(cocos2d::Node* layoutTarget);
private:
    friend class WidgetManager;
    // only do layout align once if true
    bool _isAlignOnce;
    // widget layout target, it's a Node, default target is _needAdaptNode's parent
    cocos2d::Node* _layoutTarget;
    // the node include a widget component, it must be a UI Widget?
    cocos2d::Node* _needAdaptNode;
    // insert the _layout between _nodeNeedWidget and its parent
   // cocos2d::ui::Layout* _layoutNode;

	//cocos2d::ui::RelativeLayoutParameter *_parameter;
	Margin _sMargin;
	AlignComb _alignComb;





    // insert Layout Node to support widget component.
    void setupLayout();
    // adapt layout property depend on _layoutTarget
    void syncLayoutProperty();


	float getLeftX(float parentWidth, float nodeWidth, float anchorX, float left, bool isAbs = true);
	float getRightX(float parentWidth, float nodeWidth, float anchorX, float right, bool isAbs = true);
	float getTopY(float parentHeight,float nodeHeight,float anchorY,float top, bool isAbs = true);
	float getBottomY(float parentHeight, float nodeHeight, float anchorY, float bottom, bool isAbs = true);
	float getCenterX(float parentWidth, float nodeWidth, float anchorX, float offset, bool isAbs = true);
	float getCenterY(float parentHeight, float nodeHeight, float anchorY, float offset, bool isAbs = true);

};

// manager all the widget component align
class WidgetManager : public cocos2d::Node
{
public:
    // check widget component property AlignOnce every frame, update align if this property set to false
    virtual void update(float dt) override;
    // do layout align manually, you should call it when you make layout content size different from scene in Creator.
    void forceDoAlign();
	void forceDoAlignAssignNode(cocos2d::Node* assignNode);
	static void setSafeUIRect(cocos2d::Rect &sRect);
	static cocos2d::Rect& getSafeUIRect() {
		return m_sSafeUIRect;
	};
	virtual void cleanup()override;
	void addWidget(WidgetAdapter*);
	void removeWidgetByNode(cocos2d::Node* pNode);
private:
    friend class CreatorReader;

    WidgetManager();
    virtual ~WidgetManager();
    void setupWidgets();
    void doAlign();
    bool _forceAlignDirty;
	static cocos2d::Rect m_sSafeUIRect;
    cocos2d::Vector<WidgetAdapter*> _needAdaptWidgets;
};
NS_CCR_END
