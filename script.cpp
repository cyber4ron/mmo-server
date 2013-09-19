#include "stdafx.h"

#include "script.h"

using namespace std;

script::script( boost::shared_ptr<script_manager> s_mgr, std::string f_name, std::string entry_func, boost::shared_ptr<entity_interface> ent_i, boost::shared_ptr<inference_engine> ie )
{
	script_mgr = s_mgr;
	file_name = f_name;
	file_path = "./drill/" + file_name;
	entry_function = entry_func;
	ended = false;

	child_state = luaL_newstate();
	luaL_openlibs(child_state);
	open(child_state);

	register_c_apis();
	int res = luaL_dofile(child_state, const_cast<char*>(file_path.c_str()));
	set_global(ent_i, ie, script_mgr);
}

void script::resume()
{
	if(ended) return;

	static bool started = false;

	boost::mutex::scoped_lock lock(lua_state_mutex);
	if(!started)
	{
		started =true;
		luabind::resume_function<void>(child_state, const_cast<char*>(entry_function.c_str()));
	}
	else 
	{
		luabind::resume_function<void>(child_state, const_cast<char*>(entry_function.c_str()));
	}
}

void script::resume(std::string gotolabel)
{
	boost::mutex::scoped_lock lock(lua_state_mutex);

	luabind::resume_function<void>(child_state, const_cast<char*>(entry_function.c_str()));
}

int script::yield()
{
	return lua_yield(child_state, 0);
}

void script::end()
{
	ended = true;
}

void script::register_c_apis()
{
	module(child_state)
		[
			class_<vector3>("vector3")
			.def(constructor<double, double, double>())
			.def_readwrite("x", &vector3::x)
			.def_readwrite("y", &vector3::y)
			.def_readwrite("z", &vector3::z)
			.def(self == vector3())
		];

	module(child_state)
		[
			class_<entity_interface>("entity_interface")
			.def(constructor<boost::shared_ptr<entity_manager>>())//opt
			.def("register_entity", &entity_interface::register_entity)
			.def("unregister_entity", &entity_interface::unregister_entity)

			.def("complete_by_npcs", &entity_interface::complete_by_npcs)
			.def("set_actor_move", &entity_interface::set_actor_move)
			.def("get_actor_position", &entity_interface::get_actor_position)
			.def("set_actor_action",&entity_interface::set_actor_action)

			.def("set_button_run",&entity_interface::set_button_run)
			.def("set_valve_run",&entity_interface::set_valve_run)
			.def("get_valve_state",&entity_interface::get_valve_state)

			.def("attach_sub_entity",&entity_interface::attach_sub_entity)
			.def("detach_sub_entity",&entity_interface::detach_sub_entity)

			.def("set_common_entity_run",&entity_interface::set_common_entity_run)
			.def("set_common_entity_pos",&entity_interface::set_common_entity_pos)

			.def("add_particle",&entity_interface::add_particle)
			.def("set_particle_running",&entity_interface::set_particle_running)
			.def("set_particle_run",&entity_interface::set_particle_run)
			.def("set_particle_velocity",&entity_interface::set_particle_velocity)
			.def("set_particle_direction",&entity_interface::set_particle_dir)

			.def("show_text_onscreen",&entity_interface::show_text_onscreen)
			.def("change_text_onscreen",&entity_interface::change_text_onscreen)
			.def("delete_text_onscreen",&entity_interface::delete_text_onscreen)

			.def("push_questions",&entity_interface::push_questions)

			.def("set_entity_position",&entity_interface::set_entity_position)
			.def("set_water_face_move",&entity_interface::set_water_face_move)
			.def("set_knob_turn",&entity_interface::set_knob_turn)
			.def("start_alarm_flicker",&entity_interface::start_alarm_flicker)
			.def("end_alarm_flicker",&entity_interface::end_alarm_flicker)
			.def("add_one_music",&entity_interface::add_one_music)

			.def("check_actor_equipment",&entity_interface::check_actor_equipment)
			.def("move_camera_to_goal",&entity_interface::move_camera_to_goal)
		];

	module(child_state)
		[
			class_<inference_engine>("inference_engine")
			.def(constructor<boost::shared_ptr<script_manager>>())//opt
			.def("state_work_done", &inference_engine::state_work_done)
			.def("add_fact", &inference_engine::add_fact)
		];

	module(child_state)
		[
			class_<script_manager>("script_manager")
			.def(constructor<boost::shared_ptr<server>>())//opt
			.def("end_script_by_name", &script_manager::end_script_by_name)
		]; 
}

void script::set_global( boost::shared_ptr<entity_interface> ent_i, boost::shared_ptr<inference_engine> ie, boost::shared_ptr<script_manager> sm )
{
	luabind::object global_table = luabind::globals(child_state);
	global_table["entity_interface"] = ent_i;
	global_table["inference_engine"] = ie;
	global_table["script_manager"] = sm;
}

void script::set_func_real_person( std::string ent_id )
{
	set_var("is_real_person_" + ent_id, "true");
}

void script::reset_func_real_person( std::string ent_id )
{
	set_var("is_real_person_" + ent_id, "false");
}

void script::set_func_branch( std::string func_name, std::string branch )
{
	set_var(func_name+"_branch_label", branch);
}

void script::set_var( std::string var, std::string val )
{
	luabind::object global_table = luabind::globals(child_state);
	global_table["var"] = val;
}
