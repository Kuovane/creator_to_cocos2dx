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
#include "lua_creator_reader_manual.hpp"

#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"
#include "scripting/lua-bindings/manual/cocos2d/LuaScriptHandlerMgr.h"
#include "scripting/lua-bindings/manual/CCLuaValue.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "base/CCEventListenerFocus.h"

#include "collider/ColliderManager.h"
#include "AnimationManager.h"
#include "ui/PageView.h"
#include "ui/RadioButton.h"
#include "ui/RadioButtonGroup.h"
#include "ui/Button.h"
#include "ui/Layout.h"
#include "ui/RichText.h"
#include "ui/WidgetExport.h"
#include "CreatorReader.h"



static int lua_ColliderManager_unregisterCollisionCallback(lua_State* L)
{
    if (nullptr == L)
        return 0;
    
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"creator.ColliderManager",0,&tolua_err))
    {
        tolua_error(L,"'lua_ColliderManager_unregisterCollisionCallback' is not executed in ColliderManager\n", NULL);
        return 0;
    }

    creator::ColliderManager* self = static_cast<creator::ColliderManager*>(tolua_tousertype(L,1,0));
    
    int argc = lua_gettop(L) - 1;
    if (1 == argc)
    {
        if (!lua_isstring(L, 1))
        {
            tolua_error(L, "parameter 1 is not string\n", NULL);
            return 0;
        }
        
        const char* key = lua_tostring(L, 1);
        self->unregisterCollisionCallback(key);
    }
    else
        luaL_error(L, "'unregisterCollisionCallback' function of ColliderManager has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    
    return 0;
}

static int lua_ColliderManager_registerCollitionCallback(lua_State* L)
{
    if (nullptr == L)
        return 0;
    
    tolua_Error tolua_err;
    if (!tolua_isusertype(L,1,"creator.ColliderManager",0,&tolua_err))
    {
        tolua_error(L,"'registerCollitionCallback' is not executed with ColliderManager\n", NULL);
        return 0;
    }
    
    auto self = static_cast<creator::ColliderManager*>(tolua_tousertype(L,1,0));
    
    int argc = lua_gettop(L) - 1;
    if (2 == argc)
    {
        if (!toluafix_isfunction(L,2,"LUA_FUNCTION",0,&tolua_err))
        {
            luaL_error(L, "'registerCollitionCallback' first parameter is not a function\n");
            return 0;
        }
        if (!lua_isstring(L, 3))
        {
            luaL_error(L, "'registerCollitionCallback' second parameter is not a string\n");
            return 0;
        }
        
        LUA_FUNCTION handler = (  toluafix_ref_function(L,2,0));
        const char* key = lua_tostring(L, 3);
        
        self->registerCollitionCallback([=](creator::Contract::CollisionType type, creator::Collider* collider1, creator::Collider* collider2)
        {
            auto stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
            stack->pushInt(static_cast<int>(type));
            stack->pushObject(collider1, "creator.Collider");
            stack->pushObject(collider2, "creator.Collider");
            stack->executeFunctionByHandler(handler, 3);
            stack->clean();
        }, key);
        
        ScriptHandlerMgr::getInstance()->addCustomHandler((void*)self, handler);
    }
    else
        luaL_error(L, "'addClickEventListener' function of ColliderManager has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    
    return 0;
}

static void extendColliderManager(lua_State* L)
{
	lua_pushstring(L, "creator.ColliderManager");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "registerCollitionCallback", lua_ColliderManager_registerCollitionCallback);
		tolua_function(L, "unregisterCollisionCallback", lua_ColliderManager_unregisterCollisionCallback);
	}
	lua_pop(L, 1);
}



int lua_creator_reader_AnimationManager_playAnimationClip(lua_State* tolua_S)
{
	int argc = 0;
	creator::AnimationManager* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "creator.AnimationManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (creator::AnimationManager*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_creator_reader_AnimationManager_playAnimationClip'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 2)
	{
		cocos2d::Node* arg0;
		std::string arg1;

		ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node", &arg0, "creator.AnimationManager:playAnimationClip");

		ok &= luaval_to_std_string(tolua_S, 3, &arg1, "creator.AnimationManager:playAnimationClip");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_creator_reader_AnimationManager_playAnimationClip'", nullptr);
			return 0;
		}
		cobj->playAnimationClip(arg0, arg1);
		lua_settop(tolua_S, 1);
		return 1;
	}

	else if (argc == 3)
	{



		cocos2d::Node* arg0;
		std::string arg1;

		ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node", &arg0, "creator.AnimationManager:playAnimationClip");

		ok &= luaval_to_std_string(tolua_S, 3, &arg1, "creator.AnimationManager:playAnimationClip");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_creator_reader_AnimationManager_playAnimationClip'", nullptr);
			return 0;
		}


		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 4, 0));


		auto func = [=]()
		{
			auto stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
			stack->executeFunctionByHandler(handler, 0);
			stack->clean();
		};

		ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);

		cobj->playAnimationClip(arg0, arg1, func);

		lua_settop(tolua_S, 1);
		return 1;

	}
	else if (argc == 4)
	{
		cocos2d::Node* arg0;
		std::string arg1;

		ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node", &arg0, "creator.AnimationManager:playAnimationClip");

		ok &= luaval_to_std_string(tolua_S, 3, &arg1, "creator.AnimationManager:playAnimationClip");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_creator_reader_AnimationManager_playAnimationClip'", nullptr);
			return 0;
		}


		LUA_FUNCTION handler = (toluafix_ref_function(tolua_S, 4, 0));
		int  nModeType = lua_tointeger(tolua_S, 5);

		if (handler)
		{
			auto func = [=]()
			{
				auto stack = cocos2d::LuaEngine::getInstance()->getLuaStack();
				stack->executeFunctionByHandler(handler, 0);
				stack->clean();
			};

			ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);

			cobj->playAnimationClip(arg0, arg1, func, nModeType);
		}
		else
		{

			cobj->playAnimationClip(arg0, arg1, nullptr, nModeType);
		}


		lua_settop(tolua_S, 1);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "creator.AnimationManager:playAnimationClip", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_creator_reader_AnimationManager_playAnimationClip'.", &tolua_err);
#endif

				return 0;
}


int lua_creator_reader_AnimationManager_stopAllAnimationClips(lua_State* tolua_S)
{
	int argc = 0;
	creator::AnimationManager* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "creator.AnimationManager", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (creator::AnimationManager*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_creator_reader_AnimationManager_stopAllAnimationClips'", nullptr);
		return 0;
	}
#endif

	
	cobj->stopAllAnimationClips();
	lua_settop(tolua_S, 1);
	return 1;
	

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_creator_reader_AnimationManager_stopAllAnimationClips'.", &tolua_err);
#endif

				return 0;
}


static void extendAnimationManager(lua_State* L)
{
	lua_pushstring(L, "creator.AnimationManager");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "playAnimationClip", lua_creator_reader_AnimationManager_playAnimationClip);
		tolua_function(L, "stopAllAnimationClips", lua_creator_reader_AnimationManager_stopAllAnimationClips);
		
	
	}
	lua_pop(L, 1);
}


static int lua_register_creator_reader_pageView(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "creator.CreatorPageView");
	tolua_cclass(tolua_S, "CreatorPageView", "creator.CreatorPageView", "ccui.PageView", nullptr);

	tolua_beginmodule(tolua_S, "CreatorPageView");

	tolua_endmodule(tolua_S);
	std::string typeName = typeid(creator::CreatorPageView).name();
	g_luaType[typeName] = "creator.CreatorPageView";
	g_typeCast["CreatorPageView"] = "creator.CreatorPageView";
	return 1;
}





static int lua_Button_create(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "creator.CreatorButton", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
		std::string sBG = lua_tostring(tolua_S, 2);

		//std::string sSelect = lua_tostring(tolua_S, 3);

		creator::CreatorButton* ret = creator::CreatorButton::create(sBG);
		object_to_luaval<creator::CreatorButton>(tolua_S, "creator.CreatorButton", (creator::CreatorButton*)ret);
		return 1;
	}


	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "creator.CreatorButton:create", argc, 2);
	return 0;
#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_Button_create'.", &tolua_err);
#endif
				return 0;
}


static int lua_Button_addRefrenceChild(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1

	if (!tolua_isusertype(tolua_S, 1, "creator.CreatorButton", 0, &tolua_err))
	{
		tolua_error(tolua_S, "'addRefrenceChild' is not executed with CreatorButton\n", NULL);
		return 0;
	}

#endif
	auto self = static_cast<creator::CreatorButton*>(tolua_tousertype(tolua_S, 1, 0));
	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{

		cocos2d::Node* arg0;
		ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node", &arg0, "creator.CreatorButton:addRefrenceChild");

		self->addRefrenceChild(arg0);

		lua_settop(tolua_S, 1);
		return 1;
	}


	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "creator.CreatorButton:addRefrenceChild", argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_Button_addRefrenceChild'.", &tolua_err);
#endif
				return 0;
}


static int lua_Button_removeRefrenceChild(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1

	if (!tolua_isusertype(tolua_S, 1, "creator.CreatorButton", 0, &tolua_err))
	{
		tolua_error(tolua_S, "'removeRefrenceChild' is not executed with CreatorButton\n", NULL);
		return 0;
	}

#endif
	auto self = static_cast<creator::CreatorButton*>(tolua_tousertype(tolua_S, 1, 0));
	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{

		cocos2d::Node* arg0;
		ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node", &arg0, "creator.CreatorButton:removeRefrenceChild");

		self->removeRefrenceChild(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}


	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "creator.CreatorButton:removeRefrenceChild", argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_Button_removeRefrenceChild'.", &tolua_err);
#endif
				return 0;
}

static int lua_Button_enableAutoGrayEffect(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1

	if (!tolua_isusertype(tolua_S, 1, "creator.CreatorButton", 0, &tolua_err))
	{
		tolua_error(tolua_S, "'enableAutoGrayEffect' is not executed with CreatorButton\n", NULL);
		return 0;
	}

#endif
	auto self = static_cast<creator::CreatorButton*>(tolua_tousertype(tolua_S, 1, 0));
	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
		bool arg0 = false;
		ok &= luaval_to_boolean(tolua_S, 2, &arg0, "creator.CreatorButton:enableAutoGrayEffect");

		self->enableAutoGrayEffect(arg0);
		lua_settop(tolua_S, 1);
		return 1;
	}


	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "creator.CreatorButton:enableAutoGrayEffect", argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_Button_enableAutoGrayEffect'.", &tolua_err);
#endif
				return 0;
}


static int lua_Button_setTransitionType(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;


	auto self = static_cast<creator::CreatorButton*>(tolua_tousertype(tolua_S, 1, 0));
	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
		int type = lua_tointeger(tolua_S, 2);
		self->setTransitionType((creator::CreatorButton::TransitionType)type);
		lua_settop(tolua_S, 1);
		return 1;
	}


	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "creator.CreatorButton:setTransitionType", argc, 1);
	return 0;

}



static int lua_register_creator_reader_button(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "creator.CreatorButton");
	tolua_cclass(tolua_S, "CreatorButton", "creator.CreatorButton", "ccui.Button", nullptr);

	tolua_beginmodule(tolua_S, "CreatorButton");
		tolua_function(tolua_S, "create", lua_Button_create);
		tolua_function(tolua_S, "addRefrenceChild", lua_Button_addRefrenceChild);
		tolua_function(tolua_S, "removeRefrenceChild", lua_Button_removeRefrenceChild);
		tolua_function(tolua_S, "enableAutoGrayEffect", lua_Button_enableAutoGrayEffect);
		tolua_function(tolua_S, "setTransitionType", lua_Button_setTransitionType);
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(creator::CreatorButton).name();
	g_luaType[typeName] = "creator.CreatorButton";
	g_typeCast["CreatorButton"] = "creator.CreatorButton";
	return 1;
}







static int lua_radioButton_create(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "creator.CreatorRadioButton", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 2)
	{
		std::string sBG = lua_tostring(tolua_S, 2);

		std::string sSelect = lua_tostring(tolua_S, 3);

		creator::CreatorRadioButton* ret = creator::CreatorRadioButton::create(sBG,sSelect);
		object_to_luaval<creator::CreatorRadioButton>(tolua_S, "creator.CreatorRadioButton", (creator::CreatorRadioButton*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "creator.CreatorRadioButton:create", argc, 2);
	return 0;
#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_radioButton_create'.", &tolua_err);
#endif
				return 0;
}




static int lua_radioButton_setToggleGroup(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1

	if (!tolua_isusertype(tolua_S, 1, "creator.CreatorRadioButton", 0, &tolua_err))
	{
		tolua_error(tolua_S, "'setToggleGroup' is not executed with CreatorRadioButton\n", NULL);
		return 0;
	}

#endif

	auto self = static_cast<creator::CreatorRadioButton*>(tolua_tousertype(tolua_S, 1, 0));

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 1)
	{
		bool b = lua_toboolean(tolua_S,2);// lua_tob(tolua_S, 2);

		self->setToggleGroup(b);

		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "creator.CreatorRadioButton:setToggleGroup", argc, 1);
	return 0;
#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_radioButton_setToggleGroup'.", &tolua_err);
#endif
				return 0;
}




static int lua_register_creator_reader_radioButton(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "creator.CreatorRadioButton");
	tolua_cclass(tolua_S, "CreatorRadioButton", "creator.CreatorRadioButton", "ccui.RadioButton", nullptr);

	tolua_beginmodule(tolua_S, "CreatorRadioButton");
		tolua_function(tolua_S, "create", lua_radioButton_create);
		tolua_function(tolua_S, "setToggleGroup", lua_radioButton_setToggleGroup);
	tolua_endmodule(tolua_S);

	std::string typeName = typeid(creator::CreatorRadioButton).name();
	g_luaType[typeName] = "creator.CreatorRadioButton";
	g_typeCast["CreatorRadioButton"] = "creator.CreatorRadioButton";
	return 1;
}

static int lua_register_creator_reader_radioButtonGroup(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "creator.CreatorRadioButtonGroup");
	tolua_cclass(tolua_S, "CreatorRadioButtonGroup", "creator.CreatorRadioButtonGroup", "ccui.RadioButtonGroup", nullptr);

	tolua_beginmodule(tolua_S, "CreatorRadioButtonGroup");

	tolua_endmodule(tolua_S);
	std::string typeName = typeid(creator::CreatorRadioButtonGroup).name();
	g_luaType[typeName] = "creator.CreatorRadioButtonGroup";
	g_typeCast["CreatorRadioButtonGroup"] = "creator.CreatorRadioButtonGroup";
	return 1;
}





static int lua_Layout_create(lua_State* tolua_S)
{
	int argc = 0;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S, 1, "creator.CreatorLayout", 0, &tolua_err)) goto tolua_lerror;
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (argc == 0)
	{
		
		creator::CreatorLayout* ret = creator::CreatorLayout::create();
		object_to_luaval<creator::CreatorLayout>(tolua_S, "creator.CreatorLayout", (creator::CreatorLayout*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "creator.CreatorLayout:create", argc, 0);
	return 0;
#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_Layout_create'.", &tolua_err);
#endif
				return 0;
}

static int lua_Layout_setSpacingX(lua_State* tolua_S)
{
    int argc = 0;
    creator::CreatorLayout* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"creator.CreatorLayout",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (creator::CreatorLayout*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_Layout_setSpacingX'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        double arg0;
        ok &= luaval_to_number(tolua_S, 2,&arg0, "creator.CreatorLayout:setSpacingX");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_Layout_setSpacingX'", nullptr);
            return 0;
        }
        cobj->setSpacingX(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "creator.CreatorLayout:setSpacingX",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_Layout_setSpacingX'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_Layout_setSpacingY(lua_State* tolua_S)
{
    int argc = 0;
    creator::CreatorLayout* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"creator.CreatorLayout",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (creator::CreatorLayout*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_Layout_setSpacingY'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        double arg0;
        ok &= luaval_to_number(tolua_S, 2,&arg0, "creator.CreatorLayout:setSpacingY");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_Layout_setSpacingY'", nullptr);
            return 0;
        }
        cobj->setSpacingY(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "creator.CreatorLayout:setSpacingY",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_Layout_setSpacingY'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_Layout_setPaddingTop(lua_State* tolua_S)
{
    int argc = 0;
    creator::CreatorLayout* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"creator.CreatorLayout",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (creator::CreatorLayout*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_Layout_setPaddingTop'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        double arg0;
        ok &= luaval_to_number(tolua_S, 2,&arg0, "creator.CreatorLayout:setPaddingTop");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_Layout_setPaddingTop'", nullptr);
            return 0;
        }
        cobj->setPaddingTop(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "creator.CreatorLayout:setPaddingTop",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_Layout_setPaddingTop'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_Layout_setPaddingBottom(lua_State* tolua_S)
{
    int argc = 0;
    creator::CreatorLayout* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"creator.CreatorLayout",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (creator::CreatorLayout*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_Layout_setPaddingBottom'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        double arg0;
        ok &= luaval_to_number(tolua_S, 2, &arg0, "creator.CreatorLayout:setPaddingBottom");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_Layout_setPaddingBottom'", nullptr);
            return 0;
        }
        cobj->setPaddingBottom(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "creator.CreatorLayout:setPaddingBottom",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_Layout_setPaddingBottom'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_Layout_setPaddingLeft(lua_State* tolua_S)
{
    int argc = 0;
    creator::CreatorLayout* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"creator.CreatorLayout",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (creator::CreatorLayout*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_Layout_setPaddingLeft'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        double arg0;
        ok &= luaval_to_number(tolua_S, 2, &arg0, "creator.CreatorLayout:setPaddingLeft");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_Layout_setPaddingLeft'", nullptr);
            return 0;
        }
        cobj->setPaddingLeft(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "creator.CreatorLayout:setPaddingLeft",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_Layout_setPaddingLeft'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_Layout_setPaddingRight(lua_State* tolua_S)
{
    int argc = 0;
    creator::CreatorLayout* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"creator.CreatorLayout",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (creator::CreatorLayout*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_Layout_setPaddingRight'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        double arg0;
        ok &= luaval_to_number(tolua_S, 2, &arg0, "creator.CreatorLayout:setPaddingRight");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_Layout_setPaddingRight'", nullptr);
            return 0;
        }
        cobj->setPaddingRight(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "creator.CreatorLayout:setPaddingRight",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_Layout_setPaddingRight'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_register_creator_reader_layout(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "creator.CreatorLayout");
	tolua_cclass(tolua_S, "CreatorLayout", "creator.CreatorLayout", "ccui.Layout", nullptr);

	tolua_beginmodule(tolua_S, "CreatorLayout");
	tolua_function(tolua_S, "create", lua_Layout_create);
    tolua_function(tolua_S, "setSpacingX", lua_Layout_setSpacingX);
    tolua_function(tolua_S, "setSpacingY", lua_Layout_setSpacingY);
    tolua_function(tolua_S, "setPaddingTop", lua_Layout_setPaddingTop);
    tolua_function(tolua_S, "setPaddingBottom", lua_Layout_setPaddingBottom);
    tolua_function(tolua_S, "setPaddingLeft", lua_Layout_setPaddingLeft);
    tolua_function(tolua_S, "setPaddingRight", lua_Layout_setPaddingRight);
    
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(creator::CreatorLayout).name();
	g_luaType[typeName] = "creator.CreatorLayout";
	g_typeCast["CreatorLayout"] = "creator.CreatorLayout";
	return 1;
}

static int lua_richText_create(lua_State* tolua_S)
{
		int argc = 0;
		bool ok = true;

#if COCOS2D_DEBUG >= 1
		tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
		if (!tolua_isusertable(tolua_S, 1, "creator.CreatorRichText", 0, &tolua_err)) goto tolua_lerror;
#endif

		argc = lua_gettop(tolua_S) - 1;

		if (argc == 0)
		{

			creator::CreatorRichText* ret = creator::CreatorRichText::create();
			object_to_luaval<creator::CreatorRichText>(tolua_S, "creator.CreatorRichText", (creator::CreatorRichText*)ret);
			return 1;
		}
		luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "creator.CreatorRichText:create", argc, 0);
		return 0;
#if COCOS2D_DEBUG >= 1
		tolua_lerror:
					tolua_error(tolua_S, "#ferror in function 'lua_richText_create'.", &tolua_err);
#endif
					return 0;
	}
static int lua_richText_removeAllElements(lua_State* L)
{


	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "creator.CreatorRichText", 0, &tolua_err))
	{
		tolua_error(L, "'removeAllElements' is not executed with CreatorRichText\n", NULL);
		return 0;
	}

	auto self = static_cast<creator::CreatorRichText*>(tolua_tousertype(L, 1, 0));

	int argc = lua_gettop(L) - 1;
	if (0 == argc)
	{
		self->removeAllElements();
	}
	else
		luaL_error(L, "'lua_richText_removeAllElements' function of CreatorRichText has wrong number of arguments: %d, was expecting %d\n", argc, 0);

	return 0;
}

static int lua_richText_setElementsAnchorPoint(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "creator.CreatorRichText", 0, &tolua_err))
	{
		tolua_error(L, "'setElementsAnchorPoint' is not executed with CreatorRichText\n", NULL);
		return 0;
	}

	auto self = static_cast<creator::CreatorRichText*>(tolua_tousertype(L, 1, 0));

	int argc = lua_gettop(L) - 1;
	if (argc == 1)
	{
		cocos2d::Vec2 arg0;
		bool ok = false;
		ok = luaval_to_vec2(L, 2, &arg0, "creator.CreatorRichText:setElementsAnchorPoint");
		if (!ok)
		{
			tolua_error(L, "invalid arguments in function 'lua_richText_setElementsAnchorPoint'", nullptr);
			return 0;
		}
		self->setElementsAnchorPoint(arg0);
		lua_settop(L, 1);
		return 1;
	}
	else
		luaL_error(L, "'lua_richText_setElementsAnchorPoint' function of CreatorRichText has wrong number of arguments: %d, was expecting %d\n", argc, 1);

	return 0;
}






static int lua_richText_getElement(lua_State* L)
{
	if (nullptr == L)
		return 0;

	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "creator.CreatorRichText", 0, &tolua_err))
	{
		tolua_error(L, "'getElement' is not executed with CreatorRichText\n", NULL);
		return 0;
	}

	auto self = static_cast<creator::CreatorRichText*>(tolua_tousertype(L, 1, 0));

	int argc = lua_gettop(L) - 1;
	if (1 == argc)
	{
		
		if (!lua_isnumber(L, 2))
		{
			luaL_error(L, "'getElement' first parameter is not a number\n");
			return 0;
		}


		int index = lua_tointeger(L, 2);

		auto pElement = self->getElement(index);

		auto pElementText = dynamic_cast<ui::RichElementText*>(pElement);
		if (pElementText)
		{

			object_to_luaval<cocos2d::ui::RichElementText>(L, "ccui.RichElementText", (cocos2d::ui::RichElementText*)pElementText);
			return 1;
		}

		auto pElementImage= dynamic_cast<ui::RichElementImage*>(pElement);
		if (pElementImage)
		{

			object_to_luaval<cocos2d::ui::RichElementImage>(L, "ccui.RichElementImage", (cocos2d::ui::RichElementImage*)pElementImage);
			return 1;
		}

		auto pElementNewLine = dynamic_cast<ui::RichElementNewLine*>(pElement);
		if (pElementNewLine)
		{

			object_to_luaval<cocos2d::ui::RichElementNewLine>(L, "ccui.RichElementNewLine", (cocos2d::ui::RichElementNewLine*)pElementNewLine);
			return 1;
		}

		auto pElementNode = dynamic_cast<ui::RichElementCustomNode*>(pElement);
		if (pElementNode)
		{

			object_to_luaval<cocos2d::ui::RichElementCustomNode>(L, "ccui.RichElementCustomNode", (cocos2d::ui::RichElementCustomNode*)pElementNode);
			return 1;
		}


	}
	else
		luaL_error(L, "'getElement' function of CreatorRichText has wrong number of arguments: %d, was expecting %d\n", argc, 1);

	return 0;
}


static int lua_richText_setElementText(lua_State* L)
{
	if (nullptr == L)
		return 0;

	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "creator.CreatorRichText", 0, &tolua_err))
	{
		tolua_error(L, "'setElementText' is not executed with CreatorRichText\n", NULL);
		return 0;
	}

	auto self = static_cast<creator::CreatorRichText*>(tolua_tousertype(L, 1, 0));

	int argc = lua_gettop(L) - 1;
	if (2 == argc)
	{

		if (!lua_isnumber(L, 2))
		{
			luaL_error(L, "'setElementText' first parameter is not a number\n");
			return 0;
		}

		if (!lua_isstring(L, 3))
		{
			luaL_error(L, "'setElementText' second parameter is not a string\n");
			return 0;
		}

		int index = lua_tointeger(L, 2);
		std::string sText = lua_tostring(L, 3);


		self->setElementText(index, sText);
		lua_settop(L, 1);
		return 1;
		


	}
	else
		luaL_error(L, "'getElement' function of CreatorRichText has wrong number of arguments: %d, was expecting %d\n", argc, 2);

	return 0;
}


static int lua_richText_setXMLData(lua_State* L)
{
	if (nullptr == L)
		return 0;

	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "creator.CreatorRichText", 0, &tolua_err))
	{
		tolua_error(L, "'setXMLData' is not executed with CreatorRichText\n", NULL);
		return 0;
	}

	auto self = static_cast<creator::CreatorRichText*>(tolua_tousertype(L, 1, 0));

	int argc = lua_gettop(L) - 1;
	if (1 == argc)
	{
		if (!lua_isstring(L, 2))
		{
			luaL_error(L, "'setXMLData' first parameter is not a string\n");
			return 0;
		}

	
		std::string sText = lua_tostring(L, 2);


		self->setXMLData( sText);
		lua_settop(L, 1);
		return 1;

	}
	else
		luaL_error(L, "'getElement' function of CreatorRichText has wrong number of arguments: %d, was expecting %d\n", argc, 1);

	return 0;
}




static int lua_richText_getElementCount(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "creator.CreatorRichText", 0, &tolua_err))
	{
		tolua_error(L, "'getElement' is not executed with CreatorRichText\n", NULL);
		return 0;
	}

	auto self = static_cast<creator::CreatorRichText*>(tolua_tousertype(L, 1, 0));
	auto count = self->getElementCount();

	lua_pushinteger(L, count);
	return 1;
}


static int lua_richText_flipElements(lua_State* L)
{
	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "creator.CreatorRichText", 0, &tolua_err))
	{
		tolua_error(L, "'getElement' is not executed with CreatorRichText\n", NULL);
		return 0;
	}

	auto self = static_cast<creator::CreatorRichText*>(tolua_tousertype(L, 1, 0));

	self->flipElements();

	lua_settop(L, 1);
	return 1;
}


static int lua_register_creator_reader_richText(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "creator.CreatorRichText");
	tolua_cclass(tolua_S, "CreatorRichText", "creator.CreatorRichText", "ccui.RichText", nullptr);

	tolua_beginmodule(tolua_S, "CreatorRichText");
	tolua_function(tolua_S, "create", lua_richText_create);
	tolua_function(tolua_S, "removeAllElements", lua_richText_removeAllElements);
	tolua_function(tolua_S, "setElementsAnchorPoint", lua_richText_setElementsAnchorPoint);

	
	tolua_function(tolua_S, "getElement", lua_richText_getElement);
	tolua_function(tolua_S, "setElementText", lua_richText_setElementText);
	tolua_function(tolua_S, "setXMLData", lua_richText_setXMLData);
	tolua_function(tolua_S, "getElementCount", lua_richText_getElementCount);
	tolua_function(tolua_S, "flipElements", lua_richText_flipElements);
	
	
	tolua_endmodule(tolua_S);
	std::string typeName = typeid(creator::CreatorRichText).name();
	g_luaType[typeName] = "creator.CreatorRichText";
	g_typeCast["CreatorRichText"] = "creator.CreatorRichText";
	return 1;
}



static int lua_richText_forceDoAlign(lua_State* L)
{
	if (nullptr == L)
		return 0;

	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "creator.WidgetManager", 0, &tolua_err))
	{
		tolua_error(L, "'forceDoAlign' is not executed with WidgetManager\n", NULL);
		return 0;
	}

	auto self = static_cast<creator::WidgetManager*>(tolua_tousertype(L, 1, 0));

	int argc = lua_gettop(L) - 1;
	if (0 == argc)
	{
		self->forceDoAlign();
	}
	else
		luaL_error(L, "'getElement' function of WidgetManager has wrong number of arguments: %d, was expecting %d\n", argc, 0);

	return 0;
}

static int lua_WidgetManager_forceDoAlignAssignNode(lua_State* L)
{
	if (nullptr == L)
		return 0;

	tolua_Error tolua_err;
	if (!tolua_isusertype(L, 1, "creator.WidgetManager", 0, &tolua_err))
	{
		tolua_error(L, "'forceDoAlignAssignNode' is not executed with WidgetManager\n", NULL);
		return 0;
	}

	auto self = static_cast<creator::WidgetManager*>(tolua_tousertype(L, 1, 0));

	int argc = lua_gettop(L) - 1;
	if (1 == argc)
	{

		cocos2d::Node* arg0;
		bool ok = true;
		ok &= luaval_to_object<cocos2d::Node>(L, 2, "cc.Node", &arg0, "creator.WidgetManager:forceDoAlignAssignNode");

		if (!ok || nullptr == arg0)
		{
			tolua_error(L, "invalid arguments in function 'lua_WidgetManager_forceDoAlignAssignNode'", nullptr);
			return 0;
		}

		self->forceDoAlignAssignNode(arg0);

		return 1;

	}
	else
		luaL_error(L, "'forceDoAlignAssignNode' function of WidgetManager has wrong number of arguments: %d, was expecting %d\n", argc, 1);

	return 0;
}

static int lua_register_creator_reader_widgetManager(lua_State* tolua_S)
{
	tolua_usertype(tolua_S, "creator.WidgetManager");
	tolua_cclass(tolua_S, "WidgetManager", "creator.WidgetManager", "cc.Node", nullptr);

	tolua_beginmodule(tolua_S, "WidgetManager");
	tolua_function(tolua_S, "forceDoAlign", lua_richText_forceDoAlign);
	tolua_function(tolua_S, "forceDoAlignAssignNode", lua_WidgetManager_forceDoAlignAssignNode);
	


	tolua_endmodule(tolua_S);
	std::string typeName = typeid(creator::WidgetManager).name();
	g_luaType[typeName] = "creator.WidgetManager";
	g_typeCast["WidgetManager"] = "creator.WidgetManager";
	return 1;
}




int lua_creator_reader_getWidgetManager(lua_State* tolua_S)
{
	int argc = 0;
	creator::CreatorReader* cobj = nullptr;
	bool ok = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S, 1, "creator.CreatorReader", 0, &tolua_err)) goto tolua_lerror;
#endif

	cobj = (creator::CreatorReader*)tolua_tousertype(tolua_S, 1, 0);

#if COCOS2D_DEBUG >= 1
	if (!cobj)
	{
		tolua_error(tolua_S, "invalid 'cobj' in function 'lua_creator_reader_CreatorReader_getWidgetManager'", nullptr);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;
	if (argc == 0)
	{
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_creator_reader_CreatorReader_getWidgetManager'", nullptr);
			return 0;
		}
		creator::WidgetManager* ret = cobj->getWidgetManager();
		object_to_luaval<creator::WidgetManager>(tolua_S, "creator.WidgetManager", (creator::WidgetManager*)ret);
		return 1;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "creator.CreatorReader:getWidgetManager", argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
	tolua_lerror:
				tolua_error(tolua_S, "#ferror in function 'lua_creator_reader_CreatorReader_getWidgetManager'.", &tolua_err);
#endif

				return 0;
}


int lua_creator_reader_resetSpriteFrames(lua_State* tolua_S)
{
	int argc = 0;
	creator::CreatorReader* cobj = nullptr;
	bool ok = true;

	if (argc == 0)
	{
		creator::CreatorReader::resetSpriteFrames();
		return 0;
	}
	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "creator.CreatorReader:resetSpriteFrames", argc, 0);
	return 0;
}



static void extendReader(lua_State* L)
{
	lua_pushstring(L, "creator.CreatorReader");
	lua_rawget(L, LUA_REGISTRYINDEX);
	if (lua_istable(L, -1))
	{
		tolua_function(L, "getWidgetManager", lua_creator_reader_getWidgetManager);
		tolua_function(L, "resetSpriteFrames", lua_creator_reader_resetSpriteFrames);
		
	}
	lua_pop(L, 1);
}



int register_all_creator_reader_manual(lua_State* L)
{
    if (nullptr == L)
        return 0;
	
	lua_register_creator_reader_pageView(L);
	lua_register_creator_reader_button(L);
	lua_register_creator_reader_radioButton(L);
	lua_register_creator_reader_radioButtonGroup(L);
	lua_register_creator_reader_layout(L);
	lua_register_creator_reader_richText(L);
	lua_register_creator_reader_widgetManager(L);

    extendColliderManager(L);
	extendAnimationManager(L);

	extendReader(L);
    return 0;
}
