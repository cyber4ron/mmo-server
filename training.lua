require("template")

--global vars
is_real_person_103 = "false"
smog1_branch_label = ""
opA_branch_label = ""
text1_branch_label = ""
monitor_branch_label = ""
opB_branch_label = ""
opC_branch_label = ""
inopA_branch_label = ""
vicemonitor_branch_label = ""
opD_branch_label = ""
opE_branch_label = ""
opF_branch_label = ""
opG_branch_label = ""
opH_branch_label = ""
opI_branch_label = ""
opJ_branch_label = ""
chejianleader_branch_label = ""
huagongbuleader_branch_label = ""
fangtingyw1_branch_label = ""
fangtingyw2_branch_label = ""
gongchengyw1_branch_label = ""
gongchengyw2_branch_label = ""
ptayw1_branch_label = ""
ptayw2_branch_label = ""
fireengine1_branch_label = ""
fireengine2_branch_label = ""
ambulance1_branch_label = ""

function smog1()
::br0_1::	
	print('version 2')	
	print('smog1 state1')	
	ScriptSys.lua_set_particle_run("400:1", "training.lua::smog1", "400", "1", true)
	print('smog1 ......')
	ScriptSys.lua_set_particle_dir("400:1", "training.lua::smog1", "400", "1", vector3(-1,0,0))
	print('smog1 state1 over')
	inference_engine:state_work_done("smog1_state1")
end

function opA()
::br0_1::	
	print('opA state1')
	ScriptSys.lua_set_actor_action("100", "training.lua::opA", "100", "walking", 100)	
	ScriptSys.lua_set_actor_move("100", "training.lua::opA", "100", vector3(-2411,0,1964), 100)
	

	inference_engine:state_work_done("opA_state1")
	print('opA state2')
	ScriptSys.lua_set_actor_action("100", "training.lua::opA", "100", "idle", 100,3000) --opA缓慢倒下
	ScriptSys.lua_set_actor_action("100", "training.lua::opA", "100", "die", 100,3000)

	inference_engine:state_work_done("opA_state2")
	print('opA state3')
	ScriptSys.lua_set_actor_action("100", "training.lua::opA", "100", "lie", 100)

	inference_engine:state_work_done("opA_state3")
	print('opA state4')
	ScriptSys.lua_set_actor_action("100", "training.lua::opA", "100", "beencarried", 100)	
	ScriptSys.lua_set_actor_move("100", "training.lua::opA", "100", vector3(-2410,0,1960), 100)
	ScriptSys.lua_set_actor_move("100", "training.lua::opA", "100", vector3(-2245,0,1945), 100)
	ScriptSys.lua_set_actor_move("100", "training.lua::opA", "100", vector3(-2245,0,1917), 100)
	ScriptSys.lua_set_actor_move("100", "training.lua::opA", "100", vector3(-1172,0,1191)) --隐藏

	inference_engine:state_work_done("opA_state4")	
end

function text1()
::br0_1::
	print('text1 state1')
	ScriptSys.lua_add_one_music("555","training.lua::text1","555","text1.wma",1,1,"1",0)
	print('text1 state1 music')
	ScriptSys.lua_show_text_onscreen("text1","training.lua::text1","text1", "100", "[报警仪]:T-50401发生硫化氢泄露，请派人进行现场处理",0,0,30,vector3(1,0,0),5000)
	inference_engine:state_work_done("text1_state1")
	--script_manager:end_script_by_name("training.lua::text1")

end

function monitor()
::br0_1::
	print('monitor state1')
	print('baojingyi')
	ScriptSys.lua_start_alarm_flicker("600:1","training.lua::monitor","600","1",5,5000)
	print('baojingyi over')
	ScriptSys.lua_attach_sub_entity("110", "training.lua::monitor", "110", "Bip01 R Hand","phone.mesh", 1000)
	ScriptSys.lua_set_actor_action("110", "training.lua::monitor", "110", "holdphone", 100,1000)
	
	ScriptSys.lua_add_one_music("555","training.lua::monitor","555","text2.wma",1,1,"2",0)
	print('monitor music1')
	
	ScriptSys.lua_show_text_onscreen("text2","training.lua::monitor","text2", "100", "[班长]:外操间请派人到现场进行处理",0,0,30,vector3(1,0,0),10000)
	ScriptSys.lua_set_actor_action("110", "training.lua::monitor", "110", "idle", 100,1000)
	print('close baojingyi')
	ScriptSys.lua_end_alarm_flicker("600:1","training.lua::monitor","600","1",3000)
	print('close baojingyi over')
	inference_engine:state_work_done("monitor_state1")
	print('monitor state2')
	ScriptSys.lua_set_actor_action("110", "training.lua::monitor", "110", "idle", 100,1000)
	
	ScriptSys.lua_add_one_music("555","training.lua::monitor","555","text4.wma",1,1,"3",0)
	print('monitor music2')
	
	ScriptSys.lua_show_text_onscreen("text4","training.lua::monitor","text4", "100", "[班长]:启动车间应急预案，内操同志马上报警并通知各单位同志，副班长同志带领几名外操同志支援现场",0,0,30,vector3(1,0,0),10000)
	print('monitor state2 bbbbbb')
	inference_engine:state_work_done("monitor_state2")
	--script_manager:end_script_by_name("training.lua::monitor")

end

function opB()
::br0_1::
	print('opB state1')
	ScriptSys.lua_attach_sub_entity("101", "training.lua::opB", "101", "Bip01 Spine3", "mask.mesh", 50)
	ScriptSys.lua_set_actor_action("101", "training.lua::opB", "101", "run", 50,0)
	ScriptSys.lua_set_actor_move("101", "training.lua::opB", "101", vector3(-1978,0,1720), 100)
	ScriptSys.lua_set_actor_move("101", "training.lua::opB", "101", vector3(-2414,0,1959), 50)
	ScriptSys.lua_attach_sub_entity("101", "training.lua::opB", "101", "Bip01 R Hand","phone.mesh", 1000)
	ScriptSys.lua_set_actor_action("101", "training.lua::opB", "101", "holdphone", 20,0)
	--ScriptSys.lua_add_one_music("555","training.lua::smog1","555","Believe.mp3",1,0,"1",0)
	
	--ScriptSys.lua_add_one_music("555","training.lua::opB","555","text3.wma",1,1,"4",0)
	print('opB music')
	
	ScriptSys.lua_show_text_onscreen("text3","training.lua::opB","text3", "100", "[外操B]:原料罐区T-50401切水口泄露，泄漏物质为石脑油，现场一人中毒昏迷",0,0,30,vector3(1,0,0),1000)
	print('opB detach')	
	ScriptSys.lua_detach_sub_entity("101", "training.lua::opB", "101", "Bip01 R Hand","phone.mesh", 1000)
	print('opB detach over')
	--ScriptSys.lua_set_actor_action("101", "training.lua::opB", "101", "idle", 100,1000)
	print('opB state1 over')
	inference_engine:state_work_done("opB_state1")

	print('opB state2')
	ScriptSys.lua_set_actor_action("101", "training.lua::opB", "101", "breath", 100,0)
	print('opB state2 overover..')
	inference_engine:state_work_done("opB_state2")
	print('opB state3')
	ScriptSys.lua_set_actor_action("101", "training.lua::opB", "101", "run", 20,0)
	--ScriptSys.lua_set_actor_move("101", "training.lua::opB", "101", vector3(-2420,0,1928), 40)
	ScriptSys.lua_set_actor_move("101", "training.lua::opB", "101", vector3(-2420,0,1900), 100)
	ScriptSys.lua_set_actor_move("101", "training.lua::opB", "101", vector3(-2245,0,1900), 100)
	ScriptSys.lua_set_actor_move("101", "training.lua::opB", "101", vector3(-2245,0,1917), 100)
	ScriptSys.lua_set_actor_move("101", "training.lua::opB", "101", vector3(-1172,0,1191)) --隐藏
	
	--ScriptSys.lua_set_common_entity_run("500:3", "training.lua::opB", "500", "3", vector3(-1692,11,72), 0, 100)	
	--ScriptSys.lua_set_common_entity_run("500:3", "training.lua::opB", "500", "3", vector3(-1692,11,1821), 0, 100)
	--ScriptSys.lua_set_common_entity_run("500:3", "training.lua::opB", "500", "3", vector3(-42,11,63), 0, 100)
	
	ScriptSys.lua_set_common_entity_run("500:3", "training.lua::opB", "500", "3", vector3(-1692,11,1821), 0, 100)	
	ScriptSys.lua_set_common_entity_run("500:3", "training.lua::opB", "500", "3", vector3(-1692,11,72), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:3", "training.lua::opB", "500", "3", vector3(-42,11,63), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:3", "training.lua::opB", "500", "3", vector3(-72,11,450), 0, 100)

	inference_engine:state_work_done("opB_state3")	
	--script_manager:end_script_by_name("training.lua::opB")

end

function opC()
::br0_1::
	print('opC state2')

	inference_engine:state_work_done("opC_state2")
	print('opC state1')
	ScriptSys.lua_attach_sub_entity("102", "training.lua::opC", "102", "Bip01 Spine3", "mask.mesh", 50)
	ScriptSys.lua_set_actor_action("102", "training.lua::opC", "102", "run", 100)
	ScriptSys.lua_set_actor_move("102", "training.lua::opC", "102", vector3(-1978,0,1676), 100)
	ScriptSys.lua_set_actor_move("102", "training.lua::opC", "102", vector3(-2408,0,1959), 100)
	ScriptSys.lua_set_actor_action("102", "training.lua::opC", "102", "cure", 100)
	print('opC state1 done')
	inference_engine:state_work_done("opC_state1")
	--print('opC state2')

	--inference_engine:state_work_done("opC_state2")
	print('opC state3')
	ScriptSys.lua_set_actor_action("102", "training.lua::opC", "102", "carrypeople", 100)
	ScriptSys.lua_set_actor_move("102", "training.lua::opC", "102", vector3(-2410,0,1958), 100)
	ScriptSys.lua_set_actor_move("102", "training.lua::opC", "102", vector3(-2245,0,1943), 100)
	ScriptSys.lua_set_actor_move("102", "training.lua::opC", "102", vector3(-2245,0,1917), 100)
	ScriptSys.lua_set_actor_move("102", "training.lua::opC", "102", vector3(-1172,0,1191)) --隐藏

	inference_engine:state_work_done("opC_state3")
	--script_manager:end_script_by_name("training.lua::opC")

end

function inopA()
::br0_1::
	print('inopA state1')
	ScriptSys.lua_attach_sub_entity("112", "training.lua::inopA", "112", "Bip01 R Hand","phone.mesh", 1000)
	ScriptSys.lua_set_actor_action("112", "training.lua::inopA", "112", "holdphone", 100)
	
	ScriptSys.lua_add_one_music("555","training.lua::inopA","555","text5.wma",1,1,"5",0)
	print('inopA music')
	
	ScriptSys.lua_show_text_onscreen("text5","training.lua::inopA","text5", "100", "[内操]:化工部大芳烃车间原料罐区T-50401切水口泄露，泄漏物为石脑油，并含有硫化氢，现场一人中毒昏迷",0,0,30,vector3(1,0,0),3000)
	print('inopA state1 over')
	inference_engine:state_work_done("inopA_state1")
	--script_manager:end_script_by_name("training.lua::inopA")

end

function vicemonitor()
::br0_1::
	print('vicemonitor state1')
	ScriptSys.lua_set_actor_action("111", "training.lua::vicemonitor", "111", "run", 100)
	ScriptSys.lua_set_actor_move("111", "training.lua::vicemonitor", "111", vector3(-1670,0,719), 111)
	ScriptSys.lua_set_actor_move("111", "training.lua::vicemonitor", "111", vector3(-1970,0,719), 111)
	--ScriptSys.lua_set_actor_move("111", "training.lua::vicemonitor", "111", vector3(-1967,0,1683), 100)
	--ScriptSys.lua_set_actor_move("111", "training.lua::vicemonitor", "111", vector3(-2125,0,1730), 100)
	ScriptSys.lua_set_actor_action("111", "training.lua::vicemonitor", "111", "idle", 100)
	print('vicemonitor state1 ddddd')
	inference_engine:state_work_done("vicemonitor_state1")

	print('vicemonitor state2')
	ScriptSys.lua_set_actor_action("111", "training.lua::vicemonitor", "111", "run", 100, 100)
	ScriptSys.lua_set_actor_move("111", "training.lua::vicemonitor", "111", vector3(-1967,0,1683), 100)
	ScriptSys.lua_set_actor_move("111", "training.lua::vicemonitor", "111", vector3(-2125,0,1730), 100)
	ScriptSys.lua_set_actor_action("111", "training.lua::vicemonitor", "111", "idle", 100, 0)
	ScriptSys.lua_attach_sub_entity("111", "training.lua::vicemonitor","111", "Bip01 R UpperArm", "bandage.mesh", 10)
	
	ScriptSys.lua_add_one_music("555","training.lua::vicemonitor","555","text6.wma",1,1,"6",0)
	print('vicemonitor music1')
	
	ScriptSys.lua_show_text_onscreen("text6", "training.lua::vicemonitor", "text6", "111", "[副值班长]:在上风口建立指挥中心，树立大旗，请各位员工执行各自的任务", 0, 0, 30, vector3(1,0,0),2000)
	--ScriptSys.lua_set_common_entity_run("700:1", "training.lua::vicemonitor", "700", "1", vector3(-2147,0,1700), 1, 100)	
	ScriptSys.lua_set_entity_position("700:1", "training.lua::vicemonitor","700", "1",vector3(-2147,0,1700),2000)
	--print('副班长 attach 成功')
	--ScriptSys.lua_set_actor_action("111", "training.lua::vicemonitor", "111", "run", 100, 10000)
	--ScriptSys.lua_detach_sub_entity("111", "training.lua::vicemonitor","111", "Bip01 R UpperArm", "bandage.mesh", 1000)
	--print('副班长 detach 成功')
	print('vice end....')	
	inference_engine:state_work_done("vicemonitor_state2")	
	
	print('vicemonitor state3')
	ScriptSys.lua_set_actor_move("111", "training.lua::vicemonitor", "111", vector3(-2125,0,1729), 100)
	
	ScriptSys.lua_add_one_music("555","training.lua::vicemonitor","555","text8.wma",1,1,"7",0)
	print('vicemonitor music2')
	
	ScriptSys.lua_show_text_onscreen("text8", "training.lua::vicemonitor", "text8", "111", "[副值班长]:报告车间领导现场情况，移交指挥权", 0, 0, 30, vector3(1,0,0),3000)
	ScriptSys.lua_set_particle_run("400:2", "training.lua::vicemonitor", "400", "2", true)
	ScriptSys.lua_set_particle_run("400:1", "training.lua::vicemonitor", "400", "1", false)
	ScriptSys.lua_set_actor_action("111", "training.lua::vicemonitor", "111", "idle", 100, 3000)
--	ScriptSys.lua_set_actor_move("111", "training.lua::vicemonitor", "111", vector3(-2125,0,1770), 100)
	print('vicemonitor state3 end//')
	inference_engine:state_work_done("vicemonitor_state3")
	print('vicemonitor state4')
	ScriptSys.lua_set_actor_move("111", "training.lua::vicemonitor", "111", vector3(-2124,0,1729), 100)	

	inference_engine:state_work_done("vicemonitor_state4")
	print('vicemonitor state5')
	--ScriptSys.lua_set_common_entity_run("700:1", "training.lua::vicemonitor", "700", "1", vector3(-2147,0,1700), 1, 100)
	--ScriptSys.lua_set_common_entity_run("500:1", "training.lua::fireengine1", "500", "1", vector3(-330,0,63), 0, 100)
	ScriptSys.lua_set_actor_action("111", "training.lua::vicemonitor", "111", "idle", 100, 2000)
	ScriptSys.lua_set_entity_position("700:1", "training.lua::vicemonitor","700", "1", vector3(-2147,0,1700),1000)
	print('大旗OK')
	inference_engine:state_work_done("vicemonitor_state5")	
	--script_manager:end_script_by_name("training.lua::vicemonitor")

end

function opD()
::br0_1::	
	--跑位
	print('opD state1')
	if is_real_person_103 == "true" then 
		print('inner opD state1 ......')
		entity_interface:push_questions("103", "做题", "0,1,2", "0");
		print('inner .......... opD state1 ......')
		entity_interface:register_entity("103", "training.lua::opD")
		coroutine.yield();
		entity_interface:unregister_entity("103", "training.lua::opD")
	end
	print('opD state1 ......')
	--戴帽子
	if is_real_person_103 == "true" then 
		--可提取方法，跑位
		ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:请到指定位置戴上安全帽",0,0,30,vector3(1,0,0),2000);
		entity_interface:register_entity("103", "training.lua::opD")
		coroutine.yield();
		while true do
			pos = entity_interface:get_actor_position("103");
			print('@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
			if checkPos(vector3(-2044, 0, 360), pos, 1000) and entity_interface:check_actor_equipment("103","capred.mesh") then
				print('................................................................')
				entity_interface:unregister_entity("103", "training.lua::opD");
				break;
			end
			ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:请到指定位置戴上安全帽...",0,0,30,vector3(1,0,0),2000);
			coroutine.yield();
		end
	end
	if is_real_person_103 == "false" then 
		ScriptSys.lua_set_actor_move("103", "training.lua::opD", "103", vector3(-2044, 0, 360), 100) 
		print('arrive test')
		ScriptSys.lua_attach_sub_entity("103", "training.lua::opD","103","Bip01 Head","capred.mesh", 1000)
	end

	--戴上呼吸器
	if is_real_person_103 == "true" then 
		--可提取方法，跑位
		ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:请到指定位置戴上呼吸器",0,0,30,vector3(1,0,0),2000);
		entity_interface:register_entity("103", "training.lua::opD")
		coroutine.yield();
		while true do
			pos = entity_interface:get_actor_position("103")
			if checkPos(vector3(-2044, 0, 360), pos, 1000) and entity_interface:check_actor_equipment("103","mask.mesh") then
				print('aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa')
				entity_interface:unregister_entity("103", "training.lua::opD");
				break;
			end
			ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:请到指定位置戴上呼吸器...",0,0,30,vector3(1,0,0),2000);
			coroutine.yield();
		end
	end
	if is_real_person_103 == "false" then 
		ScriptSys.lua_set_actor_move("103", "training.lua::opD", "103", vector3(-2044, 0, 360), 100) 
		print('arrive test')
		ScriptSys.lua_attach_sub_entity("103", "training.lua::opD","103","Bip01 Spine3","mask.mesh", 1000)
	end			
	print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')	
	if is_real_person_103 == "true" then 
		--可提取方法，跑位
		ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:跑向位置1",0,0,30,vector3(1,0,0),2000);
		entity_interface:register_entity("103", "training.lua::opD")
		coroutine.yield();
		while true do
			pos = entity_interface:get_actor_position("103");
			if checkPos(vector3(-1961, 0, 1684), pos, 50) then
				entity_interface:unregister_entity("103", "training.lua::opD");
				break;
			end
			ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:位置不正确，跑向位置1",0,0,30,vector3(1,0,0),2000);
			coroutine.yield();
		end
	end
	if is_real_person_103 == "false" then ScriptSys.lua_set_actor_move("103", "training.lua::opD", "103", vector3(-1970,0,710), 100) end

	inference_engine:state_work_done("opD_state1")

	--跑位
	print('opD state2')
	if is_real_person_103 == "true" then
		--可提取方法，跑位
		ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:跑向位置2",0,0,30,vector3(1,0,0),2000);
		entity_interface:register_entity("103", "training.lua::opD")
		coroutine.yield();
		while true do
			pos = entity_interface:get_actor_position("103");
			if checkPos(vector3(-1961, 0, 1684), pos, 50) then
				entity_interface:unregister_entity("103", "training.lua::opD");
				break;
			end
			ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:位置不正确，跑向位置2",0,0,30,vector3(1,0,0),2000);
			coroutine.yield();
		end
	end
	if is_real_person_103 == "false" then ScriptSys.lua_set_actor_move("103", "training.lua::opD", "103", vector3(-1967,0,1683), 100) end

	inference_engine:state_work_done("opD_state2")

	--跑位
	print('opD state3')
	if is_real_person_103 == "true" then
		--可提取方法，跑位
		ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:跑向位置3",0,0,30,vector3(1,0,0),2000);
		entity_interface:register_entity("103", "training.lua::opD")
		coroutine.yield();
		while true do
			pos = entity_interface:get_actor_position("103");
			if checkPos(vector3(-1961, 0, 1684), pos, 50) then
				entity_interface:unregister_entity("103", "training.lua::opD");
				break;
			end
			ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:位置不正确，跑向位置3",0,0,30,vector3(1,0,0),2000);
			coroutine.yield();
		end
	end
	if is_real_person_103 == "false" then 
		ScriptSys.lua_set_actor_move("103", "training.lua::opD", "103", vector3(-2185,0,1902), 100)
		ScriptSys.lua_set_actor_move("103", "training.lua::opD", "103", vector3(-2219,0,2201), 100)
		ScriptSys.lua_set_actor_move("103", "training.lua::opD", "103", vector3(-2220,0,2201), 100)
		ScriptSys.lua_set_actor_action("103", "training.lua::opD", "103", "idle", 100,1000)
	end
	
	--阀门
	if is_real_person_103 == "true" then  
		ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:阀门",0,0,30,vector3(1,0,0),2000);		
		entity_interface:register_entity("103", "training.lua::opD")
		coroutine.yield();
		while true do
			state = entity_interface:get_valve_state("200:1");
			if equal(state, 0, 0.1) then
				entity_interface:unregister_entity("103", "training.lua::opD");
				break;
			end
			ScriptSys.lua_show_text_onscreen("text7","training.lua::opD","text7", "103", "[opD]:操作不正确，阀门",0,0,30,vector3(1,0,0),2000);
			coroutine.yield();
		end
	end
	if is_real_person_103 == "false" then ScriptSys.lua_set_actor_action("103", "training.lua::opD", "103", "miehuo", 100,1000) end
	
	print('opD state3 ...')
	inference_engine:state_work_done("opD_state3")
	--script_manager:end_script_by_name("training.lua::opD")
end

function opE()
::br0_1::
	print('opE state1')
	ScriptSys.lua_set_actor_move("104", "training.lua::opE", "104", vector3(-1970,0,700), 100)

	inference_engine:state_work_done("opE_state1")

	print('opE state2')
	ScriptSys.lua_set_actor_move("104", "training.lua::opE", "104", vector3(-1967,0,1673), 100)

	inference_engine:state_work_done("opE_state2")
	print('opE state3')
	ScriptSys.lua_set_actor_move("104", "training.lua::opE", "104", vector3(-2185,0,1902), 100)
	ScriptSys.lua_set_actor_move("104", "training.lua::opE", "104", vector3(-2209,0,2168), 100)
	ScriptSys.lua_set_actor_move("104", "training.lua::opE", "104", vector3(-2210,0,2168), 100)
	ScriptSys.lua_set_actor_action("104", "training.lua::opE", "104", "idle", 100)
	ScriptSys.lua_set_actor_action("104", "training.lua::opE", "104", "miehuo", 100)
	print('opE state3 ...')
	inference_engine:state_work_done("opE_state3")
	--script_manager:end_script_by_name("training.lua::opE")

end

function opF()
::br0_1::
	print('opF state1')
	ScriptSys.lua_set_actor_move("105", "training.lua::opF", "105", vector3(-1970,0,690), 100)

	inference_engine:state_work_done("opF_state1")

	print('opF state2')
	ScriptSys.lua_set_actor_move("105", "training.lua::opF", "105", vector3(-1967,0,1663), 100)

	inference_engine:state_work_done("opF_state2")
	print('opF state3')
--[[	ScriptSys.lua_set_actor_move("105", "training.lua::opF", "105", vector3(-2212,0,1992), 100)
	ScriptSys.lua_set_actor_move("105", "training.lua::opF", "105", vector3(-2220,0,2102), 100)
	ScriptSys.lua_set_actor_move("105", "training.lua::opF", "105", vector3(-2221,0,2102), 100)
	ScriptSys.lua_set_actor_action("105", "training.lua::opF", "105", "idle", 100,1000)
	ScriptSys.lua_set_actor_action("105", "training.lua::opF", "105", "twisttwo", 100,1000)
	ScriptSys.lua_set_valve_run("200:3", "training.lua::opF", "200", "3", 360, 100, 1)
	ScriptSys.lua_set_actor_action("105", "training.lua::opF", "105", "idle", 100,1000)
	ScriptSys.lua_set_actor_action("105", "training.lua::opF", "105", "run", 100,1000)
	ScriptSys.lua_set_actor_move("105", "training.lua::opF", "105", vector3(-2220,0,2102), 100)
	ScriptSys.lua_set_actor_move("105", "training.lua::opF", "105", vector3(-2212,0,1992), 100)
	ScriptSys.lua_set_actor_move("105", "training.lua::opF", "105", vector3(-1967,0,1663), 100)
	ScriptSys.lua_set_actor_move("105", "training.lua::opF", "105", vector3(-1970,0,690), 100)
	ScriptSys.lua_set_actor_move("105", "training.lua::opF", "105", vector3(-2070,0,360), 100)
	ScriptSys.lua_set_actor_action("105", "training.lua::opF", "105", "idle", 100,1000)	--]]
	print('opF state3 ...')
	inference_engine:state_work_done("opF_state3")
	--script_manager:end_script_by_name("training.lua::opF")

end

function opG()
::br0_1::
	print('opG state1')
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-1970,0,680), 100)

	inference_engine:state_work_done("opG_state1")

	print('opG state2')
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-1967,0,1653), 100)

	inference_engine:state_work_done("opG_state2")
	print('opG state3')
--[[	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-2370,0,1937), 100)
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-2370,0,2023), 100)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "idle", 100,1000)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "twisttwo", 100,1000)
	ScriptSys.lua_set_valve_run("200:4", "training.lua::opG", "200", "4", 360, 100, 1)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "idle", 100,1000)--]]
	print('opG state3 ...')
	inference_engine:add_fact("expr1", "a")
	inference_engine:add_fact("expr2", "a")
	print('add fact over at OPG state3')
	inference_engine:state_work_done("opG_state3")
	
	print('OPG test if')
	--opG_branch_label = "br0_1_1"
	print(opG_branch_label)
	if opG_branch_label == "br0_1_1" then
		goto br0_1_1
	elseif opG_branch_label == "br0_1_2" then
		goto br0_1_2
	else 
		print(opG_branch_label)	coroutine.yield()
	end	
	print('OPG test if suc')	
::br0_1_1::
	print('br0_1_1 start')
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "run", 100,1000)
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-2370,0,1989), 100)
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-2562,0,1989), 100)
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-2562,0,1990), 100)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "idle", 100,1000)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "twisttwo", 100,1000)
	ScriptSys.lua_set_valve_run("200:5", "training.lua::opG", "200", "5", 360, 100, 1)
	
	ScriptSys.lua_add_one_music("555","training.lua::opG","555","text7.wma",1,1,"8",0)
	print('opG music')
	
	ScriptSys.lua_show_text_onscreen("text7","training.lua::opG","text7", "100", "[中控室]:预加氢反应系统停车，调度室请停供直馏重石油",0,0,30,vector3(1,0,0),2000)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "idle", 100,1000)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "run", 100,1000)
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-2562,0,1989), 100)	
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-2370,0,1937), 100)
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-1967,0,1653), 100)
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-1970,0,680), 100)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "idle", 100,1000)
	print('br0_1_1 end')
	inference_engine:state_work_done("opG_state4")
::br0_1_2::
	print('br0_1_2 start')
	ScriptSys.lua_add_one_music("555","training.lua::opG","555","text7.wma",1,1,"8",0)
	print('opG music')		
	ScriptSys.lua_show_text_onscreen("text7","training.lua::opG","text7", "100", "[中控室]:预加氢反应系统停车，调度室请停供直馏重石油",0,0,30,vector3(1,0,0),2000)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "idle", 100,1000)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "run", 100,1000)
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-2562,0,1989), 100)	
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-2370,0,1937), 100)
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-1967,0,1653), 100)
	ScriptSys.lua_set_actor_move("106", "training.lua::opG", "106", vector3(-1970,0,680), 100)
	ScriptSys.lua_set_actor_action("106", "training.lua::opG", "106", "idle", 100,1000)
	print('br0_1_2 end')
	inference_engine:state_work_done("opG_state5")
end

function opH()
::br0_1::
	print('opH state1')
	ScriptSys.lua_set_actor_move("107", "training.lua::opH", "107", vector3(-1970,0,670), 100)

	inference_engine:state_work_done("opH_state1")

	print('opH state2')
	ScriptSys.lua_set_actor_move("107", "training.lua::opH", "107", vector3(-1967,0,1643), 100)

	inference_engine:state_work_done("opH_state2")
	print('opH state3')
--[[	ScriptSys.lua_set_actor_move("107", "training.lua::opH", "107", vector3(-2182,0,2040), 100)
	ScriptSys.lua_set_actor_move("107", "training.lua::opH", "107", vector3(-2220,0,2280), 100)
	ScriptSys.lua_set_actor_move("107", "training.lua::opH", "107", vector3(-2221,0,2280), 100)
	ScriptSys.lua_set_actor_action("107", "training.lua::opH", "107", "idle", 100,1000)
	ScriptSys.lua_set_actor_action("107", "training.lua::opH", "107", "twisttwo", 100,1000)
	ScriptSys.lua_set_valve_run("200:1", "training.lua::opH", "200", "1", 360, 100, 1)
	ScriptSys.lua_set_actor_action("107", "training.lua::opH", "107", "idle", 100,1000)
	ScriptSys.lua_set_actor_action("107", "training.lua::opH", "107", "run", 100)
	ScriptSys.lua_set_actor_move("107", "training.lua::opH", "107", vector3(-2185,0,1902), 100)
	ScriptSys.lua_set_actor_move("107", "training.lua::opH", "107", vector3(-1993,0,1750), 100)
	ScriptSys.lua_set_actor_move("107", "training.lua::opH", "107", vector3(-1992,0,1750), 100)
	ScriptSys.lua_set_actor_action("107", "training.lua::opH", "107", "idle", 100,1000)
--]]	print('opH state3 ...')
	inference_engine:state_work_done("opH_state3")
	print('opH state4')
	--ScriptSys.lua_set_common_entity_run("800:1", "training.lua::opH", "800", "1", vector3(-1993,15,1748), 1, 100)
	ScriptSys.lua_set_entity_position("800:1", "training.lua::opH","800", "1",vector3(-1993,15,1748),1000)
	ScriptSys.lua_set_entity_position("800:1", "training.lua::opH","800", "1",vector3(-1993,15,1748),1000)
	print('警戒线OK')
	inference_engine:state_work_done("opH_state4")
	--script_manager:end_script_by_name("training.lua::opH")

end

function opI()
::br0_1::
	print('opI state1')
	ScriptSys.lua_set_actor_move("108", "training.lua::opI", "108", vector3(-1970,0,660), 100)

	inference_engine:state_work_done("opI_state1")

	print('opI state2')
	ScriptSys.lua_set_actor_move("108", "training.lua::opI", "108", vector3(-1967,0,1633), 100)

	inference_engine:state_work_done("opI_state2")
	print('opI state3')
--[[	ScriptSys.lua_set_actor_move("108", "training.lua::opI", "108", vector3(-2182,0,2040), 100)
	ScriptSys.lua_set_actor_move("108", "training.lua::opI", "108", vector3(-2220,0,2260), 100)
	ScriptSys.lua_set_actor_move("108", "training.lua::opI", "108", vector3(-2221,0,2260), 100)
	ScriptSys.lua_set_actor_action("108", "training.lua::opI", "108", "idle", 100,1000)
	ScriptSys.lua_set_actor_action("108", "training.lua::opI", "108", "twisttwo", 100,1000)
	ScriptSys.lua_set_valve_run("200:2", "training.lua::opI", "200", "2", 360, 100, 1)--]]
	print("Circle Water1")
	ScriptSys.lua_set_particle_run("420:1","training.lua::opI","420","1",true)
	print("Circle Water1 End")
	ScriptSys.lua_set_actor_action("108", "training.lua::opI", "108", "idle", 100,1000)
	ScriptSys.lua_set_actor_action("108", "training.lua::opI", "108", "run", 100)
	ScriptSys.lua_set_actor_move("108", "training.lua::opI", "108", vector3(-2185,0,1902), 100)
	ScriptSys.lua_set_actor_move("108", "training.lua::opI", "108", vector3(-1663,0,1750), 100)
	ScriptSys.lua_set_actor_move("108", "training.lua::opI", "108", vector3(-1664,0,1750), 100)
	ScriptSys.lua_set_actor_action("108", "training.lua::opI", "108", "idle", 100,1000)
	print('opI state3 ....')
	inference_engine:state_work_done("opI_state3")
	--script_manager:end_script_by_name("training.lua::opI")

end

function opJ()
::br0_1::
	print('opJ state1')
	ScriptSys.lua_set_actor_move("109", "training.lua::opJ", "109", vector3(-1970,0,650), 100)

	inference_engine:state_work_done("opJ_state1")

	print('opJ state2')
	ScriptSys.lua_set_actor_move("109", "training.lua::opJ", "109", vector3(-1967,0,1623), 100)
	print('opJ state2 end')
	inference_engine:state_work_done("opJ_state2")

	print('opJ state3')
	ScriptSys.lua_set_actor_move("109", "training.lua::opJ", "109", vector3(-1953,0,1817), 100)
	ScriptSys.lua_set_actor_move("109", "training.lua::opJ", "109", vector3(-1657,0,1817), 100)
	ScriptSys.lua_set_actor_move("109", "training.lua::opJ", "109", vector3(-1657,0,105), 100)
	ScriptSys.lua_set_actor_move("109", "training.lua::opJ", "109", vector3(-1660,0,105), 100)
	ScriptSys.lua_set_actor_action("109", "training.lua::opJ", "109", "lead", 100, 0)
	print('opJ state3 end')
	inference_engine:state_work_done("opJ_state3")
	--script_manager:end_script_by_name("training.lua::opJ")

end

function chejianleader()
::br0_1::
	print('chejianleader state1')
	ScriptSys.lua_set_actor_action("113", "training.lua::chejianleader", "113", "run", 100, 1000)
	ScriptSys.lua_set_actor_move("113", "training.lua::chejianleader", "113", vector3(-1670,0,710), 100)
	ScriptSys.lua_set_actor_move("113", "training.lua::chejianleader", "113", vector3(-1970,0,719), 100)
	ScriptSys.lua_set_actor_move("113", "training.lua::chejianleader", "113", vector3(-1967,0,1683), 100)
	ScriptSys.lua_set_actor_move("113", "training.lua::chejianleader", "113", vector3(-2125,0,1715), 100)
	ScriptSys.lua_set_actor_move("113", "training.lua::chejianleader", "113", vector3(-2125,0,1716), 100)
	ScriptSys.lua_set_actor_action("113", "training.lua::chejianleader", "113", "idle", 1, 1000)
	ScriptSys.lua_detach_sub_entity("111", "training.lua::chejianleader","111", "Bip01 R UpperArm", "bandage.mesh", 1000)
	ScriptSys.lua_attach_sub_entity("113", "training.lua::chejianleader","113", "Bip01 R UpperArm", "bandage.mesh", 1000)
	print('chejianleader state1 chejian over')
	inference_engine:state_work_done("chejianleader_state1")
	
	print('chejianleader state2')
	
	ScriptSys.lua_add_one_music("555","training.lua::chejianleader","555","text9.wma",1,1,"9",0)
	print('chejianleader state1 music')	
	ScriptSys.lua_show_text_onscreen("text9","training.lua::chejianleader","text9", "113", "[车间领导]:请各单位积极参与救火工作",0,0,30,vector3(1,0,0),2000)
	
	ScriptSys.lua_add_one_music("555","training.lua::chejianleader","555","text10.wma",1,1,"10",0)
	print('chejianleader state2 music')	
	ScriptSys.lua_show_text_onscreen("text10","training.lua::chejianleader","text10", "113", "[调度室]:化工部大芳烃车间原料罐区T-50401切水口泄露，泄漏物质为石脑油，并含有硫化氢，现场一人中毒昏迷",0,0,30,vector3(1,0,0),2000)
	print('chejianleader state2 is ok')
	inference_engine:state_work_done("chejianleader_state2")
	print('chejianleader state3 isok')
	ScriptSys.lua_set_actor_move("113", "training.lua::chejianleader", "113", vector3(-2124,0,1716), 100)

	inference_engine:state_work_done("chejianleader_state3")	
	--script_manager:end_script_by_name("training.lua::chejianleader")

end

function huagongbuleader()
::br0_1::
	print('huagongbuleader state1')
	ScriptSys.lua_set_actor_action("114", "training.lua::huagongbuleader", "114", "run", 100, 0)
	ScriptSys.lua_set_actor_move("114", "training.lua::huagongbuleader", "114", vector3(-1670,0,54), 100)
	ScriptSys.lua_set_actor_move("114", "training.lua::huagongbuleader", "114", vector3(-1670,0,710), 100)
	ScriptSys.lua_set_actor_move("114", "training.lua::huagongbuleader", "114", vector3(-1970,0,719), 100)
	ScriptSys.lua_set_actor_move("114", "training.lua::huagongbuleader", "114", vector3(-1967,0,1683), 100)
	ScriptSys.lua_set_actor_move("114", "training.lua::huagongbuleader", "114", vector3(-2103,0,1716), 110)
	ScriptSys.lua_set_actor_action("114", "training.lua::huagongbuleader", "114", "idle", 100, 1000)
	ScriptSys.lua_detach_sub_entity("113", "training.lua::huagongbuleader","113", "Bip01 R UpperArm", "bandage.mesh", 1000)
	ScriptSys.lua_attach_sub_entity("114", "training.lua::huagongbuleader","114", "Bip01 R UpperArm", "bandage.mesh", 1000)
	print('huagongbuleader state1 over')
	inference_engine:state_work_done("huagongbuleader_state1")
	print('huagongbuleader state2')
	ScriptSys.lua_set_actor_action("114", "training.lua::huagongbuleader", "114", "call", 100, 1000)
	print('huagongbuleader state2 overa')
	
	ScriptSys.lua_add_one_music("555","training.lua::huagongbuleader","555","text11.wma",1,1,"11",0)
	print('huagongbuleader state1 music')
	ScriptSys.lua_show_text_onscreen("text11","training.lua::huagongbuleader","text11", "114", "[化工部领导]:大芳烃车间将中毒者送至安全位置，做好灭火工作，做好工艺处理和水体污染防控工作。",0,0,30,vector3(1,0,0),1000)
	print('huagongbuleader state2 overb')
	
	--ScriptSys.lua_add_one_music("555","training.lua::huagongbuleader","555","text12.wma",1,1,"12",0)
	print('huagongbuleader state2 music')	
	ScriptSys.lua_show_text_onscreen("text12","training.lua::huagongbuleader","text12", "114", "[化工部领导]:调度生产部搞好事故状态下的生产平衡，保障通讯联络畅通。",0,0,30,vector3(1,0,0),1000)
	
	--ScriptSys.lua_add_one_music("555","training.lua::huagongbuleader","555","text13.wma",1,1,"13",0)
	print('huagongbuleader state3 music')
	ScriptSys.lua_show_text_onscreen("text13","training.lua::huagongbuleader","text13", "114", "[化工部领导]:安全环保部、质检车间做好现场监测，确定警戒区域，并注意含油消防水的流向，防止水体污染。",0,0,30,vector3(1,0,0),1000)
	
	--ScriptSys.lua_add_one_music("555","training.lua::huagongbuleader","555","text14.wma",1,1,"14",0)
	print('huagongbuleader state4 music')
	ScriptSys.lua_show_text_onscreen("text14","training.lua::huagongbuleader","text14", "114", "[化工部领导]:保卫组根据监测数据对事故现场实施警戒及交通管制，引导公司应急车辆进入现场。",0,0,30,vector3(1,0,0),1000)
	
	--ScriptSys.lua_add_one_music("555","training.lua::huagongbuleader","555","text15.wma",1,1,"15",0)
	print('huagongbuleader state5 music')
	ScriptSys.lua_show_text_onscreen("text15","training.lua::huagongbuleader","text15", "114", "[化工部领导]:设备管理部立即研究石脑油罐切水口泄漏管线的封堵方案，减少物料泄漏量。",0,0,30,vector3(1,0,0),1000)
	
	--ScriptSys.lua_add_one_music("555","training.lua::huagongbuleader","555","text16.wma",1,1,"16",0)
	print('huagongbuleader state6 music')
	ScriptSys.lua_show_text_onscreen("text16","training.lua::huagongbuleader","text16", "114", "[化工部领导]:技术质量部指导车间做好工艺处理工作，并做好三剂保护。",0,0,30,vector3(1,0,0),1000)
	ScriptSys.lua_set_actor_action("114", "training.lua::huagongbuleader", "114", "idle", 100, 1000)
	print('huagongbuleader state2 over')
	inference_engine:state_work_done("huagongbuleader_state2")
	
	print('huagongbuleader state3')
	inference_engine:state_work_done("huagongbuleader_state3")
	--script_manager:end_script_by_name("training.lua::huagongbuleader")

end

function fangtingyw1()
::br0_1::
	print('fangtingyw1 state1')
	ScriptSys.lua_set_actor_action("115", "training.lua::fangtingyw1", "115", "runwithextinguisher", 100, 0)
	ScriptSys.lua_set_actor_move("115", "training.lua::fangtingyw1", "115", vector3(-1690,0,750), 100)
	ScriptSys.lua_set_actor_move("115", "training.lua::fangtingyw1", "115", vector3(-1690,0,1830), 100)
	ScriptSys.lua_set_actor_move("115", "training.lua::fangtingyw1", "115", vector3(-2185,0,1830), 100)
	ScriptSys.lua_set_actor_move("115", "training.lua::fangtingyw1", "115", vector3(-2185,0,2040), 100)
	ScriptSys.lua_set_actor_move("115", "training.lua::fangtingyw1", "115", vector3(-2300,0,2040), 100)
	ScriptSys.lua_set_actor_move("115", "training.lua::fangtingyw1", "115", vector3(-2300,0,2041), 110)
	ScriptSys.lua_set_actor_action("115", "training.lua::fangtingyw1", "115", "miehuo", 100, 0)
	ScriptSys.lua_set_particle_velocity("410:5", "training.lua::fangtingyw1", "410", "5", 40, 50)
	ScriptSys.lua_set_particle_dir("410:5", "training.lua::fangtingyw1", "410", "5", vector3(0,6,1))
	print('fangtingyw1 state1 over')
	inference_engine:state_work_done("fangtingyw1_state1")
	--script_manager:end_script_by_name("training.lua::fangtingyw1")

end

function fangtingyw2()
::br0_1::
	print('fangtingyw2 state1')
	ScriptSys.lua_set_actor_action("116", "training.lua::fangtingyw2", "116", "runwithextinguisher", 100, 0)
	ScriptSys.lua_set_actor_move("116", "training.lua::fangtingyw2", "116", vector3(-1690,0,780), 100)
	ScriptSys.lua_set_actor_move("116", "training.lua::fangtingyw2", "116", vector3(-1690,0,1830), 100)
	ScriptSys.lua_set_actor_move("116", "training.lua::fangtingyw2", "116", vector3(-2185,0,1830), 100)
	ScriptSys.lua_set_actor_move("116", "training.lua::fangtingyw2", "116", vector3(-2185,0,2040), 100)
	ScriptSys.lua_set_actor_move("116", "training.lua::fangtingyw2", "116", vector3(-2330,0,2040), 100)
	ScriptSys.lua_set_actor_move("116", "training.lua::fangtingyw2", "116", vector3(-2330,0,2041), 100)
	ScriptSys.lua_set_actor_action("116", "training.lua::fangtingyw2", "116", "miehuo", 110, 0)
	ScriptSys.lua_set_particle_velocity("410:6", "training.lua::fangtingyw2", "410", "6", 40, 50)
	ScriptSys.lua_set_particle_dir("410:6", "training.lua::fangtingyw2", "410", "6", vector3(0,6,1))
	print('fangtingyw2 state1 over')
	inference_engine:state_work_done("fangtingyw2_state1")
	--script_manager:end_script_by_name("training.lua::fangtingyw2")

end

function gongchengyw1()
::br0_1::
	print('gongchengyw1 state1')
	ScriptSys.lua_set_actor_action("117", "training.lua::gongchengyw1", "117", "runwithextinguisher", 100, 0)
	ScriptSys.lua_set_actor_move("117", "training.lua::gongchengyw1", "117", vector3(-1690,0,810), 100)
	ScriptSys.lua_set_actor_move("117", "training.lua::gongchengyw1", "117", vector3(-1690,0,1830), 100)
	ScriptSys.lua_set_actor_move("117", "training.lua::gongchengyw1", "117", vector3(-2185,0,1830), 100)
	ScriptSys.lua_set_actor_move("117", "training.lua::gongchengyw1", "117", vector3(-2185,0,2130), 100)
	ScriptSys.lua_set_actor_move("117", "training.lua::gongchengyw1", "117", vector3(-2186,0,2130), 100)
	ScriptSys.lua_set_actor_action("117", "training.lua::gongchengyw1", "117", "miehuo", 100, 0)
	ScriptSys.lua_set_particle_velocity("410:2", "training.lua::gongchengyw1", "410", "2", 40, 50)
	ScriptSys.lua_set_particle_dir("410:2", "training.lua::gongchengyw1", "410", "2", vector3(-1,6,0))
	print('gongchengyw2 state1 gongcheng1')
	inference_engine:state_work_done("gongchengyw1_state1")
	--script_manager:end_script_by_name("training.lua::gongchengyw1")

end

function gongchengyw2()
::br0_1::
	print('gongchengyw2 state1')
	ScriptSys.lua_set_actor_action("118", "training.lua::gongchengyw2", "118", "runwithextinguisher", 100, 0)	
	ScriptSys.lua_set_actor_move("118", "training.lua::gongchengyw2", "118", vector3(-1690,0,840), 100)
	ScriptSys.lua_set_actor_move("118", "training.lua::gongchengyw2", "118", vector3(-1690,0,1830), 100)
	ScriptSys.lua_set_actor_move("118", "training.lua::gongchengyw2", "118", vector3(-2185,0,1830), 100)
	ScriptSys.lua_set_actor_move("118", "training.lua::gongchengyw2", "118", vector3(-2185,0,2180), 100)
	ScriptSys.lua_set_actor_move("118", "training.lua::gongchengyw2", "118", vector3(-2186,0,2180), 110)
	ScriptSys.lua_set_actor_action("118", "training.lua::gongchengyw2", "118", "miehuo", 100, 0)
	ScriptSys.lua_set_particle_velocity("410:1", "training.lua::gongchengyw2", "410", "1", 40, 50)
	ScriptSys.lua_set_particle_dir("410:1", "training.lua::gongchengyw2", "410", "1", vector3(-1,6,0))
	ScriptSys.lua_set_water_face_move("900:1", "training.lua::gongchengyw2","900", "1",vector3(-2455,18,2249),0.2,0,1000)
	print('gongchengyw2 state1 gongcheng2')
	inference_engine:state_work_done("gongchengyw2_state1")
	--script_manager:end_script_by_name("training.lua::gongchengyw2")

end

function ptayw1()
::br0_1::
	print('ptayw1 state1')
	ScriptSys.lua_set_actor_action("119", "training.lua::ptayw1", "119", "runwithextinguisher", 100, 0)
	ScriptSys.lua_set_actor_move("119", "training.lua::ptayw1", "119", vector3(-1690,0,450), 110)
	ScriptSys.lua_set_actor_move("119", "training.lua::ptayw1", "119", vector3(-1690,0,1830), 100)
	ScriptSys.lua_set_actor_move("119", "training.lua::ptayw1", "119", vector3(-2185,0,1830), 100)
	ScriptSys.lua_set_actor_move("119", "training.lua::ptayw1", "119", vector3(-2208,0,2068), 100)
	ScriptSys.lua_set_actor_action("119", "training.lua::ptayw1", "119", "miehuo", 100, 0)
	ScriptSys.lua_set_particle_velocity("410:3", "training.lua::ptayw1", "410", "3", 40, 50)
ScriptSys.lua_set_particle_dir("410:3", "training.lua::ptayw1", "410", "3", vector3(-1,6,1))
	print('ptayw1 state1 pta1')
	inference_engine:state_work_done("ptayw1_state1")
	--script_manager:end_script_by_name("training.lua::ptayw1")

end

function ptayw2()
::br0_1::
	print('ptayw2 state1')
	ScriptSys.lua_set_actor_action("120", "training.lua::ptayw2", "120", "runwithextinguisher", 100, 0)
	ScriptSys.lua_set_actor_move("120", "training.lua::ptayw2", "120", vector3(-1690,0,450), 100)
	ScriptSys.lua_set_actor_move("120", "training.lua::ptayw2", "120", vector3(-1690,0,1830), 100)
	ScriptSys.lua_set_actor_move("120", "training.lua::ptayw2", "120", vector3(-2185,0,1830), 100)
	ScriptSys.lua_set_actor_move("120", "training.lua::ptayw2", "120", vector3(-2232,0,2048), 100)
	ScriptSys.lua_set_actor_action("120", "training.lua::ptayw2", "120", "miehuo", 100, 0)
	ScriptSys.lua_set_particle_velocity("410:4", "training.lua::ptayw2", "410", "4", 40, 50)
	ScriptSys.lua_set_particle_dir("410:4", "training.lua::ptayw2", "410", "4", vector3(-1,6,1))
	print('ptayw2 state1 pta2')
	inference_engine:state_work_done("ptayw2_state1")
	--script_manager:end_script_by_name("training.lua::ptayw2")

end

function fireengine1()
::br0_1::
	print('fireengine1 state1')
	ScriptSys.lua_set_common_entity_run("500:1", "training.lua::fireengine1", "500", "1", vector3(-330,18,63), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:1", "training.lua::fireengine1", "500", "1", vector3(-1692,18,72), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:1", "training.lua::fireengine1", "500", "1", vector3(-1692,18,1821), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:1", "training.lua::fireengine1", "500", "1", vector3(-2416,18,1827), 0, 100)
	ScriptSys.lua_set_particle_velocity("410:7", "training.lua::fireengine1", "410", "7", 100, 120)
	ScriptSys.lua_set_particle_dir("410:7", "training.lua::fireengine1", "410", "7", vector3(0,1,3))
	print('fireengine1 state1 end')
	inference_engine:state_work_done("fireengine1_state1")
	--script_manager:end_script_by_name("training.lua::fireengine1")

end

function fireengine2()
::br0_1::
	print('fireengine2 state1')
	ScriptSys.lua_set_common_entity_run("500:2", "training.lua::fireengine2", "500", "2", vector3(-177,18,63), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:2", "training.lua::fireengine2", "500", "2", vector3(-1692,18,72), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:2", "training.lua::fireengine2", "500", "2", vector3(-1692,18,1821), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:2", "training.lua::fireengine2", "500", "2", vector3(-2306,18,1827), 0, 100)
	ScriptSys.lua_set_particle_velocity("410:8", "training.lua::fireengine2", "410", "8", 100, 120)
	ScriptSys.lua_set_particle_dir("410:8", "training.lua::fireengine2", "410", "8", vector3(0,1,3))
	
	print('test water close')
	ScriptSys.lua_set_actor_action("111", "training.lua::fireengine2", "111", "idle", 100, 10000)
	ScriptSys.lua_set_particle_run("400:2", "training.lua::fireengine2", "400", "2", false)
	--ScriptSys.lua_set_particle_velocity("400:2", "training.lua::fireengine2", "400", "2", 0, 0)
	--ScriptSys.lua_set_particle_dir("400:2", "training.lua::fireengine2", "400", "2", vector3(0,-1,1))
	ScriptSys.lua_set_actor_action("111", "training.lua::fireengine2", "111", "idle", 100, 3000)
	ScriptSys.lua_set_particle_run("410:1", "training.lua::fireengine2", "410", "1", false)
	ScriptSys.lua_set_particle_run("410:2", "training.lua::fireengine2", "410", "2", false)
	ScriptSys.lua_set_particle_run("410:3", "training.lua::fireengine2", "410", "3", false)
	ScriptSys.lua_set_particle_run("410:4", "training.lua::fireengine2", "410", "4", false)
	ScriptSys.lua_set_particle_run("410:5", "training.lua::fireengine2", "410", "5", false)
	ScriptSys.lua_set_particle_run("410:6", "training.lua::fireengine2", "410", "6", false)
	ScriptSys.lua_set_particle_run("410:7", "training.lua::fireengine2", "410", "7", false)
	ScriptSys.lua_set_particle_run("410:8", "training.lua::fireengine2", "410", "8", false)	
	ScriptSys.lua_set_particle_run("420:1", "training.lua::fireengine2", "420", "1", false)		
	print('test water close suc')	
	
	print('fireengine2 state1 end')
	inference_engine:state_work_done("fireengine2_state1")
	--script_manager:end_script_by_name("training.lua::fireengine2")

end

function ambulance1()
::br0_1::
	print('ambulance1 state1')
	ScriptSys.lua_set_common_entity_run("500:3", "training.lua::ambulance1", "500", "3", vector3(-42,11,63), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:3", "training.lua::ambulance1", "500", "3", vector3(-1692,11,72), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:3", "training.lua::ambulance1", "500", "3", vector3(-1692,11,1821), 0, 100)
	ScriptSys.lua_set_common_entity_run("500:3", "training.lua::ambulance1", "500", "3", vector3(-2245,11,1917), 0, 100)
	print('ambulance1 state1 end')
	inference_engine:state_work_done("ambulance1_state1")
	--script_manager:end_script_by_name("training.lua::ambulance1")

end

function equal(a, b, eps)
	return math.abs(a - b) < eps
end

function checkPos(src, des, dis)
	if nil == dis then
		dis = 50
	end
	if math.abs(des.x-src.x)<=dis and math.abs(des.z-src.z)<=dis then
		return true
	else
		return false
	end
end
