function setup()
	syslog("a")
    FONT_load("AlexBrush","asset://JianZhunYuan.ttf")
	pLableTitle = UI_Label 	(
							nil, 			-- <parent pointer>, 
							7001, 			-- <order>, 
							400,20,		-- <x>, <y>,
                            0xFF, 0xFFFFFF,	-- <alpha>, <rgb>, 
							"heiti",	-- "<font name>", 
							50,				-- <font size>, 
							"Spine 动画展示"	-- "<text string>"
						)
		pLableInfo1 = UI_Label 	(
							nil, 			-- <parent pointer>, 
							7001, 			-- <order>, 
							50,70,		-- <x>, <y>,
                            0xFF, 0xFFFFFF,	-- <alpha>, <rgb>, 
							"heiti",	-- "<font name>", 
							30,				-- <font size>, 
							"1、点击任意位置改变动画"	-- "<text string>"
						)
		pLableInfo2 = UI_Label 	(
							nil, 			-- <parent pointer>, 
							7001, 			-- <order>, 
							50,100,		-- <x>, <y>,
                            0xFF, 0xFFFFFF,	-- <alpha>, <rgb>, 
							"heiti",	-- "<font name>", 
							30,				-- <font size>, 
							"2、双击进入战斗"	-- "<text string>"
						)
   canvasBg = UI_Canvas(
		nil,
		5000,
		0,
		0,
		10,
		10,
		"drawfunc"
	)
    sysCommand(canvasBg, UI_CANVAS_ADDRESOURCE, "asset://level_39.png.imag")

    --int tank 
    CanvasTank = UI_VariableItem(nil,							-- arg[1]:		親となるUIタスクポインタ
								   8000,							-- arg[2]:		表示プライオリティ
								   0, 0,							-- arg[3,4]:	表示位置
								  128,128,						-- arg[5,6]:	表示サイズ
								"asset://texAnimation/idle/idle000.png.imag"	-- arg[7]:		表示assetのパス
							)
    --[[
	CanvasTank = UI_Canvas(
		nil,
		7000,
		20,
		20,
		2000,
		2000,
		"drawfunc"
	)
	--sysCommand(tskCanvas, UI_CANVAS_ADDRESOURCE, "asset://TileA.png.imag")	-- 0
	--sysCommand(tskCanvas, UI_CANVAS_ADDRESOURCE, "asset://TileB.png.imag")	-- 1
	--sysCommand(tskCanvas, UI_CANVAS_ADDRESOURCE, "asset://TileC.png.imag")	-- 2
	sysCommand(CanvasTank, UI_CANVAS_ADDRESOURCE, "asset://tank.png.imag",
	"asset://spine/tank.atlas",
	"asset://spine/tank-pro.json")
	]]
	--init spine boy
	CanvasBoy = UI_Canvas(
		nil,
		7000,
		20,
		20,
		2000,
		2000,
		"drawfunc"
	)
    sysCommand(CanvasBoy, UI_CANVAS_ADDRESOURCE, "asset://spineboy.png.imag",
	"asset://spine/spineboy.atlas",
	"asset://spine/spineboy-pro.json")	-- 2

   boylifeProcess = UI_ProgressBar(	nil,			-- 親UIタスクのポインタ。nilの場合は親の無い状態で生成されます。
							8000,			-- 表示プライオリティ: 表示システム上のプライオリティ値
							70, 280,		-- 表示座標: 親UIタスクの位置を基準として、相対座標で与えます。
							70, 10,		-- 表示サイズ: 幅と高さ。表示素材のサイズです。0[%] 状態の画像と100[%]状態の画像は同じサイズである必要があります。
							"asset://life_front.png.imag",	 -- 100[%]状態の画像asset名称
							"asset://life_back.png.imag", -- 0[%]状態の画像asset名称
							0,				-- 始点pixel位置: テクスチャ画像上において、実際に表示が変わるバーの始点となる座標 (※1)
							400,			-- 終点pixel位置: テクスチャ画像上において、実際に表示が変わるバーの終点となる座標 (※1)
							300,			-- アニメーション時間 (※2)
							false			-- 縦方向フラグ: バーの成長方向が垂直方向であればtrue, 水平方向であれば false (※1)
							--"asset://barfilter.png.imag"
							)
   tanklifeProcess = UI_ProgressBar(	nil,			-- 親UIタスクのポインタ。nilの場合は親の無い状態で生成されます。
							8000,			-- 表示プライオリティ: 表示システム上のプライオリティ値
							810, 520,		-- 表示座標: 親UIタスクの位置を基準として、相対座標で与えます。
							70, 10,		-- 表示サイズ: 幅と高さ。表示素材のサイズです。0[%] 状態の画像と100[%]状態の画像は同じサイズである必要があります。
							"asset://life_front.png.imag",	 -- 100[%]状態の画像asset名称
							"asset://life_back.png.imag", -- 0[%]状態の画像asset名称
							0,				-- 始点pixel位置: テクスチャ画像上において、実際に表示が変わるバーの始点となる座標 (※1)
							400,			-- 終点pixel位置: テクスチャ画像上において、実際に表示が変わるバーの終点となる座標 (※1)
							300,			-- アニメーション時間 (※2)
							false			-- 縦方向フラグ: バーの成長方向が垂直方向であればtrue, 水平方向であれば false (※1)
							--"asset://barfilter.png.imag"
							)
   updateProcess(boylifeProcess,1.0)
   updateProcess(tanklifeProcess,1.0)
   pCtrl = UI_Control(
							"onClick",
							"onDrag"
						)
    sysCommand(pCtrl, UI_CONTROL_ON_DBLCLICK, "onDblClick")
	count = 0
    ShowState=0  --0 show 
                 --1 showBG
	currentAnimationIndex=0  --showanimation index

    boylife = 100
    tanklife =100
    shootcount=0  --change spine shoot time

    ---texAnimation args-----
    texAinmationName=idle
    texAnimationCurrentIndex=0
    texshootcount=0
end


function execute(deltaT)
	count = count + 1

	if  ShowState==0 then   -- int 
	if count == 1 then
		changerVisiblecanvas(canvasBg,false)
		showProcess(boylifeProcess,false)
		showProcess(tanklifeProcess,false)
		--hidecanvas(CanvasBoy)
		--changerAlpha(canvasBg,0)
		changerVisiblecanvas(CanvasTank,false)
	end
	if count == 2 then     --初始化动作
		syslog(string.format("TASK scalex"))
		prop = TASK_getProperty(CanvasBoy)
		prop.scaleX = 0.5
		prop.scaleY = 0.5
		TASK_setProperty(CanvasBoy, prop)	

		prop = TASK_getProperty(CanvasBoy)
		prop.x = 250
		prop.y = 300
		TASK_setProperty(CanvasBoy, prop)	

        sysCommand(CanvasBoy, UI_CANVAS_CHANGERANIMATION, 50)   --注，2个spine都存在一个容器中
	end
	end
	if ShowState ==1 then --doanimation
		if count==1 then
		   changerVisiblecanvas(canvasBg,true)
		   hideLable(pLableTitle,false)
		   hideLable(pLableInfo1,false)
		   hideLable(pLableInfo2,false)
		   changerVisiblecanvas(CanvasTank,true)
		   showProcess(tanklifeProcess,true)
		   --changerScale(CanvasTank,0.15)
		   changePos(CanvasTank,780,550)
		 
	    end
		scale = 0.50- count/200
		changerScale(CanvasBoy,scale)
		changePos(CanvasBoy,-3,0.5)
		if scale<=0.15 then
			count=0
			ShowState=2
			showProcess(boylifeProcess,true)
		end
	end

	if ShowState ==2  then   --fight
      if count==0 then   --show fight
	  hideLable(pLableTitle,true)
	  prop = TASK_getProperty(pLableTitle)
	  prop.text = "Fight"
	  prop.txt_color = 0xFF0099
	  prop.x = 400
	  prop.y = 300
	  syslog("alpah"..prop.alpha)
	  TASK_setProperty(pLableTitle, prop)
	  sysCommand(CanvasBoy, UI_CANVAS_CHANGERANIMATION, 0,"idle",true)  
	  end
	  
	  if count <100 then
	  prop = TASK_getProperty(pLableTitle)
	  prop.alpha = 255-count*2.55
	  TASK_setProperty(pLableTitle, prop)
	  end
       if count ==100 then
	     sysCommand(CanvasBoy, UI_CANVAS_CHANGERANIMATION, 0,"run",true)   
        elseif count>100 then
           changePos(CanvasBoy,2,0)
           changePos(boylifeProcess,2,0)

           if count ==200 then
              ShowState=3
              count=0
           end 
	   end

	end
	 updateTexAnimation()
    if ShowState ==3  then   --fight
    	shootcount=shootcount-1
    	texshootcount=texshootcount-1
    	if shootcount == -1 then
    		sysCommand(CanvasBoy, UI_CANVAS_CHANGERANIMATION, 0,"idle",true)  
    	end 
    	if texshootcount == -1 then
    		 texAinmationName="idle"
             texAnimationCurrentIndex=0 
    	end 

       updateTexAnimation()
	
    	if count%200==0 then  --tank out fight
    		 texAinmationName="shoot"
             texAnimationCurrentIndex=0
             texshootcount=20
    		--sysCommand(CanvasTank, UI_CANVAS_CHANGERANIMATION, 0,"shoot",false) 
    		boylife=boylife-20
            
           syslog(string.format("boylife left= %i cal=%i",boylife,boylife/100))
           syslog(boylife/100)
    		if boylife<=0 then  --gameover
               ShowState = 4
    		end
    		updateProcess(boylifeProcess,boylife/100)
    	end
	end

	if ShowState ==4  then   --gameover
	   
	   if tanklife ==0 then
         hideLable(pLableTitle,true)
	     prop = TASK_getProperty(pLableTitle)
	     prop.text = "Success"
	     prop.alpha = 255
	     prop.txt_color = 0x990033
	     prop.x = 400
	     prop.y = 300
	     syslog("alpah"..prop.alpha)
	     TASK_setProperty(pLableTitle, prop)
	   end
	   if boylife ==0 then
         hideLable(pLableTitle,true)
	     prop = TASK_getProperty(pLableTitle)
	     prop.text = "Fail"
	     prop.alpha = 255
	     prop.txt_color = 0xCCCCCC
	     prop.x = 400
	     prop.y = 300
	     syslog("alpah"..prop.alpha)
	     TASK_setProperty(pLableTitle, prop)
	   end

	end

end


function leave()
end

function drawScaleFunc(canvas)
	sysCommand(canvas, UI_CANVAS_DRAWIMAGESCALE, 0,  0, 0.2, 0, 0xFFFFFF, 255)
	sysCommand(canvas, UI_CANVAS_DRAWIMAGESCALE, 20, 0, 0.2, 1, 0xFFFFFF, 255)
	sysCommand(canvas, UI_CANVAS_DRAWIMAGESCALE, 0, 20, 0.2, 2, 0xFFFFFF, 255)
	-- sysCommand(canvas, UI_CANVAS_FREEZE, true);
end

function drawfunc(canvas)
	sysCommand(canvas, UI_CANVAS_DRAWIMAGE, 0, 0, 0, 0xFFFFFF, 255)
end
--改变canvas显隐
function changerVisiblecanvas(canvas,show)
	syslog(string.format("TASK visible = false"))
		prop = TASK_getProperty(canvas)
		prop.visible = show
	TASK_setProperty(canvas, prop)	
end

function onClick(x,y)

	syslog(string.format("dddd Click (%i,%i)",x,y))
	if ShowState==0 then
			animationName="idle"
	        if currentAnimationIndex==0 then
               animationName="walk"
           
            elseif currentAnimationIndex==1 then
            	animationName="shoot"
            elseif currentAnimationIndex==2 then
            	animationName="run"
            	 elseif currentAnimationIndex==3 then
            	animationName="death"
            	 elseif currentAnimationIndex==4 then
            	animationName="jump"
            	 elseif currentAnimationIndex==5 then
            	animationName="aim"
            	 elseif currentAnimationIndex==6 then
            	animationName="idle"
            end
            currentAnimationIndex=currentAnimationIndex+1
            if currentAnimationIndex>6 then
            	currentAnimationIndex=0
            end

	        sysCommand(CanvasBoy, UI_CANVAS_CHANGERANIMATION, 1,animationName,true)   --注，2个spine都存在一个容器中
	end
	if ShowState ==3  then   --fight
		sysCommand(CanvasBoy, UI_CANVAS_CHANGERANIMATION, 0,"shoot",false) 
    		tanklife=tanklife-20

    		if tanklife<=0 then  --gameover
               ShowState = 4
    		end
    		shootcount=20
    	updateProcess(tanklifeProcess,tanklife/100.0)
	end

end

function onDblClick( x, y )
	if ShowState ==0 then
	   ShowState = 1
	   count =0
    end

	syslog(string.format("Double Click (%i,%i)",x,y))
end


function onDrag(mode,x,y,mvX,mvY)
	syslog(string.format("Drag - %i - (%i,%i) - mv : (%i,%i)",mode,x,y,mvX,mvY))
end

function changePos(canvas,deltaTx,deltaTy)
	prop = TASK_getProperty(canvas)
	prop.x = prop.x+ deltaTx
	prop.y = prop.y+deltaTy
	TASK_setProperty(canvas, prop)	
end

function changerScale(canvas,scale)
	prop = TASK_getProperty(canvas)
	prop.scaleX = scale
	prop.scaleY = scale
	TASK_setProperty(canvas, prop)	
end
function hideLable(pLabel,isshow)
	prop = TASK_getProperty(pLabel)
	prop.visible = isshow
	TASK_setProperty(pLabel, prop)	
end
	
function updateProcess(pProg,process) --updateProcess
	syslog(process)
	prop = TASK_getProperty(pProg)
	prop.value = process*0.2
	TASK_setProperty(pProg, prop)
end
function showProcess(pProg,isshow) --updateProcess
    prop = TASK_getProperty(pProg)
	prop.visible = isshow
	TASK_setProperty(pProg, prop)
end

function updateTexAnimation() --updateProcess
	if count%3==0 then
		texAnimationCurrentIndex=texAnimationCurrentIndex+1

        if texAnimationCurrentIndex > 7 then
        	texAnimationCurrentIndex=0
        end
		assetname= "asset://texAnimation/idle/idle00"..texAnimationCurrentIndex..".png.imag"
		if texAinmationName =="shoot" then
            assetname= "asset://texAnimation/shoot/skill00"..texAnimationCurrentIndex..".png.imag"
		end
		sysCommand(CanvasTank, UI_VARITEM_CHANGE_ASSET, assetname) 
	end
end

	