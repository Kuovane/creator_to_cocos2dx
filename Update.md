# 版本支持
 - 目前支持 1.93，2.0.9，2.0.10，2.3.0 等版本的大部分控件

# 使用操作

1，拉取代码

2，运行 Hello world 目录下的 mklink.bat，创建软链接

3，运行 simulator/win32 目录下 HelloWorld.exe


# 各个控件的支持情况
## 1，插件导出资源
 - 拷贝所有资源文件到项目，不管是否在编辑器中用到
 - 优化了 Fire 文件没有更改就不需要拷贝过去
 - 修复部分因为找不到 uuid 而 crash 的问题

## 2，Label
bugfixed：
  * 因 Line height 影响位置问题

新支持 feat:
  - CLAMP 裁剪，超出宽高将不显示
  - Enable Wrap 自动换行
  - SHRINK 自动根据节点约束框缩放大小
  - RESIZE_HEIGHT 根据文本自动更新 height

## 3，Sprite
bugfixed：
  - 因图片四周有透明裁剪而引起的位置问题
  - 选择 SLICED 时，不管有没勾选 Trim 项，都会裁剪透明部分，修复此设置引起的位置问题
  
新支持 feat：
  - Trim 裁剪周围透明部分，或者不裁剪都支持
  - SLICED 九宫格模式
  - TILED 平铺原始大小图片

暂不支持：
  - FILLED 模式，计划支持


## 4，Button
bugfixed：
  - 因图片四周有透明裁剪而引起的位置问题
  - 选择缩放模式时，不会缩放子节点问题

新支持
  -  COLOR 模式
  -  SCALE 模式的 duration
  -  SPRIT 模式
  -  enable Auto gray effect，就是禁用的时候自动变成灰色按钮
    
## 5，Toggle
bugFixed：
  - 多了一张选择的精灵，显示效果不一致
  - 没有选择背景target时，显示问题不一致
  - 不能有其它节点作为子树的问题
新支持：
    
  -  COLOR 模式
  -  SCALE 模式
  -  SPRIT 模式
  -  enable Auto gray effect
  这三种模式与按钮类似
    
使用例子
``` lua
  local toggle = NodeUtils:seekNodeByName(self.m_root,'New Toggle') 
  local func = function ( obj,eventType )
    print("toggle "..eventType)
  end
  toggle:addEventListener(func)
```
    
## 5，Toggle Group
    注意里面的 Toggle 是不为空的，是有 Toggle Group 且为其父节点，不然会有问题
    还有一点是要在编辑器中用旧版的
    
使用例子
``` lua
    local group = NodeUtils:seekNodeByName(self.m_root,'New ToggleGroup') 
  local func = function ( obj,index,eventType )
    print("testRedioButtonGroup "..index)
  end
  group:addEventListener(func)
```

## 6，Layout
    全面支持 Layout 的所有方式和参数

    特别注意每次更新完节点得调用 forceDoLayout() 方法重现计算

    新支持：
    -- Type:NONE
    -- Type:HORIZONAL 水平方向添加，不改变垂直方向的
    -- Type:VERTICAL  垂直方向添加，不改变水平方向的
    -- Type:GRID 水平和垂直方向都会添加，根据参数固定水平或者垂直方向的节点个数
    
    -- ResizeMode:NONE 不会更改本身的 size
    -- ResizeMode:CONTAINER  子节点的 size 可以大小不一样，会自动根据子节点的情况更改自身的 size
    -- ResizeMode:CHILDREN  子节点的 size 都一样，且会根据个数来缩放子节点大小，本身的size大小不变
    
   使用例子
``` lua
    local layout = NodeUtils:seekNodeByName(self.m_root,'New Layout') 

    -- 代码中添加或者移除节点后 需要调用 forceDoLayout 方法
  local sprite = cc.Sprite:create("welcome/HelloWorld.png")
  layout:addChild(sprite)

  layout:forceDoLayout()
    
```
    
## 7，RichText
    BugFixed：
    1，设置锚点不起作用的问题
    2，设置的 Max Width 不起作用问题
    3，设置 Line height 不起作用问题
    4，编辑器中直接换行不起作用问题
    
使用例子 1
``` lua
    local richText = NodeUtils:seekNodeByName(self.m_root,'New RichText') 
  richText:setElementText(0,"tst")
```

使用例子 2
``` lua
    local richText = NodeUtils:seekNodeByName(self.m_root,'New RichText') 
  richText:setElementText(0,"tst")

  local newElementLine = ccui.RichElementNewLine:create(10,ccc3(255,0,255),255)
  richText:pushBackElement(newElementLine)

  local newElement = ccui.RichElementText:create(10,ccc3(255,0,255),255,"添加的元素","res/creator/Texture/msyh.ttf",50)
  richText:pushBackElement(newElement)


  local newElement2 = ccui.RichElementText:create(10,ccc3(255,100,255),255,"添加到指定位置元素","res/creator/Texture/msyh.ttf",50)
  richText:insertElement(newElement2,0)
  richText:insertElement(newElementLine,1)
```

## 8，widget
支持 widget 所有组合方式
支持 百分比模式

## 9，AnimationManager
支持动画播放结束回调

使用例子



``` lua

local callback = function (  )
    print("playAnimationClip end**")
end

self.m_aniManager:playAnimationClip(self.m_table, "seatAnim",callback)

```
    
## 10，prefab
注意根节点得是node，如果用其它节点也会转成 node



# 常见问题

#### 一，导出时 Crash 报错了。
1，试试在那个报错的那个界面按 F5 重新导出
2，如果第一种方法不行，在编辑工程根目录下创建 temp 目录（如果没有的话）；
3，如果有 temp 目录，删除其目录下的 timeCreatorfiles，timefiles 这两个文件

#### 二，Toggle Group 不起作用
得用旧版的
右击某节点->创建节点->创建 UI 节点 -> 创建 Toggle Group（旧版单选按钮）

#### 三，创建的 fnt 文件在代码运行时不起作用
把 fnt 和 png 的名字改成一样，如 a.fnt 和 a_0.png 改成 a_0.fnt 和 a_0.png

#### 四，root 节点的作用

1，添加了 widget 的 root 节点 会根据屏幕大小而设置大小
2，未添加 widget 的 root 节点，不会根据屏幕大小而设置大小，但位置跟creator上所看到的的是不一样的，得注意一下

#### 五，怎么重新导出所有文件

在编辑器工程目录下，删除 temp 目录下的 timeCreatorfiles，timefiles 这两个文件，重新导出
