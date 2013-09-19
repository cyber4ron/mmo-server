--ScriptSysΪ�ű������ṩ�ű�api����������������
ScriptSys = {}

ScriptSys.walkingRate = 4000
ScriptSys.actionRate = 1
ScriptSys.delaytime = 1000
ScriptSys.valveSpeed = 60
ScriptSys.valveAngle = 360
ScriptSys.valveDir = 1
ScriptSys.carSpeed = 100
--���������ɫ�Ķ�������idle��walking
function	ScriptSys.lua_set_actor_action(id, funcName, idx, name, speed,delaytime)
		if nil == delaytime then
			delaytime=0
		end
		entity_interface:register_entity(id,funcName)
		entity_interface:set_actor_action(idx, name, ScriptSys.actionRate,delaytime)
		coroutine.yield();
		entity_interface:unregister_entity(id,funcName)
end

--���������ƶ�
function	ScriptSys.lua_set_actor_move(id, funcName, idx, goal, speed)
		if nil == speed then
			speed = 10000			
		else
			speed = ScriptSys.walkingRate
		end			
		goal.y = 11 --����������ʼλ��Ӧ����11
		entity_interface:register_entity(id,funcName)
		entity_interface:set_actor_move(idx, goal, speed)
		coroutine.yield();
		entity_interface:unregister_entity(id,funcName)
end

--��������豸
function ScriptSys.lua_attach_sub_entity(id, funcName,myid, boneName, deviceName, delaytime)
	entity_interface:register_entity(id,funcName)
	entity_interface:attach_sub_entity(myid,boneName,deviceName,ScriptSys.delaytime)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

function ScriptSys.lua_detach_sub_entity(id, funcName,myid, boneName, deviceName, delaytime)
	entity_interface:register_entity(id,funcName)
	entity_interface:detach_sub_entity(myid,boneName,deviceName)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

--���Ʒ��ŵ�ת��
function ScriptSys.lua_set_valve_run(id,funcName,idx1,idx2,angle,speed,direction)
	entity_interface:register_entity(id,funcName)
	entity_interface:set_valve_run(idx1,idx2,ScriptSys.valveAngle,ScriptSys.valveSpeed,ScriptSys.valveDir);
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

--���������������
function ScriptSys.lua_set_particle_dir(id, funcName, idx1, idx2, dir)
	entity_interface:register_entity(id,funcName)
	entity_interface:set_particle_direction(idx1, idx2, dir)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

function ScriptSys.lua_set_particle_velocity(id, funcName, idx1, idx2, vmin,vmax)
	entity_interface:register_entity(id,funcName)
	entity_interface:set_particle_velocity(idx1, idx2, vmax,vmin,0,0)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

function ScriptSys.lua_set_particle_run(id, funcName, idx1, idx2, flag)
	entity_interface:register_entity(id,funcName)
	entity_interface:set_particle_run(idx1, idx2, flag)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

--�������������Ȼ����ȸ�����ƶ�
function ScriptSys.lua_set_common_entity_run(id, funcName, idx1, idx2, goal, isTransient, speed)
	entity_interface:register_entity(id,funcName)
	entity_interface:set_common_entity_run( idx1,idx2,goal,isTransient,ScriptSys.carSpeed)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

--���ƴ��졢�����ߵȶ�����ƶ�
function ScriptSys.lua_set_entity_position(id,funcName,idx1,idx2,goal,delaytime)
	entity_interface:register_entity(id,funcName)
	entity_interface:set_entity_position(idx1,idx2,goal,delaytime)	
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

--�������ֵ���ʾ
function ScriptSys.lua_show_text_onscreen(id, funcName, idx, roleid, content, num1, num2, num3, pos, delaytime)
	if nil == delaytime then
		delaytime = 0
	end
	--delaytime = 5000
	entity_interface:register_entity(id,funcName)
	entity_interface:show_text_onscreen(id, funcName, content,num1,num2,num3,pos,delaytime)
	coroutine.yield()
	entity_interface:unregister_entity(id,funcName)
end

--����ˮ��Ĳ���
function ScriptSys.lua_set_water_face_move(id,funcName,idx1,idx2,goal,speed,ifTran,delaytime)
	entity_interface:register_entity(id,funcName)
	entity_interface:set_water_face_move(idx1,idx2,goal,speed,ifTran,delaytime)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

--��ť��ת
function ScriptSys.lua_set_knob_turn(id,funcName,idx1,idx2,angle,delaytime)
	entity_interface:register_entity(id,funcName)
	entity_interface:set_knob_turn(idx1,idx2,angle,delaytime)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

--�������𣬽�����˸
function ScriptSys.lua_start_alarm_flicker(id,funcName,idx1,idx2,frameCount,delaytime)
	entity_interface:register_entity(id,funcName)
	entity_interface:start_alarm_flicker(idx1,idx2,frameCount, delaytime)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

--��������ֹͣ��˸
function ScriptSys.lua_end_alarm_flicker(id,funcName,idx1,idx2,delaytime)
	entity_interface:register_entity(id,funcName)
	entity_interface:end_alarm_flicker(idx1,idx2, delaytime)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

--���һ������������
function ScriptSys.lua_add_one_music(id,funcName,idx,music,volumn,ifloop,clientid,delaytime)
	entity_interface:register_entity(id,funcName)
	entity_interface:add_one_music(idx,music,volumn,ifloop,funcName,clientid,delaytime)
	coroutine.yield();
	entity_interface:unregister_entity(id,funcName)
end

--ScriptSys.lua_set_entity_position(id,funcName,idx1,idx2,goal,delaytime)
--ScriptSys.lua_set_water_face_move(id,funcName,idx1,idx2,goal,speed,ifTran,delaytime)
--ScriptSys.lua_set_knob_turn(id,funcName,idx1,idx2,angle,delaytime)
--ScriptSys.lua_start_alarm_flicker(id,funcName,idx1,idx2,frameCount,delaytime)
--ScriptSys.lua_end_alarm_flicker(id,funcName,idx1,idx2,delaytime)


--entity_interface:show_text_onscreen("text1", "100", "[������]:T-50401��������й¶�������˽����ֳ�����",0,0,30,vector3(1,0,0),0)
