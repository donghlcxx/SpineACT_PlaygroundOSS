function setup()
	FONT_load("AlexBrush","asset://AlexBrush-Regular-OTF.otf")
	pLableTitle = UI_Label 	(
							nil, 			-- <parent pointer>, 
							7000, 			-- <order>, 
							100,100,		-- <x>, <y>,
                            0xFF, 0x000000,	-- <alpha>, <rgb>, 
							"AlexBrush",	-- "<font name>", 
							32,				-- <font size>, 
							"Spine"	-- "<text string>"
						)
end

function execute(deltaT)
	sysLoad("asset://Canvas.lua")
end

function leave()

end
