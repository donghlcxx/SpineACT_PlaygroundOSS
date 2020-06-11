                           Klab PlaygroundOSS集成Spine动画并生成ACT游戏
						   ===

 1、游戏玩法说明
 ---

    *第一个场景展示Spine集成，每次点击屏幕将会改变主角动作；双击进入战斗场景

    *第二个战斗场景，单击开火，目标击毙对方；

 2、文件构成
 ---
    *游戏项目路径 /trunk/ActGame<br>  
                 /trunk/ActGame/Spine            ----游戏主角的spine动画Atlas及json文件<br>  
                 /trunk/ActGame/texAnimation     ----游戏中对手序列帧动画图片<br>  
                 /trunk/ActGame/Canvas.lua       ----游戏主逻辑<br>  
                 /trunk/ActGame/JianZhunYuan.ttf ----游戏字体<br>  
    *游戏视频路径 /trunk/show.mp4
    *Spine插件 /trunk/Engine/source/spine        --Spine插件及接口文件路径<br>  
               /trunk/Engine/source/spine/spineInterface.h     --spine接口<br>  
               /trunk/Engine/source/spine/spineInterface.cpp   --spine接口<br>  
			   
 3 、引擎扩充方法
 ---
     *UI_Canvas 命令UI_CANVAS_ADDRESOURCE增加带3参数方法
      sysCommand(CanvasBoy, UI_CANVAS_ADDRESOURCE, "asset://spineboy.png.imag",           --spine 图片数据<br>  
	                                               "asset://spine/spineboy.atlas",        ---参数为spineatlas<br>  
	                                               "asset://spine/spineboy-pro.json")	  -- 参数为spine json <br>  
	 *UI_Canvas 增加了UI_CANVAS_CHANGERANIMATION切换动画命令接口
	  sysCommand(CanvasBoy,                                    --canvas handle<br>  
	             UI_CANVAS_CHANGERANIMATION,                   --改变动画命令<br>  
				 0,                                            --spine动画的索引<br>  
				 "idle",                                       --spine动画的名称<br>  
				 true)                                         --spine动画是否loop<br>  

4、在开发过程中遇到的问
---
   1、Spine版本与引擎兼容问题
   --
      尝试过spine多个版本，最终用spine3.8 C++版本集成进引擎。修改了引擎的CKLBUICanvas.h、<br>  
      CKLBUICanvas.cpp来集成，增加了UI_CANVAS_CHANGERANIMATION切换动画命令接口<br>  
   2、Spine动画数据渲染的方式和PlaygroundOSS引擎有些不同，<br>  
   --
      比如：uv，需要做uv的镜像处理等问题<br>  
   