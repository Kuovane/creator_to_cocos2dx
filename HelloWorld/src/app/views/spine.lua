
local PopupLayer = require "app.views.PopupLayer"
local UIUtils = require "app.views.UIUtils"

local Spine = class("Spine", PopupLayer)

function Spine:ctor()
	PopupLayer.ctor(self)
	self:init()
end

function Spine:dtor()

end

function Spine:init()

	self.m_root,self.m_aniManager = UIUtils:getRootNodeInCreatorEx(self,"creator/scenes/spine/spine.ccreator")


--[[	self.m_skeletonNode = sp.SkeletonAnimation:create("creator/scenes/spine/spineboy-pro.json", "creator/scenes/spine/spineboy-pro.atlas", 1.0)

	self.m_skeletonNode:setPosition(cc.p(400, 80))
	self:addChild(self.m_skeletonNode)
	self.m_skeletonNode:setAnimation(0, "walk", true)
	self.m_skeletonNode:update(0)
	]]

end




return Spine
