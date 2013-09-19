#pragma once

#include "script_manager.h"

using namespace luabind;

class script_manager;
class entity_interface;
class inference_engine;

class script : public boost::enable_shared_from_this<script>
{
public:
	lua_State *child_state;
	boost::mutex lua_state_mutex;
	bool ended;

	luabind::object myobj;

	boost::shared_ptr<script_manager> script_mgr;

	std::string file_name;//"xxx.lua"
	std::string entry_function;//"xxx"
	std::string file_path;

	script(boost::shared_ptr<script_manager> s_mgr, std::string f_name, std::string entry_function, boost::shared_ptr<entity_interface> ent_i, boost::shared_ptr<inference_engine> ie);

	void resume(std::string gotolabel);
	void resume();
	int yield();
	void end();

	void set_func_real_person(std::string ent_id);
	void reset_func_real_person(std::string ent_id);
	void set_func_branch(std::string func_name, std::string branch);
	void set_var( std::string var, std::string val );

	void register_c_apis();
	void set_global(boost::shared_ptr<entity_interface> ent_i, boost::shared_ptr<inference_engine> ie, boost::shared_ptr<script_manager> sm);
};