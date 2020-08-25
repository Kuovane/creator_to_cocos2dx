
local MainScene = class("MainScene", cc.load("mvc").ViewBase)

cc.exports.UIUtils = require "app.views.UIUtils"

local TestCreatorAni = require "app.views.TestCreatorAni"
local Testcollider = require "app.views.collider"
local TestSpine = require "app.views.spine"
local Testmask = require "app.views.mask"
local Testmotionstreak = require "app.views.motionstreak"
local Testpageview = require "app.views.pageview"
local Testprefab = require "app.views.prefab"
local Testrichtext = require "app.views.richtext"
local Testsprites = require "app.views.sprites"
local Testtilemap = require "app.views.tilemap"
local Testtoggle = require "app.views.toggle"
local Testui = require "app.views.ui"
local Testvideo = require "app.views.video"
local Testwebview = require "app.views.webview"
local Testlabel = require "app.views.label"
local Testslider = require "app.views.slider"
local Testtoggle_group= require "app.views.toggle_group"
local tests = {
	{name="TestCreatorAni",create_func=TestCreatorAni},
}



function MainScene:onCreate()
    self:init2()
end

function MainScene:init2()
    self.m_root = UIUtils:getRootNodeInCreatorEx(self,"creator/scenes/Main.ccreator")


    local btAnimation = UIUtils:seekNodeByName(self.m_root,"animation")
    btAnimation:addClickEventListener(function(sender)
        self:addChild(TestCreatorAni:create())
      end)


      local motionstreak = UIUtils:seekNodeByName(self.m_root,"motionstreak")
    motionstreak:addClickEventListener(function(sender)
        self:addChild(Testmotionstreak:create())
      end)

      local collider = UIUtils:seekNodeByName(self.m_root,"collider")
    collider:addClickEventListener(function(sender)
        self:addChild(Testcollider:create())
      end)

      local label = UIUtils:seekNodeByName(self.m_root,"label")
    label:addClickEventListener(function(sender)
        self:addChild(Testlabel:create())
      end)

      local mask = UIUtils:seekNodeByName(self.m_root,"mask")
    mask:addClickEventListener(function(sender)
        self:addChild(Testmask:create())
      end)

      local pageview = UIUtils:seekNodeByName(self.m_root,"pageview")
    pageview:addClickEventListener(function(sender)
        self:addChild(Testpageview:create())
      end)

      local prefab = UIUtils:seekNodeByName(self.m_root,"prefab")
    prefab:addClickEventListener(function(sender)
        self:addChild(Testprefab:create())
      end)

      local richtext = UIUtils:seekNodeByName(self.m_root,"richtext")
    richtext:addClickEventListener(function(sender)
        self:addChild(Testrichtext:create())
      end)

      local slider = UIUtils:seekNodeByName(self.m_root,"slider")
    slider:addClickEventListener(function(sender)
        self:addChild(Testslider:create())
      end)
      local sprites = UIUtils:seekNodeByName(self.m_root,"sprites")
    sprites:addClickEventListener(function(sender)
        self:addChild(Testsprites:create())
      end)

      local tilemap = UIUtils:seekNodeByName(self.m_root,"tilemap")
      tilemap:addClickEventListener(function(sender)
        self:addChild(Testtilemap:create())
        end)
        local toggle_group = UIUtils:seekNodeByName(self.m_root,"toggle_group")
    toggle_group:addClickEventListener(function(sender)
        self:addChild(Testtoggle_group:create())
      end)
      local toggle = UIUtils:seekNodeByName(self.m_root,"toggle")
    toggle:addClickEventListener(function(sender)
        self:addChild(Testtoggle:create())
      end)
      local ui = UIUtils:seekNodeByName(self.m_root,"ui")
    ui:addClickEventListener(function(sender)
        self:addChild(Testui:create())
      end)
      local video = UIUtils:seekNodeByName(self.m_root,"video")
    video:addClickEventListener(function(sender)
        self:addChild(Testvideo:create())
      end)
      local webview = UIUtils:seekNodeByName(self.m_root,"webview")
    webview:addClickEventListener(function(sender)
        self:addChild(Testwebview:create())
      end)
      local spine = UIUtils:seekNodeByName(self.m_root,"spine")
      spine:addClickEventListener(function(sender)
        self:addChild(TestSpine:create())
      end)




end

function MainScene:init()





	self.m_scrollView = ccui.ScrollView:create()
	self:addChild(self.m_scrollView);
	self.m_scrollView:setContentSize(cc.size(display.width,display.height));
	self.m_scrollView:setPosition(display.center)
	--获取 Container
	local container = self.m_scrollView:getInnerContainer()
	

	--添加 ArrangeNode 更好的操作 scrollview
	local arrangeNode = cc.Node:create()
	self:addChild(arrangeNode)

	local dy = 50
	local len = #tests
	for i=1,len do
		local item = ccui.Button:create()
		item:setContentSize(cc.size(300,40))
		item:setTitleText(tests[i].name)		
		item:setTitleColor(cc.WHITE)--cc.c3b(_r,_g,_b))
		item:setTitleFontSize(40)
		--item:getTitleLabel():setAnchorPoint(cc.p(0.5,1))
		item:setAnchorPoint(cc.p(0,1))
		item:setPressedActionEnabled(true)
		item:addClickEventListener(function(sender)
			local layer = tests[i].create_func:create()
			cc.Director:getInstance():getRunningScene():addChild(layer)
		  end)

		arrangeNode:addChild(item)
		item:setPositionY(-(i-1)*dy)
	end



	local s = self.m_scrollView:getContentSize()
	local height = len*dy

	if height < s.height then
		height = s.height
	end

	arrangeNode:setPosition(20,height)

	self.m_scrollView:setInnerContainerSize(cc.size(s.width,height))
	--self.m_scrollView:scrollToTop(1.0,true)
	--self.m_scrollView:jumpToTop()
end

return MainScene
